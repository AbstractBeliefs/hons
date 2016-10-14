#include "gtest/gtest.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "common.h"

TEST(Common, HeightmapFromArray){
    int32_t map_array[9] = {
        1, 1, 1,
        1, 3, 1,
        1, 1, 1
    };

    vs_heightmap_t map = heightmap_from_array(3, 3, map_array);
    
    EXPECT_EQ(map.cols, 3);
    EXPECT_EQ(map.rows, 3);

    EXPECT_EQ(map.heightmap[1*map.cols + 1], 3);
    EXPECT_EQ(map.heightmap[0*map.cols + 0], 1);
}

TEST(Common, HeightmapFromFile){
    const char* rasterstring = \
        "NCOLS 5\n"
        "NROWS 5\n"
        "XLLCORNER 200\n"
        "YLLCORNER 500\n"
        "CELLSIZE 1\n"
        "0 0 1 0 0\n"
        "0 1 2 1 0\n"
        "1 2 5 2 1\n"
        "0 1 2 1 0\n"
        "0 0 1 0 0\n";

    FILE *rasterfile = fmemopen((char*)rasterstring, strlen(rasterstring), "r");

    vs_heightmap_t map = heightmap_from_file(rasterfile);

    EXPECT_EQ(map.cols, 5);
    EXPECT_EQ(map.rows, 5);
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

