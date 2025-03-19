#!/bin/python3

import uuid, sys

arg = sys.argv[1]
if arg == 'random':
    u = uuid.uuid4()
else:
    u = uuid.UUID(arg)

print(u)
print("{", ", ".join(hex(n) for n in u.bytes), "}")
