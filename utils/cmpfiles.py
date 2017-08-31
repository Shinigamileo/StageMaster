# ----------------------------------------------------------------------------
# "THE NO-WARE LICENSE" (Revision -0):
# Léo Barré wrote this file.  As long as you retain this notice you
# can do whatever you want with this stuff.
# ...That's it.
# ----------------------------------------------------------------------------
import sys

f1 = open(sys.argv[1]).read()
f2 = open(sys.argv[2]).read()

if f1 == f2:
    print("\033[1;32mTrue\033[0m")
    sys.exit(0)
else:
    print("\033[1;31mFalse\033[0m");
    sys.exit(1)
