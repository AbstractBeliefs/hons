#include "gtest/gtest.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"

TEST(Common, HeightmapFromArray){
    float map_array[] = {
        1.0, 1.0, 1.0,
        1.0, 3.0, 1.0,
        1.0, 1.0, 1.0,
        2.0, 1.0, 1.0
    };

    vs_heightmap_t map = heightmap_from_array(4, 3, map_array);
    
    EXPECT_EQ(map.cols, 3.0);
    EXPECT_EQ(map.rows, 4.0);

    EXPECT_EQ(map.heightmap[1*map.cols + 1], 3.0);
    EXPECT_EQ(map.heightmap[0*map.cols + 0], 1.0);
    EXPECT_EQ(map.heightmap[3*map.cols + 0], 2.0);
}

TEST(Common, HeightmapFromFile){
    const char* rasterstring = \
        "NCOLS 5\n"
        "NROWS 6\n"
        "XLLCORNER 200\n"
        "YLLCORNER 500\n"
        "CELLSIZE 1\n"
        "0 0 1 0 0\n"
        "0 1 2 1 0\n"
        "1 2 5 2 1\n"
        "0 1 2 1 0\n"
        "0 0 1 0 0\n"
        "1 1 1 0 0\n";

    FILE* rasterfile = fmemopen((char*)rasterstring, strlen(rasterstring), "r");

    vs_heightmap_t map = heightmap_from_file(rasterfile);

    EXPECT_EQ(map.cols, 5);
    EXPECT_EQ(map.rows, 6);
    EXPECT_EQ(map.xll,200);
    EXPECT_EQ(map.yll,500);
    EXPECT_EQ(map.cellsize, 1);
    EXPECT_EQ(map.corner, true);
    EXPECT_EQ(map.heightmap[1*map.cols + 1], 1);
    EXPECT_EQ(map.heightmap[2*map.cols + 2], 5);
}

TEST(Common, ViewshedFromHeightmap){
    vs_heightmap_t heightmap;
    heightmap.rows = 1;
    heightmap.cols = 2;
    heightmap.cellsize = 3;

    heightmap.corner = true;
    heightmap.xll = 4;
    heightmap.yll = 5;

    vs_viewshed_t viewshed = viewshed_from_heightmap(heightmap);

    EXPECT_EQ(viewshed.rows, 1);
    EXPECT_EQ(viewshed.cols, 2);
    EXPECT_EQ(viewshed.cellsize, 3);
    EXPECT_EQ(viewshed.corner, true);
    EXPECT_EQ(viewshed.xll, 4);
    EXPECT_EQ(viewshed.yll, 5);
}

TEST(Common, ViewshedToFile){
    bool viewshed_data[] = {
        0, 0, 1, 0, 0,
        0, 1, 1, 1, 0,
        1, 1, 1, 1, 1,
        0, 1, 1, 1, 0,
        0, 0, 1, 0, 0,
        1, 1, 1, 0, 0
    };

    vs_viewshed_t viewshed;
    viewshed.cols = 5;
    viewshed.rows = 6;
    viewshed.xll = 200;
    viewshed.yll = 500;
    viewshed.corner = true;
    viewshed.cellsize = 10;
    viewshed.viewshed = viewshed_data;

    const char* rasterstring = \
        "NCOLS 5\n"
        "NROWS 6\n"
        "XLLCORNER 200\n"
        "YLLCORNER 500\n"
        "CELLSIZE 10\n"
        "0 0 1 0 0\n"
        "0 1 1 1 0\n"
        "1 1 1 1 1\n"
        "0 1 1 1 0\n"
        "0 0 1 0 0\n"
        "1 1 1 0 0\n";

    char* buffer = (char*)calloc(256, sizeof(char));
    FILE* rasterfile = fmemopen(buffer, 256, "w");

    viewshed_to_file(viewshed, rasterfile);

    EXPECT_EQ(strcmp(buffer, rasterstring), 0);
    free(buffer);
}
