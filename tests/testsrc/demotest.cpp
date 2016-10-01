#include <stdio.h>
#include "gtest/gtest.h"

TEST(DemoTest, Passes){
    EXPECT_EQ(1, 2-1);
}

TEST(DemoTest, Fails){
    EXPECT_EQ(1, 2);
}
