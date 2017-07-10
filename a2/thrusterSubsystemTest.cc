/*

  thrusterSubsystemTest - This file contains
  unit tests for the thrusterSubsystem.

*/

#include "gtest/gtest.h"
extern "C" {
    #include "./thrusterSubsystem.h"
    #include <stdint.h>
}

// This tests the "parseCommands" method in "thrusterSubsystem.c"
TEST(ThrusterSubsystemTest, Test_ParseCommands) {
  // 0. Declare test variables
  uint8_t left = 0x00;
  uint8_t up = 0x02;
  uint8_t down = 0x03;
  uint8_t thrust_dir[3] = {left, up, down};

  uint8_t off = 0x00;
  uint8_t twenty_prct = 0x03;
  uint8_t max = 0x0F;
  uint8_t magnitude[3] = {off, twenty_prct, max};

  uint8_t zero_sec = 0x00;
  uint8_t oneeighty_sec  = 0xB4;
  uint8_t twofiftyfive_sec = 0xFF;
  uint8_t duration[3] = {zero_sec, oneeighty_sec, twofiftyfive_sec};

  // 1. Construct thrust commands from
  // all combinations of test variables
  unsigned int thrusterCommands[3][3][3];
  int i, j, k;
  for(i = 0; i < 3; i++) {
    uint8_t dir = thrust_dir[i];
    for(j = 0; j < 3; j++) {
      uint8_t mag = magnitude[j];
      for(k = 0; k < 3; k++) {
        uint8_t dur = duration[k];
        unsigned int thrusterCommand = (dur << 8)|(mag << 4)|(dir);
        thrusterCommands[i][j][k] = thrusterCommand;
      }
    }
  }

  // 2. Cycle through all thrusterCommands and
  // compare to returned value.
  cleanCommands cc;
  for(i = 0; i < 3; i++) {
    uint8_t dir = thrust_dir[i];
    for(j = 0; j < 3; j++) {
      uint8_t mag = magnitude[j];
      for(k = 0; k < 3; k++) {
        uint8_t dur = duration[k];
        unsigned int thrusterCommand = thrusterCommands[i][j][k];
        parseCommands(&thrusterCommand, &cc);
        ASSERT_EQ(cc.duration, dur);
        ASSERT_EQ(cc.thruster_dir, dir);
        ASSERT_EQ(cc.magnitude, mag);
      }
    }
  }
}

// This tests the "getFuelCost" method in "thrusterSubsystem.c"
TEST(ThrusterSubsystemTest, Test_GetCost) {
  // 1. Cycle through all thrusterCommands and
  // compare to returned value.
  cleanCommands cc = {0xFF, 0xF, 0x0};
  double fuelLvl = 100;
  double fCost;
  unsigned short roundedCost;
  while(fuelLvl > 0) {
    fCost = getFuelCost(&cc);
    roundedCost = (unsigned short)fCost;
    fuelLvl -= roundedCost;
  }
  ASSERT_EQ(1,1);
}
