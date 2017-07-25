/*
  schedulerTest.cc - This file contains
  unit tests for the scheduler.
*/

#include "gtest/gtest.h"
extern "C" {
    #include "./scheduler.h"
    #include "startup.h"
}

// This tests the "thrusterSubsystem" method in "thrusterSubsystem.c"
TEST(ThrusterSubsystemTest, Test_ThrusterSubsystem) {
  // 1. Set up data struct
  unsigned int thrusterCommand = 0xFFF0;
  unsigned short fuelLvl = 100;
  thrustData tData = {&thrusterCommand, &fuelLvl};
  void *tDataPtr = (void*)&tData;
  // 2. Store absolute cost for each iteration
  double actual_cost = 0.49132966499999997;
  double actual_fuelLvl = 100.00;
  // 3. Store the difference between the
  // rounded and actual cost.
  double diff, fuelAsDouble;
  while(fuelLvl > 0) {
    thrusterSubsystem(tDataPtr);
    fuelAsDouble = (double)fuelLvl;
    diff = fuelAsDouble - actual_fuelLvl;
    diff = diff < 0 ? diff * -1 : diff;
    ASSERT_TRUE(diff <= 0.51);
    actual_fuelLvl -= actual_cost;
  }
}
