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
    bool pwm = setPWM(PWM_test1);
    ASSERT_TRUE(pwm);
    while(1) {
      PWM_test1 = 0;
      while(PWM_test1 <= 100) {
        PWM_test1 += 10;
        pwm = setPWM(PWM_test1);
        ASSERT_TRUE(pwm);
      }
    }

}

TEST(SolarPanelControlTest, Test_SolarPanelControl) {
  // Initialize();
  // extern solarData solData;
  // while(1) {
  //
  // }
}
