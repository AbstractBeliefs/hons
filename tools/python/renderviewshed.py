from mayavi import mlab
from fastparse import esriParser
import sys

heightmap = esriParser(open(sys.argv[-2], 'r').read()).esri()
viewshed = esriParser(open(sys.argv[-1], 'r').read()).esri()

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
