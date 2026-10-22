#!/usr/bin/env python3
"""Drives minishell in a pseudo-terminal and prints one result line per check:
    ok|<label>
    bad|<label>|<what happened>
The subject asks for the behaviour of bash: ctrl-C shows a new prompt on a new
line, ctrl-D exits, ctrl-\\ does nothing, and each of them acts on a running
command the way bash lets it."""
import os
import pty
import re
import select
import signal
import sys
import tempfile
import time

BIN = os.path.abspath(sys.argv[1])
QUIET = 0.4


def spawn(workdir):
    env = {"PATH": "/usr/bin:/bin", "HOME": workdir, "TERM": "dumb", "LC_ALL": "C",
           "INPUTRC": "/dev/null", "COLUMNS": "200", "LINES": "50"}
    pid, fd = pty.fork()
    if pid == 0:
        os.chdir(workdir)
        os.execve(BIN, [BIN], env)
        os._exit(127)
    return pid, fd


def settle(fd, need_data=False, limit=4.0):
    """Everything the shell prints until it has been quiet for a moment."""
    data = b""
    start = last = time.time()
    while time.time() - start < limit:
        ready, _, _ = select.select([fd], [], [], 0.05)
        if ready:
            try:
                chunk = os.read(fd, 4096)
            except OSError:
                break
            if not chunk:
                break
            data += chunk
            last = time.time()
        elif time.time() - last > QUIET and (data or not need_data):
            break
    return data.decode("utf-8", "replace")


def alive(pid):
    try:
        done, _ = os.waitpid(pid, os.WNOHANG)
    except ChildProcessError:
        return False
    return done == 0


def says(output, word):
    return any(line.strip() == word for line in re.split(r"[\r\n]+", output))


def send(fd, text):
    os.write(fd, text.encode())


def run_line(fd, line):
    send(fd, line + "\n")
    return settle(fd)


class Session:
    def __enter__(self):
        self.dir = tempfile.mkdtemp(prefix="ms_pty_")
        self.pid, self.fd = spawn(self.dir)
        self.start = settle(self.fd, need_data=True)
        return self

    def __exit__(self, *exc):
        try:
            os.kill(self.pid, signal.SIGKILL)
            os.waitpid(self.pid, 0)
        except (ProcessLookupError, ChildProcessError):
            pass
        os.close(self.fd)
        os.rmdir(self.dir)


def result(label, problem):
    print("ok|%s" % label if problem is None else "bad|%s|%s" % (label, problem))
    sys.stdout.flush()


def still_works(s):
    if not alive(s.pid):
        return "the shell exited"
    if not says(run_line(s.fd, "echo ok"), "ok"):
        return "the shell no longer runs commands"
    return None


def check_ctrl_c_prompt():
    with Session() as s:
        if not s.start:
            return "no prompt appeared"
        send(s.fd, "\x03")
        out = settle(s.fd)
        if not alive(s.pid):
            return "ctrl-C killed the shell"
        if "\n" not in out:
            return "ctrl-C did not show a new prompt on a new line"
        return still_works(s)


def check_ctrl_c_status():
    with Session() as s:
        send(s.fd, "\x03")
        settle(s.fd)
        return status_then_works(s, "130", "ctrl-C at the prompt")


def check_ctrl_backslash_prompt():
    with Session() as s:
        send(s.fd, "\x1c")
        out = settle(s.fd)
        if not alive(s.pid):
            return "ctrl-\\ killed the shell"
        if "Quit" in out:
            return "ctrl-\\ printed Quit; it must do nothing at the prompt"
        return still_works(s)


def check_ctrl_d_exit():
    with Session() as s:
        send(s.fd, "\x04")
        end = time.time() + 2.5
        while time.time() < end:
            settle(s.fd, limit=0.2)
            if not alive(s.pid):
                return None
        return "ctrl-D on an empty prompt did not exit the shell"


def status_then_works(s, status, what):
    """$? is read first: any other command would reset it."""
    if not alive(s.pid):
        return "the shell exited"
    out = run_line(s.fd, "echo $?")
    if not says(out, status):
        return "$? after %s is not %s" % (what, status)
    return still_works(s)


def interrupt_cat(key, status, label):
    with Session() as s:
        send(s.fd, "cat\n")
        settle(s.fd)
        send(s.fd, key)
        settle(s.fd)
        return status_then_works(s, status, "%s on a running command" % label)


def check_ctrl_c_cat():
    return interrupt_cat("\x03", "130", "ctrl-C")


def check_ctrl_backslash_cat():
    return interrupt_cat("\x1c", "131", "ctrl-\\")


def check_ctrl_d_cat():
    with Session() as s:
        send(s.fd, "cat\n")
        settle(s.fd)
        send(s.fd, "\x04")
        settle(s.fd)
        return still_works(s)


def check_ctrl_c_heredoc():
    with Session() as s:
        send(s.fd, "cat << EOF\n")
        settle(s.fd)
        send(s.fd, "\x03")
        settle(s.fd)
        return status_then_works(s, "130", "ctrl-C in a heredoc")


CHECKS = [
    ("ctrl-C shows a new prompt on a new line", check_ctrl_c_prompt),
    ("$? is 130 after ctrl-C at the prompt", check_ctrl_c_status),
    ("ctrl-\\ does nothing at the prompt", check_ctrl_backslash_prompt),
    ("ctrl-D on an empty prompt exits", check_ctrl_d_exit),
    ("ctrl-C stops a running command and the shell goes on", check_ctrl_c_cat),
    ("ctrl-\\ stops a running command and the shell goes on", check_ctrl_backslash_cat),
    ("ctrl-D ends the input of a running command, not the shell", check_ctrl_d_cat),
    ("ctrl-C cancels a heredoc and the shell goes on", check_ctrl_c_heredoc),
]

for label, check in CHECKS:
    try:
        result(label, check())
    except Exception as error:  # a broken terminal must not hide the other checks
        result(label, "could not run the check: %s" % error)
