"""Behaviour checks for the garden_guardian suite; it is not student code.

usage: gg_dynamic.py <file.py> <ex0|ex1|ex2|ex3|ex4>

Imports the student's file (its __main__ block does not run) and calls the
functions and classes the subject names. Prints "OK|BAD|WARN <text>" lines.
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
    except BaseException as error:
        return None, buffer.getvalue(), error
    return result, buffer.getvalue(), None


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


def returns(label, function, argument, want):
    got, _, error = captured(function, argument)
    if error is None and got == want and type(got) is type(want):
        say("OK", "%s returns %r" % (label, want))
    elif error is not None:
        say("BAD", "%s raised %s: %s" % (label, type(error).__name__, error))
    else:
        say("BAD", "%s should return %r (got %r)" % (label, want, got))


def raises(label, function, argument, kind=Exception):
    _, _, error = captured(function, argument)
    if error is None:
        say("BAD", "%s should raise an exception" % label)
    elif isinstance(error, kind):
        say("OK", "%s raises %s" % (label, type(error).__name__))
    else:
        say("BAD", "%s should raise %s (raised %s)"
            % (label, kind.__name__, type(error).__name__))
    return error


def runs(label, function):
    _, out, error = captured(function)
    if error is not None:
        say("BAD", "%s crashes: %s: %s" % (label, type(error).__name__, error))
        return None
    if not out.strip():
        say("BAD", "%s prints nothing" % label)
        return None
    say("OK", "%s runs without crashing" % label)
    return out


def ex0(module):
    func = need(module, "input_temperature")
    if func:
        returns('input_temperature("25")', func, "25", 25)
        returns('input_temperature("-5")', func, "-5", -5)
        raises('input_temperature("abc")', func, "abc", ValueError)
    test = need(module, "test_temperature")
    if test:
        runs("test_temperature()", test)


def ex1(module):
    func = need(module, "input_temperature")
    if func:
        returns('input_temperature("25")', func, "25", 25)
        returns('input_temperature("0")', func, "0", 0)
        returns('input_temperature("40")', func, "40", 40)
        raises('input_temperature("abc")', func, "abc", ValueError)
        for text in ("100", "41", "-1", "-50"):
            error = raises('input_temperature("%s")' % text, func, text)
            if error is not None and text not in str(error):
                say("WARN", "the error for %s does not mention the "
                    "temperature (%s)" % (text, error))
    test = need(module, "test_temperature")
    if test:
        runs("test_temperature()", test)


def ex2(module):
    func = need(module, "garden_operations")
    if func:
        for number, kind in ((0, ValueError), (1, ZeroDivisionError),
                             (2, FileNotFoundError), (3, TypeError)):
            raises("garden_operations(%d)" % number, func, number, kind)
        for number in (4, -1, 99):
            got, _, error = captured(func, number)
            if error is None and got is None:
                say("OK", "garden_operations(%d) simply returns" % number)
            else:
                say("BAD", "garden_operations(%d) should simply return"
                    % number)
    test = need(module, "test_error_types")
    if test:
        runs("test_error_types()", test)


def ex3(module):
    garden = need(module, "GardenError")
    plant = need(module, "PlantError")
    water = need(module, "WaterError")
    if not (garden and plant and water):
        return
    for cls in (garden, plant, water):
        if not issubclass(cls, Exception):
            say("BAD", "%s must inherit from Exception" % cls.__name__)
            return
    for cls in (plant, water):
        if issubclass(cls, garden) and cls is not garden:
            say("OK", "%s inherits from GardenError" % cls.__name__)
        else:
            say("BAD", "%s must inherit from GardenError" % cls.__name__)
    messages = {}
    for cls in (plant, water):
        instance, _, error = captured(cls)
        if error is not None:
            say("BAD", "%s() without a message fails: %s"
                % (cls.__name__, error))
        elif not str(instance).strip():
            say("BAD", "%s() needs a specific default message"
                % cls.__name__)
        else:
            messages[cls.__name__] = str(instance)
            say("OK", "%s() has the default message %r"
                % (cls.__name__, str(instance)))
        custom, _, error = captured(cls, "boom")
        if error is None and str(custom) == "boom":
            say("OK", "%s('boom') keeps its message" % cls.__name__)
        else:
            say("BAD", "%s('boom') should keep the message it is given"
                % cls.__name__)
    if len(messages) == 2 and len(set(messages.values())) == 1:
        say("WARN", "PlantError and WaterError have the same default "
            "message")
    try:
        raise water("x")
    except garden:
        say("OK", "catching GardenError catches a WaterError")


def ex4(module):
    plant_error = need(module, "PlantError")
    water = need(module, "water_plant")
    test = need(module, "test_watering_system")
    if plant_error and not issubclass(plant_error, Exception):
        say("BAD", "PlantError must inherit from Exception")
    if water:
        for name in ("Tomato", "Lettuce", "Carrots"):
            got, out, error = captured(water, name)
            if error is None and got is None and name in out:
                say("OK", "water_plant(%r) waters it and prints a message"
                    % name)
            else:
                say("BAD", "water_plant(%r) should succeed and print a "
                    "message (%s)" % (name, error or "nothing printed"))
        if plant_error:
            error = raises("water_plant('lettuce')", water, "lettuce",
                           plant_error)
            if error is not None and "lettuce" not in str(error):
                say("WARN", "the PlantError does not mention the name")
    if test:
        _, out, error = captured(test)
        if error is not None:
            say("BAD", "test_watering_system() crashes: %s: %s"
                % (type(error).__name__, error))
        else:
            opened = sum("open" in line.lower() for line in out.splitlines())
            closed = sum("clos" in line.lower() for line in out.splitlines())
            if opened and opened == closed:
                say("OK", "the watering system is closed as often as it "
                    "is opened (%d times)" % opened)
            else:
                say("BAD", "the watering system is opened %d times and "
                    "closed %d times" % (opened, closed))
        if plant_error and water:
            module.water_plant = lambda name: (_ for _ in ()).throw(
                plant_error("forced"))
            _, out, error = captured(test)
            module.water_plant = water
            if error is not None:
                say("BAD", "test_watering_system() does not handle an error "
                    "from water_plant(): %s" % error)
            else:
                opened = sum("open" in l.lower() for l in out.splitlines())
                closed = sum("clos" in l.lower() for l in out.splitlines())
                if opened and opened == closed:
                    say("OK", "cleanup still happens when water_plant() "
                        "fails")
                else:
                    say("BAD", "the watering system must be closed even "
                        "when water_plant() fails (opened %d, closed %d)"
                        % (opened, closed))


try:
    globals()[sys.argv[2]](load(sys.argv[1]))
except Exception as error:
    say("BAD", "the file cannot be imported: %s: %s"
        % (type(error).__name__, error))
