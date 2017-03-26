import sys

x = int(sys.argv[-3])
y = int(sys.argv[-2])
filename = sys.argv[-1]

outf = open(filename, 'w')

preamble = """\
NCOLS %d
NROWS %d
CELLSIZE 1
XLLCORNER 0
YLLCORNER 0
"""

outf.write(preamble % (x, y))

for row in range(y):
    outf.write(" ".join('0' for _ in range(x)))
    outf.write("\n")

outf.close()
