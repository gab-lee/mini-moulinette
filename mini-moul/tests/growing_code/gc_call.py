"""Helper for the growing_code checks; it is not student code.

usage: gc_call.py <file.py> <function> [json list of arguments]

Loads the student's file by path, calls the function and prints nothing
itself. A function that returns something other than None is reported on
stderr (exit status 3); any exception shows its traceback (exit status 1).
"""
import importlib.util
import json
import sys

sys.dont_write_bytecode = True
path, name = sys.argv[1], sys.argv[2]
args = json.loads(sys.argv[3]) if len(sys.argv) > 3 else []
spec = importlib.util.spec_from_file_location("student", path)
module = importlib.util.module_from_spec(spec)
spec.loader.exec_module(module)
result = getattr(module, name)(*args)
sys.stdout.flush()
if result is not None:
    sys.stderr.write("RETURNED %r\n" % (result,))
    sys.exit(3)
