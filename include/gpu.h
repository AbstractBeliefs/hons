#pragma once

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void curve_map(vs_heightmap_t map);
vs_viewshed_t calculate_viewshed(vs_heightmap_t heightmap, uint32_t emitter_x, uint32_t emitter_y, int32_t emitter_z);

#ifdef __cplusplus
}
#endif
