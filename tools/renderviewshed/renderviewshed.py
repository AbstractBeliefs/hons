from mayavi import mlab
from esriparse import esriParser

heightmap_s = """\
NCOLS 5
NROWS 5
0 0 1 0 0
0 0 1 0 0
0 0 0 0 0
0 0 1 0 0
0 0 1 0 0
"""

viewshed_s = """\
NCOLS 5
NROWS 5
1 1 1 0 0
1 1 1 0 1
1 1 1 1 1
1 1 1 0 1
1 1 1 0 0
"""

heightmap = esriParser(heightmap_s).esri()
viewshed = esriParser(viewshed_s).esri()

if heightmap["headers"]["rows"] != viewshed["headers"]["rows"]: raise ValueError("Mismatched raster dimensions")
if heightmap["headers"]["cols"] != viewshed["headers"]["cols"]: raise ValueError("Mismatched raster dimensions")

combination_map = [
    [x for x in range(len(heightmap["data"][0])) for y in range(len(heightmap["data"]))],
    [y for x in range(len(heightmap["data"][0])) for y in range(len(heightmap["data"]))],
]
combination_map.append([heightmap["data"][coord[1]][coord[0]] for coord in zip(combination_map[0], combination_map[1])])
combination_map.append([viewshed["data"][coord[1]][coord[0]] for coord in zip(combination_map[0], combination_map[1])])

vis_map = mlab.points3d(
    combination_map[0],
    combination_map[1],
    combination_map[2],
    combination_map[3],
    mode="cube"
)
vis_map.module_manager.scalar_lut_manager.lut.number_of_colors = 2
vis_map.module_manager.scalar_lut_manager.lut.table = [[255,0,0, 255], [0,255,0, 255]]
vis_map.glyph.scale_mode = "data_scaling_off"
vis_map.glyph.color_mode = "color_by_scalar"

mlab.draw()
mlab.show()
