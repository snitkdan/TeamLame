/*

  warningSubsystemTest - This file contains
  unit tests for the warningSubsystem.

*/

#include "gtest/gtest.h"
extern "C" {
    #include "warningAlarm.h"
    #include "dataStructs.h"
}

// This tests the "update" method in "warningAlarm.c"
TEST(WarningAlarmTest, Test_Update) {
  // 1. Declare dummy LED file paths
  static char *l1 = "LED1";
  static char *l2 = "LED2";
  static char *l3 = "LED3";

  // 2. Declare structures to store dummy LED metadata
  static LED led1 = {l1, NULL, 0, false};
  static LED led2 = {l2, NULL, 0, false};
  static LED led3 = {l3, NULL, 0, false};
  static LED *leds[3] = {&led1, &led2, &led3};

  // 3. Declare dummy variables for each warning state
  unsigned short *empty = &0;
  unsigned short *ten_prct = &10;
  unsigned short *eleven_prct = &11;
  unsigned short *half_empty = &50;
  unsigned short *fiftyOne_prct = &51;
  unsigned short *seventyFive_prct = &75;
  unsigned short *full = &100;

  unsigned short *warningStates[7] = {
    empty, ten_prct, eleven_prct, half_empty,
    fiftyOne_prct, seventyFive_prct, full
  }

  // 4. Cycle through each warning state.
  int i, j;
  unsigned short *fuelLvlPtr, *batteryLvlPtr;
  for(i = 0; i < 7; i++) { // batteryLvl states loop
    batteryLvlPtr = warningStates[j];
    for(j = 0; j < 7; j++) { // fuelLvl states loop
      fuelLvlPtr = warningStates[j];
      warnData warnStruct = {&false, &false, batteryLvlPtr, fuelLvlPtr};
      update(&warnStruct, leds);
      // Case 1: Safe
      if(*batteryLvlPtr > 50 && *fuelLvlptr > 50) {
        ASSERT_TRUE(led3.active);
        ASSERT_EQ(-1, led3.sec);
        ASSERT_FALSE(*(warnStruct.batteryLowPtr) && *(warnStruct.fuelLowPtr));
      }
      // Case 2: Alert
      else {
        if(*batteryLvlPtr <= 50) {
          ASSERT_TRUE(led2.active);
          if(*batteryLvlPtr <= 10) {
            ASSERT_TRUE(*(warnStruct.batteryLowPtr));
            ASSERT_EQ(1, led2.sec);
          } else {
            ASSERT_EQ(2, led2.sec);
          }
        }
        if(*fuelLvlPtr <= 50) {
          ASSERT_TRUE(led1.active);
          if(*fuelLvlPtr <= 10) {
            ASSERT_TRUE(*(warnStruct.fuelLowPtr));
            ASSERT_EQ(1, led1.sec);
          } else {
            ASSERT_EQ(2, led1.sec);
          }
        }
      }
    }
  }
}
