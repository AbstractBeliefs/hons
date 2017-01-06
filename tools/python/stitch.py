import os
import sys

from fastparse import esriParser

# TODO: make this work for multiple cellsizes

# Lower left and upper right extents
ll_x = None
ll_y = None
ur_x = None
ur_y = None
cell_size = None
maps = []

# Iterate over every file and find the minimum and maximum x and y extents
for root, dirs, files in os.walk(sys.argv[-2]):
    for map_file in [os.path.join(root, f) for f in files if f.endswith(".asc") or f.endswith(".esri")]:
        print "Pre-parsing %s..." % map_file
        try:
            current_map = esriParser(open(map_file, 'r').read()).esri()
            current_ll_x = current_map["headers"]["X_origin"]["offset"]
            current_ll_y = current_map["headers"]["Y_origin"]["offset"]
            current_ur_x = current_ll_x + current_map["headers"]["cols"] * current_map["headers"]["cellsize"]
            current_ur_y = current_ll_y + current_map["headers"]["rows"] * current_map["headers"]["cellsize"]
            if cell_size is None:
                cell_size = current_map["headers"]["cellsize"]
            elif cell_size != current_map["headers"]["cellsize"]:
                raise ValueError("the cell sizes don't match up!")
            else:
                pass
            maps.append(map_file)
        except Exception as e:
            print "Couldn't parse %s. Error: %s" %(map_file, str(e))
            continue

        if current_ll_x < ll_x or ll_x is None: ll_x = current_ll_x
        if current_ll_y < ll_y or ll_y is None: ll_y = current_ll_y
        if current_ur_x > ur_x or ur_x is None: ur_x = current_ur_x
        if current_ur_y > ur_y or ur_y is None: ur_y = current_ur_y

print "Allocating memory for new map..."
x_range = (ur_x - ll_x)/cell_size
y_range = (ur_y - ll_y)/cell_size
new_map = [
    [-9999 for _ in range(x_range)]
    for _ in range(y_range)
]

num_maps = len(maps)
for idx, map_file in enumerate(maps):
    current_map = esriParser(open(map_file, 'r').read()).esri()
    print "[%d/%d] Stitching %s..." %(idx+1, num_maps, map_file)
    y_start = (ur_y - (current_map["headers"]["Y_origin"]["offset"] + (current_map["headers"]["rows"]*current_map["headers"]["cellsize"])))\
              / current_map["headers"]["cellsize"]
    y_extent = current_map["headers"]["rows"]

    x_start = (current_map["headers"]["X_origin"]["offset"] - ll_x) / current_map["headers"]["cellsize"]
    x_extent = current_map["headers"]["cols"]

    for y in range(y_start, y_start+y_extent):
        for x in range(x_start, x_start+x_extent):
            new_map[y][x] = current_map["data"][y-y_start][x-x_start]

print "Writing new map out..."
outfile = open(sys.argv[-1], 'w')
outfile.write("NCOLS %d\n" %y_range)
outfile.write("NROWS %d\n" %x_range)
outfile.write("CELLSIZE %d\n" %cell_size)
outfile.write("XLLCORNER %d\n" %ll_x)
outfile.write("YLLCORNER %d" %ll_y)
for row in new_map:
    outfile.write("\n")
    outfile.write(" ".join(str(e) for e in row))
outfile.close()
