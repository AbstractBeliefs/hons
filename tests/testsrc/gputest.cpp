#include "gtest/gtest.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#include "common.h"
#include "gpu.h"

TEST(GPU, DISABLED_Curve){
    float *heightmap_source = (float*)calloc((20000+1) * (20000+1), sizeof(float));
    vs_heightmap_t heightmap = heightmap_from_array(20000+1, 20000+1, heightmap_source);

    uint32_t emitter_x = heightmap.cols/2, emitter_y = heightmap.rows/2;
    int32_t emitter_z = 5;

    uint32_t horizon = 3.57 * sqrt(emitter_z);

    gpu_curve_map(heightmap);
    vs_viewshed_t viewshed = gpu_calculate_viewshed(heightmap, emitter_x,emitter_y,emitter_z);

    EXPECT_EQ(viewshed.viewshed[(viewshed.rows/2)*viewshed.cols + (viewshed.cols/2)+horizon], true) << "Horizon too close";
    EXPECT_EQ(viewshed.viewshed[(viewshed.rows/2)*viewshed.cols + (viewshed.cols/2)+horizon+1], false) << "Horizon too far";
}

TEST(GPU, CalcViewshed){
    const char* rasterstring = \
        "NCOLS 5\n"
        "NROWS 5\n"
        "XLLCORNER 0\n"
        "YLLCORNER 0\n"
        "CELLSIZE 1\n"
        "0 0 9 0 0\n"
        "0 0 9 0 0\n"
        "0 0 0 0 0\n"
        "0 0 9 0 0\n"
        "0 0 9 0 0\n";

    FILE *rasterfile = fmemopen((char*)rasterstring, strlen(rasterstring), "r");
    vs_heightmap_t heightmap = heightmap_from_file(rasterfile);

    vs_viewshed_t viewshed = gpu_calculate_viewshed(heightmap, 0, 2, 5);

    EXPECT_EQ(viewshed.viewshed[0*viewshed.cols+0], true);
    EXPECT_EQ(viewshed.viewshed[2*viewshed.cols+4], true);
    EXPECT_EQ(viewshed.viewshed[0*viewshed.cols+4], false);
}

TEST(Performance, GPU5k){
    float *heightmap_source = (float*)calloc((5000+1) * (5000+1), sizeof(float));
    vs_heightmap_t heightmap = heightmap_from_array(5000+1, 5000+1, heightmap_source);
    
    uint32_t emitter_x = heightmap.cols/2 + 1, emitter_y = heightmap.rows/2 + 1;
    int32_t emitter_z = 5;
    time_t start_time = time(NULL);
    vs_viewshed_t viewshed = gpu_calculate_viewshed(heightmap, emitter_x,emitter_y,emitter_z);
    time_t end_time = time(NULL);
    uint32_t time_diff = (uint32_t)difftime(end_time, start_time);

    printf("Execution wall clock time: %um%us\n", time_diff/60, time_diff%60);
}
