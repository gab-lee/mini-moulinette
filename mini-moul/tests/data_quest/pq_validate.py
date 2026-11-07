"""Output validators for the data_quest suite; it is not student code.

usage: pq_validate.py <mode> <output file>

Modes: achievements (ex3), stream (ex5), alchemist (ex6). The outputs
are random, so they are checked for consistency instead of against a
fixed text. Prints "OK|BAD|WARN <text>" lines.
"""
import ast
import re
import sys


def say(kind, text):
    print("%s %s" % (kind, text))


def literal(text):
    text = text.strip()
    if text == "set()":
        return set()
    return ast.literal_eval(text)


def find(lines, pattern):
    for line in lines:
        match = re.match(pattern, line)
        if match:
            return match
    return None


def achievements(lines):
    players = {}
    for line in lines:
        match = re.match(r"Player (\w+): (\{.*\})\s*$", line)
        if match:
            players[match.group(1)] = literal(match.group(2))
    if len(players) >= 4:
        say("OK", "%d players have a set of achievements" % len(players))
    else:
        say("BAD", "needs at least 4 players (found %d)" % len(players))
        return
    union = set().union(*players.values())
    common = set.intersection(*players.values())
    match = find(lines, r"All distinct achievements: (.*)$")
    if match and literal(match.group(1)) == union:
        say("OK", "all distinct achievements are the union of the sets")
    else:
        say("BAD", "'All distinct achievements' is not the union of the "
            "players' sets")
    match = find(lines, r"Common achievements: (.*)$")
    if match and literal(match.group(1)) == common:
        say("OK", "common achievements are the intersection")
    else:
        say("BAD", "'Common achievements' is not the intersection of the "
            "players' sets")
    for name, own in players.items():
        others = set().union(*[v for k, v in players.items() if k != name])
        match = find(lines, r"Only %s has: (.*)$" % name)
        if match and literal(match.group(1)) == own - others:
            say("OK", "only %s has: correct" % name)
        else:
            say("BAD", "'Only %s has' should list what nobody else has"
                % name)
        match = find(lines, r"%s is missing: (.*)$" % name)
        if match and literal(match.group(1)) == union - own:
            say("OK", "%s is missing: correct" % name)
        else:
            say("BAD", "'%s is missing' should list the distinct "
                "achievements %s does not have" % (name, name))


def stream(lines):
    events = [re.match(r"Event (\d+): Player (\w+) did action (\w+)\s*$", l)
              for l in lines]
    events = [m for m in events if m]
    numbers = [int(m.group(1)) for m in events]
    if numbers == list(range(1000)):
        say("OK", "1000 events, numbered 0 to 999")
    else:
        say("BAD", "expected the events 0 to 999, found %d event lines"
            % len(events))
    players = {m.group(2) for m in events}
    actions = {m.group(3) for m in events}
    if len(players) > 1 and len(actions) > 1:
        say("OK", "events are random (%d players, %d actions)"
            % (len(players), len(actions)))
    else:
        say("BAD", "events should pick random players and actions")
    match = find(lines, r"Built list of 10 events: (\[.*\])\s*$")
    if not match:
        say("BAD", "no 'Built list of 10 events: [...]' line")
        return
    current = literal(match.group(1))
    if len(current) == 10 and all(isinstance(e, tuple) and len(e) == 2
                                  for e in current):
        say("OK", "built a list of 10 (name, action) tuples")
    else:
        say("BAD", "the list must hold 10 (name, action) tuples")
        return
    start = next(i for i, l in enumerate(lines)
                 if l.startswith("Built list of 10 events"))
    rest = lines[start + 1:]
    taken = []
    for i in range(0, len(rest) - 1, 2):
        got = re.match(r"Got event from list: (\(.*\))\s*$", rest[i])
        remains = re.match(r"Remains in list: (\[.*\])\s*$", rest[i + 1])
        if not got or not remains:
            break
        event, after = literal(got.group(1)), literal(remains.group(1))
        if event not in current or len(after) != len(current) - 1:
            say("BAD", "each step must take one event out of the list "
                "(step %d)" % (len(taken) + 1))
            return
        current.remove(event)
        if sorted(after) != sorted(current):
            say("BAD", "'Remains in list' is wrong at step %d"
                % (len(taken) + 1))
            return
        taken.append(event)
    if len(taken) == 10 and current == []:
        say("OK", "the 10 events are consumed one by one until the list "
            "is empty")
    else:
        say("BAD", "expected 10 'Got event' / 'Remains in list' steps "
            "ending with []")


def alchemist(lines):
    match = find(lines, r"Initial list of players: (\[.*\])\s*$")
    if not match:
        say("BAD", "no 'Initial list of players: [...]' line")
        return
    initial = literal(match.group(1))
    match = find(lines, r"New list with all names capitalized: (\[.*\])")
    if match and literal(match.group(1)) == [n.capitalize() for n in initial]:
        say("OK", "all names capitalized")
    else:
        say("BAD", "'New list with all names capitalized' is wrong")
    match = find(lines, r"New list of capitalized names only: (\[.*\])")
    if match and literal(match.group(1)) == [n for n in initial
                                             if n[:1].isupper()]:
        say("OK", "capitalized names only")
    else:
        say("BAD", "'New list of capitalized names only' should keep only "
            "the names that were capitalized in the initial list")
    match = find(lines, r"Score dict: (\{.*\})\s*$")
    scores = literal(match.group(1)) if match else None
    if scores and set(scores) == {n.capitalize() for n in initial}:
        say("OK", "a score for every capitalized name")
    else:
        say("BAD", "the score dict must have the capitalized names as keys")
        return
    average = sum(scores.values()) / len(scores)
    match = find(lines, r"Score average is ([-0-9.]+)\s*$")
    if match and abs(float(match.group(1)) - average) < 0.006:
        say("OK", "score average %s" % match.group(1))
    else:
        say("BAD", "the score average should be %.2f" % average)
    match = find(lines, r"High scores: (\{.*\})\s*$")
    if match and literal(match.group(1)) == {k: v for k, v in scores.items()
                                             if v > average}:
        say("OK", "high scores are the scores above the average")
    else:
        say("BAD", "'High scores' should hold the scores higher than the "
            "average")


mode, path = sys.argv[1], sys.argv[2]
with open(path, encoding="utf-8") as handle:
    text = handle.read()
try:
    globals()[mode](text.splitlines())
except Exception as error:
    say("BAD", "cannot read the output: %s: %s"
        % (type(error).__name__, error))
