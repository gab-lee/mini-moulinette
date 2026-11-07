#!/bin/bash

# The subject (IV.1) asks for Python 3.10+, flake8 style and mypy type hints.
# Python is needed to run anything; flake8 and mypy only get a [!] when
# missing, since the checks that use them are then skipped.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./utils/py_check.sh

py_need_python
version="$(python3 -c 'import sys; print("%d.%d.%d" % sys.version_info[:3])')"
if python3 -c 'import sys; sys.exit(sys.version_info < (3, 10))'; then
	pc_ok "python3 $version (3.10 or newer)"
else
	pc_bad "python3 $version is older than 3.10, which the subject requires"
fi
for tool in flake8 mypy; do
	if [ -n "$(py_tool $tool)" ]; then
		pc_ok "$tool is installed"
	else
		pc_warn "$tool is not installed; install it (pip install $tool) so the checks that use it can run"
	fi
done
exit $pc_error
