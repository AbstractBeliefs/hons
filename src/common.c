#include "common.h"

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


vs_heightmap_t heightmap_from_array(uint32_t rows, uint32_t cols, int32_t *input){
    vs_heightmap_t heightmap;

    heightmap.rows = rows;
    heightmap.cols = cols;
    heightmap.cellsize = 1;

    heightmap.corner = true;
    heightmap.xll = 0;
    heightmap.yll = 0;

    heightmap.nodata = -9999;
    heightmap.heightmap = calloc(rows * cols, sizeof(int32_t));

    memcpy(heightmap.heightmap, input, rows*cols*sizeof(int32_t));

    return heightmap;
}


vs_viewshed_t viewshed_from_array(uint32_t rows, uint32_t cols, bool *input){
    vs_viewshed_t viewshed;

    viewshed.rows = rows;
    viewshed.cols = cols;
    viewshed.cellsize = 1;

    viewshed.corner = true;
    viewshed.xll = 0;
    viewshed.yll = 0;

    viewshed.viewshed = calloc(rows * cols, sizeof(int32_t));

    memcpy(viewshed.viewshed, input, rows*cols*sizeof(int32_t));

    return viewshed;
}

vs_viewshed_t viewshed_from_heightmap(vs_heightmap_t heightmap){
    vs_viewshed_t viewshed;

    viewshed.rows = heightmap.rows;
    viewshed.cols = heightmap.cols;
    viewshed.cellsize = heightmap.cellsize;

    viewshed.corner = heightmap.corner;
    viewshed.xll = heightmap.xll;
    viewshed.yll = heightmap.yll;

    viewshed.viewshed = calloc(viewshed.rows * viewshed.cols, sizeof(bool));

    return viewshed;
}
