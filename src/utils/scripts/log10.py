import sys
import math

assert( len(sys.argv) == 2)

val1 = float(sys.argv[1])
assert( abs(val1) > 1e-7)

print( math.log10(val1))