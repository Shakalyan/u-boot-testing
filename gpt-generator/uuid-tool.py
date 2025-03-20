#!/bin/python3

import uuid, sys

arg = sys.argv[1]
if arg == 'random':
    u = uuid.uuid4()
else:
    u = uuid.UUID(arg)

h = [hex(n) for n in u.bytes]

# h[0:4] = h[0:4][::-1]
# h[4:6] = h[4:6][::-1]
# h[6:8] = h[6:8][::-1]

#print(h)
print("{", ", ".join(h), "}")
