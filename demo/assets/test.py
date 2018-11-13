import noon

noon.info("Running in Python!")

if 'this' not in globals() or type(this) != noon.Actor:
    noon.error("This script is meant to be run in an Actor")
    exit(1)
