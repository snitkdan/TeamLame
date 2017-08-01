/*

  satelliteComsTest - This file contains
  unit tests for satelliteComs.

*/

#include "gtest/gtest.h"
extern "C" {
    #include <stdint.h>
    #include "TCB.h"
    #include "./satelliteComs.h"
}

unsigned long GLOBALCOUNTER = 0;

TEST(satelliteComsTest, Test_maskBit) {
  unsigned int thrusterCommand = 10;
  unsigned int *thrusterCommandPtr = &thrusterCommand;
  maskBit(thrusterCommandPtr);
  ASSERT_EQ(*thrusterCommandPtr, 2);
}
