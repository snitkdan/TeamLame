/*
  schedulerTest.cc - This file contains
  unit tests for the solarPanelControls.
*/

#include "gtest/gtest.h"
extern "C" {
    #include <stdbool.h>
    #include "solarPanelControl.h"
    #include "startup.h"
    #include <unistd.h>
}

// This tests the "setPWM" method in "solarPanelControl.c"
TEST(SolarPanelControlTest, Test_SetPWM) {
    double PWM_test1 = 50;
    ASSERT_TRUE(setPWM(PWM_test1));
    PWM_test1 = 0;
    while(PWM_test1 <= 100) {
      PWM_test1 += 10;
      ASSERT_TRUE(setPWM(PWM_test1));
    }
}

TEST(SolarPanelControlTest, Test_SolarPanelControl) {
  // Initialize();
  // extern solarData solData;
  // while(1) {
  //
  // }

}
