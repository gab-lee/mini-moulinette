"""Static checks shared by the Python suites (code_cultivation,
garden_guardian, ...); it is not student code.

usage: py_ast.py <file.py> [--allow a,b] [options]

Always checked: the file parses, classes are PascalCase, functions are
snake_case, and every function and method has type hints. Options add the
structure an exercise asks for:
  --main-guard        the file has an if __name__ == "__main__": block
  --class C           class C is defined
  --method C.m        class C has a method m
  --subclass C:P      class C inherits from P
  --protected C       class C keeps its attributes protected (_x, not x or __x)
  --static C          class C has a static method
  --classmethod C     class C has a class method
  --nested C          class C holds a nested class
  --function          a function is defined outside any class
  --new C:N           C is instantiated at least N times in the file
  --uses name         the file calls name (e.g. super)
  --def name          a function called name is defined
  --try               the file has a try/except
  --finally           the file has a try with a finally block
  --multi-except      one except clause catches several exception types
  --raise             the file has a raise statement
Calls to names that end in Error or Exception (raise ValueError(...)), to
super, and to names the file binds itself (a parameter, a loop variable)
are always allowed.
Prints one line per check: "OK <text>", "BAD <text>" or "WARN <text>".
"""
import ast
import re
import sys


def say(kind, text):
    print("%s %s" % (kind, text))


def decorated_with(node, name):
    for dec in node.decorator_list:
        if isinstance(dec, ast.Name) and dec.id == name:
            return True
    return False


def class_methods(cls):
    return [n for n in cls.body if isinstance(n, ast.FunctionDef)]


def main(argv):
    path = argv[0]
    allow = set()
    wants = []
    i = 1
    while i < len(argv):
        if argv[i] == "--allow":
            allow = set(filter(None, argv[i + 1].split(",")))
            i += 1
        elif argv[i] in ("--main-guard", "--function", "--try", "--finally",
                         "--multi-except", "--raise"):
            wants.append((argv[i], None))
        else:
            wants.append((argv[i], argv[i + 1]))
            i += 1
        i += 1
    try:
        with open(path, encoding="utf-8") as handle:
            tree = ast.parse(handle.read(), path)
    except SyntaxError as error:
        say("BAD", "syntax error line %s: %s" % (error.lineno, error.msg))
        return
    except UnicodeDecodeError:
        say("BAD", "the file is not valid UTF-8")
        return

    classes = {n.name: n for n in ast.walk(tree) if isinstance(n, ast.ClassDef)}
    top_classes = {n.name: n for n in tree.body if isinstance(n, ast.ClassDef)}
    functions = [n for n in ast.walk(tree) if isinstance(n, ast.FunctionDef)]

    bad = [c for c in classes if not re.fullmatch(r"[A-Z][A-Za-z0-9]*", c)]
    if bad:
        say("BAD", "class %s is not in PascalCase" % bad[0])
    else:
        say("OK", "class names are in PascalCase")
    bad = [f.name for f in functions
           if not re.fullmatch(r"_{0,2}[a-z][a-z0-9_]*(__)?", f.name)]
    if bad:
        say("BAD", "function %s is not in snake_case" % bad[0])
    else:
        say("OK", "function names are in snake_case")

    missing = []
    for fn in functions:
        args = fn.args.posonlyargs + fn.args.args + fn.args.kwonlyargs
        for arg in args:
            if arg.arg in ("self", "cls") and arg is args[0]:
                continue
            if arg.annotation is None:
                missing.append("%s(%s)" % (fn.name, arg.arg))
        if fn.returns is None:
            missing.append("%s() has no return type" % fn.name)
    if missing:
        say("BAD", "missing type hints: %s" % ", ".join(missing[:4]))
    elif functions:
        say("OK", "every function and method has type hints")

    bound = set()
    for node in ast.walk(tree):
        if isinstance(node, ast.Name) and isinstance(node.ctx, ast.Store):
            bound.add(node.id)
        elif isinstance(node, ast.arg):
            bound.add(node.arg)
    bad_names = []
    called = set()
    for call in ast.walk(tree):
        if isinstance(call, ast.Call) and isinstance(call.func, ast.Name):
            called.add(call.func.id)
            if call.func.id not in allow and call.func.id not in classes \
                    and call.func.id not in ("cls", "super") \
                    and call.func.id not in bound \
                    and not re.search(r"(Error|Exception)$", call.func.id) \
                    and call.func.id not in {f.name for f in functions} \
                    and call.func.id not in bad_names:
                bad_names.append(call.func.id)
    if bad_names:
        say("BAD", "calls %s, which the subject does not authorize "
            "(authorized: %s)" % (", ".join(n + "()" for n in bad_names),
                                  ", ".join(sorted(allow)) or "none"))
    else:
        say("OK", "only authorized functions are called")

    for kind, arg in wants:
        if kind == "--main-guard":
            guard = any(
                isinstance(n, ast.If) and isinstance(n.test, ast.Compare)
                and isinstance(n.test.left, ast.Name)
                and n.test.left.id == "__name__"
                for n in tree.body)
            if guard:
                say("OK", 'has the if __name__ == "__main__": block')
            else:
                say("BAD", 'no if __name__ == "__main__": block')
        elif kind == "--class":
            if arg in top_classes:
                say("OK", "class %s is defined" % arg)
            else:
                say("BAD", "no class %s" % arg)
        elif kind == "--method":
            cname, mname = arg.split(".")
            cls = classes.get(cname)
            if cls and any(m.name == mname for m in class_methods(cls)):
                say("OK", "%s has a %s() method" % (cname, mname))
            else:
                say("BAD", "class %s has no %s() method" % (cname, mname))
        elif kind == "--subclass":
            cname, pname = arg.split(":")
            cls = classes.get(cname)
            bases = [b.id for b in cls.bases if isinstance(b, ast.Name)] \
                if cls else []
            if pname in bases:
                say("OK", "%s inherits from %s" % (cname, pname))
            else:
                say("BAD", "class %s must inherit from %s" % (cname, pname))
        elif kind == "--protected":
            cls = classes.get(arg)
            attrs = set()
            if cls:
                for node in ast.walk(cls):
                    if isinstance(node, ast.Attribute) \
                            and isinstance(node.ctx, ast.Store) \
                            and isinstance(node.value, ast.Name) \
                            and node.value.id == "self":
                        attrs.add(node.attr)
            public = sorted(a for a in attrs if not a.startswith("_"))
            mangled = sorted(a for a in attrs if a.startswith("__")
                             and not a.endswith("__"))
            if not attrs:
                say("BAD", "%s sets no attribute on self" % arg)
            elif public:
                say("BAD", "%s keeps self.%s public; use the protected "
                    "convention (self._%s)" % (arg, public[0], public[0]))
            elif mangled:
                say("BAD", "%s uses name mangling (self.%s); the subject "
                    "asks for the protected convention (one underscore)"
                    % (arg, mangled[0]))
            else:
                say("OK", "%s keeps its attributes protected" % arg)
        elif kind == "--static":
            cls = classes.get(arg)
            if cls and any(decorated_with(m, "staticmethod")
                           for m in class_methods(cls)) or (
                    cls and any(isinstance(n, ast.Assign)
                                and isinstance(n.value, ast.Call)
                                and isinstance(n.value.func, ast.Name)
                                and n.value.func.id == "staticmethod"
                                for n in cls.body)):
                say("OK", "%s has a static method" % arg)
            else:
                say("BAD", "class %s needs a static method" % arg)
        elif kind == "--classmethod":
            cls = classes.get(arg)
            if cls and any(decorated_with(m, "classmethod")
                           for m in class_methods(cls)) or (
                    cls and any(isinstance(n, ast.Assign)
                                and isinstance(n.value, ast.Call)
                                and isinstance(n.value.func, ast.Name)
                                and n.value.func.id == "classmethod"
                                for n in cls.body)):
                say("OK", "%s has a class method" % arg)
            else:
                say("BAD", "class %s needs a class method" % arg)
        elif kind == "--nested":
            cls = classes.get(arg)
            if cls and any(isinstance(n, ast.ClassDef) for n in cls.body):
                say("OK", "%s holds a nested class" % arg)
            else:
                say("BAD", "class %s needs a nested class" % arg)
        elif kind == "--function":
            inside = set()
            for cls in classes.values():
                for node in ast.walk(cls):
                    inside.add(id(node))
            if any(isinstance(n, ast.FunctionDef) for n in tree.body):
                say("OK", "a function is defined outside any class")
            else:
                say("BAD", "needs a function that is not part of any class")
        elif kind == "--new":
            cname, count = arg.split(":")
            made = sum(1 for n in ast.walk(tree)
                       if isinstance(n, ast.Call)
                       and isinstance(n.func, ast.Name)
                       and n.func.id == cname)
            if made >= int(count):
                say("OK", "%s is instantiated %d times" % (cname, made))
            else:
                say("BAD", "%s must be instantiated at least %s times "
                    "(found %d)" % (cname, count, made))
        elif kind == "--def":
            if any(isinstance(n, ast.FunctionDef) and n.name == arg
                   for n in tree.body):
                say("OK", "function %s is defined" % arg)
            else:
                say("BAD", "no function named %s at the top of the file"
                    % arg)
        elif kind == "--try":
            if any(isinstance(n, ast.Try) for n in ast.walk(tree)):
                say("OK", "uses try/except")
            else:
                say("BAD", "no try/except block")
        elif kind == "--finally":
            if any(isinstance(n, ast.Try) and n.finalbody
                   for n in ast.walk(tree)):
                say("OK", "uses a finally block")
            else:
                say("BAD", "no finally block")
        elif kind == "--raise":
            if any(isinstance(n, ast.Raise) for n in ast.walk(tree)):
                say("OK", "raises an exception")
            else:
                say("BAD", "no raise statement")
        elif kind == "--multi-except":
            if any(isinstance(h.type, ast.Tuple)
                   for n in ast.walk(tree) if isinstance(n, ast.Try)
                   for h in n.handlers):
                say("OK", "one except clause catches several exception types")
            else:
                say("BAD", "no except clause catching several exception "
                    "types at once, e.g. except (ValueError, TypeError)")
        elif kind == "--uses":
            if arg in called:
                say("OK", "calls %s()" % arg)
            else:
                say("BAD", "never calls %s()" % arg)


main(sys.argv[1:])
