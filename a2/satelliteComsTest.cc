/*

  satelliteComsTest - This file contains
  unit tests for satelliteComs.

*/

#include "gtest/gtest.h"
extern "C" {
    #include "./satelliteComs.h"
    #include <stdint.h>
    #include "TCB.h"
}

// This tests the "" method in "thrusterSubsystem.c"
TEST(satelliteComsTest, Test_maskBit) {
  // 1. Declare test variables
  unsigned int thrusterCommand = 10; 
  unsigned int *thrusterCommandPtr = &thrusterCommand;
  maskBit(thrusterCommandPtr);
  ASSERT_EQ(*thrusterCommandPtr, 2);
  // 2. Construct thrust commands from
  // all combinations of test variables
}
