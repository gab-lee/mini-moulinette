"""Behaviour checks for the data_quest suite; it is not student code.

usage: pq_dynamic.py <file.py> <ex2|ex3|ex5>

Imports the student's file (its __main__ block does not run) and calls
the functions the subject names. Prints "OK|BAD|WARN <text>" lines.
"""
import builtins
import contextlib
import importlib.util
import inspect
import io
import sys

sys.dont_write_bytecode = True


def say(kind, text):
    print("%s %s" % (kind, text))


def load(path):
    spec = importlib.util.spec_from_file_location("student", path)
    module = importlib.util.module_from_spec(spec)
    with contextlib.redirect_stdout(io.StringIO()):
        spec.loader.exec_module(module)
    return module


def need(module, name):
    value = getattr(module, name, None)
    if value is None:
        say("BAD", "no %s" % name)
    return value


def typed_input(lines):
    """Replaces input() by the given lines; returns how many were read."""
    remaining = list(lines)
    original = builtins.input

    def fake(prompt=""):
        print(prompt, end="")
        if not remaining:
            raise EOFError("input() called after the last line")
        return remaining.pop(0)

    builtins.input = fake
    return remaining, original


def ex2(module):
    func = need(module, "get_player_pos")
    if not func:
        return
    cases = [
        (["hello world", "1.0 , 2.5, 3.0"], (1.0, 2.5, 3.0),
         "text, then 1.0 , 2.5, 3.0"),
        (["1,2", "a,b,c", "1,2,3,4", "", "7,8,9"], (7.0, 8.0, 9.0),
         "four invalid lines, then 7,8,9"),
        (["4,abc,5", "-1,0,2.5"], (-1.0, 0.0, 2.5),
         "4,abc,5, then -1,0,2.5"),
    ]
    for lines, want, label in cases:
        remaining, original = typed_input(lines)
        try:
            with contextlib.redirect_stdout(io.StringIO()):
                got = func()
        except BaseException as error:
            say("BAD", "get_player_pos() with %s raised %s: %s"
                % (label, type(error).__name__, error))
            continue
        finally:
            builtins.input = original
        if got == want and isinstance(got, tuple) and not remaining:
            say("OK", "get_player_pos() retries until valid (%s)" % label)
        else:
            say("BAD", "get_player_pos() with %s should return %r "
                "(returned %r, %d lines unread)"
                % (label, want, got, len(remaining)))


def ex3(module):
    func = need(module, "gen_player_achievements")
    if not func:
        return
    seen = []
    for _ in range(12):
        try:
            with contextlib.redirect_stdout(io.StringIO()):
                value = func()
        except Exception as error:
            say("BAD", "gen_player_achievements() raised %s: %s"
                % (type(error).__name__, error))
            return
        if not isinstance(value, set) or not value:
            say("BAD", "gen_player_achievements() must return a "
                "non-empty set (returned %r)" % (value,))
            return
        seen.append(value)
    say("OK", "gen_player_achievements() returns a set of achievements")
    if len({frozenset(s) for s in seen}) > 1:
        say("OK", "the sets are random")
    else:
        say("BAD", "gen_player_achievements() returns the same set every "
            "time")


def ex5(module):
    gen = need(module, "gen_event")
    consume = need(module, "consume_event")
    if gen:
        if not inspect.isgeneratorfunction(gen):
            say("BAD", "gen_event must be a generator function (yield)")
        else:
            stream = gen()
            events = [next(stream) for _ in range(300)]
            if all(isinstance(e, tuple) and len(e) == 2
                   and all(isinstance(x, str) for x in e) for e in events):
                say("OK", "gen_event() yields (name, action) tuples, "
                    "endlessly")
            else:
                say("BAD", "gen_event() must yield (name, action) tuples")
            if len(set(events)) > 3:
                say("OK", "gen_event() picks random events")
            else:
                say("BAD", "gen_event() should pick random players and "
                    "actions")
    if consume:
        if not inspect.isgeneratorfunction(consume):
            say("BAD", "consume_event must be a generator function (yield)")
            return
        items = [("alice", "run"), ("bob", "eat"), ("alice", "run"),
                 ("carl", "swim"), ("dan", "use"), ("bob", "grab")]
        pool = list(items)
        out = list(consume(pool))
        if sorted(out) == sorted(items) and pool == []:
            say("OK", "consume_event() yields every element once and "
                "empties the list")
        else:
            say("BAD", "consume_event(list) should yield each element "
                "once and leave the list empty (yielded %r, left %r)"
                % (out, pool))
        if out == items:
            say("WARN", "consume_event() took the elements in order; the "
                "subject wants a random pick")


try:
    globals()[sys.argv[2]](load(sys.argv[1]))
except Exception as error:
    say("BAD", "the file cannot be imported: %s: %s"
        % (type(error).__name__, error))
