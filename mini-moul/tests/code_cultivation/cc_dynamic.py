"""Behaviour checks for the code_cultivation suite; it is not student code.

usage: cc_dynamic.py <file.py> <exercise>

Imports the student's file (its __main__ block does not run) and tries the
Plant class the way the subject describes it. The subject leaves the
attribute names free, so only what it names is used: Plant(name, height,
age), show(), grow(), age(), the getters and setters of exercise 4, the
static and class methods of exercise 6. Prints "OK|BAD|WARN <text>" lines.
"""
import contextlib
import importlib.util
import io
import sys

sys.dont_write_bytecode = True


def say(kind, text):
    print("%s %s" % (kind, text))


def captured(function, *args):
    buffer = io.StringIO()
    try:
        with contextlib.redirect_stdout(buffer):
            result = function(*args)
    except Exception as error:
        return None, buffer.getvalue(), error
    return result, buffer.getvalue(), None


def load(path):
    spec = importlib.util.spec_from_file_location("student", path)
    module = importlib.util.module_from_spec(spec)
    sink = io.StringIO()
    with contextlib.redirect_stdout(sink):
        spec.loader.exec_module(module)
    return module


def numbers(plant):
    return {k: v for k, v in vars(plant).items()
            if isinstance(v, (int, float)) and not isinstance(v, bool)}


def build(module, strict):
    """Plant("Rose", 25.0, 30), or None when the constructor differs."""
    plant_class = getattr(module, "Plant", None)
    if plant_class is None:
        say("BAD", "no class Plant to try")
        return None
    plant, _, error = captured(plant_class, "Rose", 25.0, 30)
    if error is not None:
        if strict:
            say("BAD", 'Plant("Rose", 25.0, 30) fails (%s): a plant is '
                "created with its name, height and age" % error)
        else:
            say("WARN", 'Plant("Rose", 25.0, 30) fails (%s): the '
                "behaviour of the class is not tried" % error)
        return None
    return plant


def show_check(plant):
    _, out, error = captured(plant.show)
    if error is not None:
        say("BAD", "show() fails: %s" % error)
    elif "Rose" in out and "25" in out and "30" in out:
        say("OK", "show() prints the name, height and age")
    else:
        say("BAD", "show() should print the name, height and age "
            "(printed %r)" % out)


def ex1(module):
    plant = build(module, False)
    if plant is not None:
        show_check(plant)


def ex2(module):
    plant = build(module, False)
    if plant is None:
        return
    before = numbers(plant)
    _, _, error = captured(plant.grow)
    after = numbers(plant)
    if error is not None:
        say("BAD", "grow() fails: %s" % error)
    elif any(after.get(k, 0) > v for k, v in before.items()):
        say("OK", "grow() makes the plant bigger")
    else:
        say("BAD", "grow() should increase a number of the plant")
    before = numbers(plant)
    _, _, error = captured(plant.age)
    after = numbers(plant)
    if error is not None:
        say("BAD", "age() fails: %s" % error)
    elif any(after.get(k, 0) > v for k, v in before.items()):
        say("OK", "age() makes the plant older")
    else:
        say("BAD", "age() should increase a number of the plant")


def ex3(module):
    plant = build(module, True)
    if plant is None:
        return
    show_check(plant)
    if hasattr(plant, "grow"):
        _, _, error = captured(plant.grow)
        if error is None:
            say("OK", "grow() works right after construction")
        else:
            say("BAD", "grow() fails right after construction: %s" % error)


def ex4(module):
    plant = build(module, True)
    if plant is None:
        return
    checks = [("get_height", 25), ("get_age", 30)]
    for name, want in checks:
        got, _, error = captured(getattr(plant, name, None) or (lambda: 1 / 0))
        if error is None and got == want:
            say("OK", "%s() returns %s" % (name, want))
        else:
            say("BAD", "%s() should return %s (got %r)" % (name, want, got))
    plant.set_height(40)
    plant.set_age(50)
    if plant.get_height() == 40 and plant.get_age() == 50:
        say("OK", "set_height() and set_age() store valid values")
    else:
        say("BAD", "set_height(40) and set_age(50) should be kept")
    for setter, getter, value, keep in (("set_height", "get_height", -5, 40),
                                        ("set_age", "get_age", -3, 50)):
        _, out, error = captured(getattr(plant, setter), value)
        if error is not None:
            say("BAD", "%s(%d) fails: %s" % (setter, value, error))
        elif not out.strip():
            say("BAD", "%s(%d) should print an error message" % (setter, value))
        elif getattr(plant, getter)() != keep:
            say("BAD", "%s(%d) changed the value instead of rejecting it"
                % (setter, value))
        else:
            say("OK", "%s(%d) is rejected with a message" % (setter, value))
    bad, out, error = captured(module.Plant, "Bad", -1.0, -1)
    if error is not None:
        say("BAD", "Plant with negative values fails: %s" % error)
    elif bad.get_height() < 0 or bad.get_age() < 0:
        say("BAD", "a plant created with negative values must not keep them")
    else:
        say("OK", "a plant created with negative values is safe")


def ex5(module):
    for name in ("Flower", "Tree", "Vegetable"):
        child = getattr(module, name, None)
        plant = getattr(module, "Plant", None)
        if child is None or plant is None:
            continue
        if issubclass(child, plant) and child is not plant:
            say("OK", "%s is a Plant" % name)
        else:
            say("BAD", "%s must inherit from Plant" % name)
        if child.show is plant.show:
            say("BAD", "%s must override show()" % name)


def ex6(module):
    plant_class = getattr(module, "Plant", None)
    if plant_class is None:
        say("BAD", "no class Plant to try")
        return
    statics = [(k, v) for k, v in vars(plant_class).items()
               if isinstance(v, staticmethod)]
    if statics:
        name, method = statics[0]
        old, _, e1 = captured(method.__func__, 400)
        young, _, e2 = captured(method.__func__, 30)
        if e1 is None and e2 is None and old and not young:
            say("OK", "%s(400) is true and %s(30) is false" % (name, name))
        else:
            say("BAD", "the static method %s should be true for 400 days "
                "and false for 30" % name)
    makers = [(k, v) for k, v in vars(plant_class).items()
              if isinstance(v, classmethod)]
    if makers:
        name, method = makers[0]
        plant, _, error = captured(getattr(plant_class, name))
        if error is not None:
            say("WARN", "the class method %s() needs arguments: not "
                "tried" % name)
        elif isinstance(plant, plant_class):
            say("OK", "%s() builds a Plant without information" % name)
        else:
            say("BAD", "the class method %s() should return a Plant" % name)
    seed = getattr(module, "Seed", None)
    flower = getattr(module, "Flower", None)
    if seed is not None and flower is not None and not issubclass(seed, flower):
        say("BAD", "Seed must inherit from Flower")


def instances(path, minimum):
    """Runs the file as a script and counts the Plant objects it makes."""
    import builtins
    import runpy
    count = [0]
    original = builtins.__build_class__

    def counting_new(klass, *args, **kwargs):
        count[0] += 1
        return object.__new__(klass)

    def build(func, name, *bases, **kwargs):
        klass = original(func, name, *bases, **kwargs)
        if name == "Plant" and "__new__" not in vars(klass):
            klass.__new__ = counting_new
        return klass

    builtins.__build_class__ = build
    try:
        sink = io.StringIO()
        with contextlib.redirect_stdout(sink):
            runpy.run_path(path, run_name="__main__")
    except Exception as error:
        say("WARN", "cannot count the plants: %s" % error)
        return
    finally:
        builtins.__build_class__ = original
    if count[0] >= minimum:
        say("OK", "the program creates %d plants" % count[0])
    else:
        say("BAD", "the program must create at least %d plants (it "
            "creates %d)" % (minimum, count[0]))


def main(argv):
    if argv[1] == "instances":
        instances(argv[0], int(argv[2]))
        return
    module = load(argv[0])
    globals()[argv[1]](module)


try:
    main(sys.argv[1:])
except Exception as error:
    say("BAD", "the file cannot be imported: %s: %s"
        % (type(error).__name__, error))
