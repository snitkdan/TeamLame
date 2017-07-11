/*

  warningAlarmTest - This file contains
  unit tests for the warningAlarmTest.

*/

#include "gtest/gtest.h"
extern "C" {
    #include "warningAlarm.h"
    #include "dataStructs.h"
}

// This tests the "update" method in "warningAlarm.c"
TEST(WarningAlarmTest, Test_Update) {
  // 1. Declare structures to store dummy LED metadata
  LED led1 = {"LED1", NULL, 0, false};
  LED led2 = {"LED2", NULL, 0, false};
  LED led3 = {"LED3", NULL, 0, false};
  LED *leds[3] = {&led1, &led2, &led3};

  // 2. Declare dummy variables for each warning state
  unsigned short zero = 0;
  unsigned short ten = 10;
  unsigned short eleven = 11;
  unsigned short fifty = 50;
  unsigned short fiftyOne = 51;
  unsigned short seventyFive = 75;
  unsigned short hundred = 100;

  unsigned short *empty = &zero;
  unsigned short *ten_prct = &ten;
  unsigned short *eleven_prct = &eleven;
  unsigned short *half_empty = &fifty;
  unsigned short *fiftyOne_prct = &fiftyOne;
  unsigned short *seventyFive_prct = &seventyFive;
  unsigned short *full = &hundred;

  unsigned short *warningStates[7] = {
    empty, ten_prct, eleven_prct, half_empty,
    fiftyOne_prct, seventyFive_prct, full
  };

  // 3. Cycle through each warning state.
  int i, j;
  unsigned short *fuelLvlPtr, *batteryLvlPtr;
  bool batteryLow = false, fuelLow = false;
  for(i = 0; i < 7; i++) { // batteryLvl states loop
    batteryLvlPtr = warningStates[j];
    for(j = 0; j < 7; j++) { // fuelLvl states loop
      fuelLvlPtr = warningStates[j];

      warnData warnStruct = {&batteryLow, &fuelLow, batteryLvlPtr, fuelLvlPtr};
      update(&warnStruct, leds);
      // Case 1: Safe
      if(*batteryLvlPtr > 50 && *fuelLvlPtr > 50) {
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
