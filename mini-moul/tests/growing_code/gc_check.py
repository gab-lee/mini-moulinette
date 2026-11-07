"""Static checks for the growing_code suite; it is not student code.

usage: gc_check.py <file.py> [--allow a,b] [--function f]... [--typed]
                   [--recursive f]

Prints one line per check: "OK <text>", "BAD <text>" or "WARN <text>".
The subject (IV) wants a file that holds only the requested function(s):
no main block, no call at module level, only the authorized functions.
"""
import ast
import sys


def say(kind, text):
    print("%s %s" % (kind, text))


def calls_in(node):
    for sub in ast.walk(node):
        if isinstance(sub, ast.Call):
            yield sub


def main(argv):
    path = argv[0]
    allow = set()
    functions = []
    typed = False
    recursive = None
    i = 1
    while i < len(argv):
        if argv[i] == "--allow":
            allow = set(filter(None, argv[i + 1].split(",")))
            i += 1
        elif argv[i] == "--function":
            functions.append(argv[i + 1])
            i += 1
        elif argv[i] == "--typed":
            typed = True
        elif argv[i] == "--recursive":
            recursive = argv[i + 1]
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

    top = {n.name: n for n in tree.body if isinstance(n, ast.FunctionDef)}
    for name in functions:
        if name in top:
            say("OK", "function %s is defined" % name)
        else:
            say("BAD", "no function named %s at the top of the file" % name)

    outside = []
    imports = False
    for node in tree.body:
        if isinstance(node, ast.FunctionDef):
            continue
        if isinstance(node, ast.Expr) and isinstance(node.value, ast.Constant) \
                and isinstance(node.value.value, str):
            continue
        if isinstance(node, (ast.Import, ast.ImportFrom)):
            imports = True
            continue
        outside.append(node)
    if outside:
        first = outside[0]
        what = "an if __name__ block" if isinstance(first, ast.If) else \
            "code"
        say("BAD", "%s outside a function (line %d): the file may only hold "
            "the requested function" % (what, first.lineno))
    else:
        say("OK", "only functions at the top of the file")
    if imports:
        say("WARN", "the file imports a module; none is authorized by the "
            "subject")

    defined = {n.name for n in ast.walk(tree) if isinstance(n, ast.FunctionDef)}
    bad_names = []
    methods = False
    for call in calls_in(tree):
        if isinstance(call.func, ast.Name):
            if call.func.id not in allow and call.func.id not in defined \
                    and call.func.id not in bad_names:
                bad_names.append(call.func.id)
        elif isinstance(call.func, ast.Attribute):
            methods = True
    if bad_names:
        say("BAD", "calls %s, which the subject does not authorize "
            "(authorized: %s)" % (", ".join(n + "()" for n in bad_names),
                                  ", ".join(sorted(allow)) or "none"))
    else:
        say("OK", "only authorized functions are called")
    if methods and not typed:
        say("WARN", "a method call is used; the subject authorizes only %s "
            "here" % (", ".join(n + "()" for n in sorted(allow))))

    if typed and functions:
        node = top.get(functions[0])
        if node is not None:
            args = [(a.arg, ast.unparse(a.annotation) if a.annotation else None)
                    for a in node.args.args]
            ret = ast.unparse(node.returns) if node.returns else None
            want = [("seed_type", "str"), ("quantity", "int"), ("unit", "str")]
            if args == want and ret == "None" and not node.args.vararg \
                    and not node.args.kwarg and not node.args.kwonlyargs \
                    and not node.args.defaults:
                say("OK", "signature is (seed_type: str, quantity: int, "
                    "unit: str) -> None")
            else:
                say("BAD", "the signature must be def ft_seed_inventory("
                    "seed_type: str, quantity: int, unit: str) -> None")

    if recursive:
        selfcalling = set()
        for fn in ast.walk(tree):
            if isinstance(fn, ast.FunctionDef):
                for call in calls_in(fn):
                    if isinstance(call.func, ast.Name) \
                            and call.func.id == fn.name:
                        selfcalling.add(fn.name)
        target = top.get(recursive)
        if target is not None:
            nested = {n.name for n in ast.walk(target)
                      if isinstance(n, ast.FunctionDef)}
            called = {c.func.id for c in calls_in(target)
                      if isinstance(c.func, ast.Name)}
            if recursive in selfcalling or nested & selfcalling \
                    or called & selfcalling:
                say("OK", "%s is recursive" % recursive)
            else:
                say("BAD", "%s must use recursion (no function in the file "
                    "calls itself)" % recursive)


main(sys.argv[1:])
