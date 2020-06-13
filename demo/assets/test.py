import noon
import random

if 'this' not in locals() or type(this) != noon.Actor:
    noon.error("This script is meant to be run in an Actor")
    exit(1)

this.set_position(200, 200)
this.set_scale(2, 2)
this.set_rotation(random.random() * 90.0)
