#pragma once

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint32_t rows;      // Size and shape
    uint32_t cols;      // of grid
    uint16_t cellsize;

    bool corner;        // Corner or center origin
    int32_t xll;        // x-lower-left corner/center
    int32_t yll;        // y-lower-left corner/center

    int32_t nodata;     // null value. Default -9999

    int32_t* heightmap; // Data goes here
} vs_heightmap_t;

typedef struct {
    uint32_t rows;      // Size and shape
    uint32_t cols;      // of grid
    uint16_t cellsize;

    bool corner;        // Corner or center origin
    int32_t xll;        // x-lower-left corner/center
    int32_t yll;        // y-lower-left corner/center

    bool* viewshed;     // Data goes here
} vs_viewshed_t;

vs_heightmap_t heightmap_from_array(uint32_t rows, uint32_t cols, int32_t *input);
vs_heightmap_t heightmap_from_file(FILE* inputfile);

vs_viewshed_t viewshed_from_array(uint32_t rows, uint32_t cols, bool *input);
void viewshed_to_file(vs_viewshed_t viewshed, FILE* outputfile);
vs_viewshed_t viewshed_from_heightmap(vs_heightmap_t heightmap);

#ifdef __cplusplus
}
#endif
