import noon

if 'this' not in globals() or type(this) != noon.Actor:
    noon.error("This script is meant to be run in an Actor")
    exit(1)

this.set_name("Player")
print(this.get_name())

this.set_position(200, 200)
this.set_scale(2, 2)
this.set_rotation(45.0)
