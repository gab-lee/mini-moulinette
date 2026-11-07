"""Behaviour checks for exercise 3 of the data_archivist suite; it is not
student code.

usage: pa_dynamic.py <file.py> <work directory>

Imports the student's file (its __main__ block does not run), goes to the
work directory and calls secure_archive() the way the subject describes:
secure_archive(file name, action, content), the action being an int or a
str of the student's choice. Prints "OK|BAD|WARN <text>" lines.
"""
import contextlib
import importlib.util
import io
import os
import sys

sys.dont_write_bytecode = True
CONTENT = ("[FRAGMENT 001] Digital preservation protocols established 2087\n"
           "[FRAGMENT 002] Knowledge must survive the entropy wars\n"
           "[FRAGMENT 003] Every byte saved is a victory against "
           "oblivion\n")


def say(kind, text):
    print("%s %s" % (kind, text))


def call(function, *args):
    buffer = io.StringIO()
    try:
        with contextlib.redirect_stdout(buffer):
            return function(*args), None
    except BaseException as error:
        return None, error


def shaped(label, result, error):
    if error is not None:
        say("BAD", "%s raised %s: %s (it must return a tuple)"
            % (label, type(error).__name__, error))
        return False
    if not (isinstance(result, tuple) and len(result) == 2
            and isinstance(result[0], bool) and isinstance(result[1], str)):
        say("BAD", "%s must return a (bool, str) tuple (returned %r)"
            % (label, result))
        return False
    return True


def main(path, work):
    spec = importlib.util.spec_from_file_location("student", path)
    module = importlib.util.module_from_spec(spec)
    with contextlib.redirect_stdout(io.StringIO()):
        spec.loader.exec_module(module)
    func = getattr(module, "secure_archive", None)
    if func is None:
        say("BAD", "no function secure_archive")
        return
    os.chdir(work)
    with open("fragment.txt", "w", encoding="utf-8") as handle:
        handle.write(CONTENT)
    os.mkdir("a_directory")

    result, error = call(func, "/not/existing/file")
    if shaped("secure_archive('/not/existing/file')", result, error):
        if result[0] is False and "No such file" in result[1] \
                and "/not/existing/file" in result[1]:
            say("OK", "reading a nonexistent file returns (False, the "
                "error message)")
        else:
            say("BAD", "reading a nonexistent file should return (False, "
                "the error message), got %r" % (result,))
    result, error = call(func, "a_directory")
    if shaped("secure_archive('a_directory')", result, error):
        if result[0] is False and result[1]:
            say("OK", "reading a directory returns (False, a message)")
        else:
            say("BAD", "reading a directory should return (False, a "
                "message), got %r" % (result,))
    result, error = call(func, "fragment.txt")
    if shaped("secure_archive('fragment.txt')", result, error):
        if result == (True, CONTENT):
            say("OK", "reading a file returns (True, its content)")
        else:
            say("BAD", "reading a file should return (True, its content), "
                "got %r" % (result,))

    written = None
    for action in ("write", 1, "w", "WRITE"):
        if os.path.exists("new.txt"):
            os.remove("new.txt")
        result, error = call(func, "new.txt", action, CONTENT)
        if error is None and isinstance(result, tuple) and result \
                and result[0] is True and os.path.exists("new.txt"):
            written = action
            break
    if written is None:
        say("WARN", "could not make secure_archive() write a file with "
            "the action 'write', 1, 'w' or 'WRITE': the writing part is "
            "not tried (the subject lets you choose the action)")
        return
    if shaped("secure_archive('new.txt', %r, content)" % (written,),
              result, error):
        with open("new.txt", encoding="utf-8") as handle:
            saved = handle.read()
        if result[0] is True and result[1] and saved == CONTENT:
            say("OK", "writing returns (True, a message) and saves the "
                "content (action %r)" % (written,))
        else:
            say("BAD", "writing should return (True, a message) and save "
                "the content (returned %r, file holds %r)"
                % (result, saved))
    result, error = call(func, "new.txt", written, "second\n")
    with open("new.txt", encoding="utf-8") as handle:
        saved = handle.read()
    if error is None and saved == "second\n":
        say("OK", "writing replaces an existing file")
    else:
        say("BAD", "writing to an existing file should replace its "
            "content (it holds %r)" % (saved,))
    result, error = call(func, "a_directory", written, "x")
    if shaped("secure_archive('a_directory', %r, 'x')" % (written,),
              result, error):
        if result[0] is False and result[1]:
            say("OK", "writing to a directory returns (False, a message)")
        else:
            say("BAD", "writing to a directory should return (False, a "
                "message), got %r" % (result,))
    result, error = call(func, "no_such_dir/new.txt", written, "x")
    if shaped("secure_archive('no_such_dir/new.txt', %r, 'x')"
              % (written,), result, error):
        if result[0] is False:
            say("OK", "writing in a missing folder returns (False, ...)")
        else:
            say("BAD", "writing in a missing folder should fail")


try:
    main(sys.argv[1], sys.argv[2])
except Exception as error:
    say("BAD", "the file cannot be imported: %s: %s"
        % (type(error).__name__, error))
