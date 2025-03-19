#!/bin/python3

import uuid, sys

arg = sys.argv[1]
if arg == 'random':
    u = uuid.uuid4()
else:
    u = uuid.UUID(arg)

h = [hex(n) for n in u.bytes]

print(h)
#print("{", ", ".join(hex(n) for n in u.bytes), "}")
