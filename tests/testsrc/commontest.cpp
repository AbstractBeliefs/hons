#include "gtest/gtest.h"

#include <stdint.h>
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
