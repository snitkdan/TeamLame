/*

  powerSubsystemTest - This file contains
  unit tests for the powerSubsystem.

*/

#include "gtest/gtest.h"
#include "datastructs.h"
#include "powerSubsystem.h"

// This tests the "powerConsum ption" method in "powerSubsystem"
TEST(PowerSubsystemTest, Test_PowerConsumption) {
  unsigned short pConsume = 0;
  unsigned short *pConsumePtr = &pConsume;

  // Case 1: pConsume <= 10 && call == even
  powerConsumption(pConsumePtr);
  ASSERT_EQ(2, *pConsumePtr);

  // Case 2: pConsume <= 10 && call == odd
  powerConsumption(pConsumePtr);
  ASSERT_EQ(1, *pConsumePtr);

  // Case 3: pConsume > 10 && call == even
  pConsume = 11;
  pConsumePtr = &pConsume;
  powerConsumption(pConsumePtr);
  ASSERT_EQ(10, *pConsumePtr);

  // Case 4: pConsume > 10 && call == odd
  powerConsumption(pConsumePtr);
  ASSERT_EQ(12, *pConsumePtr);


}
