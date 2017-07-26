/*
  schedulerTest.cc - This file contains
  unit tests for the solarPanelControls.
*/

#include "gtest/gtest.h"
extern "C" {
    #include <stdbool.h>
    #include "./solarPanelControl.h"
}

// This tests the "setPWM" method in "solarPanelControl.c"
TEST(SolarPanelControlTest, Test_SetPWM) {
    double PWM_test1 = 50;
    double PWM_test2 = 50;
    double PWM_test3 = 50;
    ASSERT_TRUE(setPWM(PWM_test1));
    ASSERT_TRUE(setPWM(PWM_test2));
    ASSERT_TRUE(setPWM(PWM_test3));
}
