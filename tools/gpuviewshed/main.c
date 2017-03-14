#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "common.h"
#include "gpu.h"

int main(int argc, char* argv[]){
    if (argc != 6){ printf("Expects 5 arguments\n"); return 1; }
    // name x y z if of
    //    0 1 2 3  4  5
    FILE* heightmap_file = fopen(argv[4], "r");
    FILE* viewshed_file = fopen(argv[5], "w");

    uint32_t x = atoi(argv[1]);
    uint32_t y = atoi(argv[2]);
    uint32_t z = atoi(argv[3]);

    if (heightmap_file == NULL){ printf("Couldn't open heightmap file\n"); return 2; }
    if (viewshed_file == NULL){ printf("Couldn't open viewshed file\n"); return 3; }

    vs_heightmap_t heightmap = heightmap_from_file(heightmap_file);
    vs_viewshed_t viewshed = gpu_calculate_viewshed(heightmap, x, y, z);

    viewshed_to_file(viewshed, viewshed_file);

    return 0;
}
