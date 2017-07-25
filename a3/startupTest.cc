/*

  startupTest - This file contains
  unit tests for the startup task.

*/

#include "gtest/gtest.h"
extern "C" {
    #include <stdbool.h>
    #include "startup.h"
    #include "dataStructs.h"
    #include "TCB.h"
}

// This tests the "initialize" method in "startup.c"
TEST(StartupTest, Test_Initialization) {
  Initialize();
  // 1. Verify correct initial variable values
  ASSERT_EQ(thrusterCommand, 0);
  ASSERT_EQ(batteryLvl, 100);
  ASSERT_EQ(fuelLvl, 100);
  ASSERT_EQ(pConsume, 0);
  ASSERT_EQ(pGenerate, 0);
  ASSERT_EQ(solarPanelState, false);
  ASSERT_EQ(fuelLow, false);
  ASSERT_EQ(batteryLow, false);
  ASSERT_EQ(solarPanelDeploy, false);
  ASSERT_EQ(solarPanelRetract, false);
  ASSERT_EQ(motorInc, false);
  ASSERT_EQ(motorDec, false);
  ASSERT_EQ(command, '\0');
  ASSERT_EQ(response, '\0');
  // 2. Verify the TCBs correctly initialized
  ASSERT_EQ(powerSubsystemTCB.taskDataPtr, (void*)&pData);
  ASSERT_EQ(powerSubsystemTCB.myTask, powerSubsystem);
  ASSERT_EQ(solarPanelControlTCB.taskDataPtr, (void*)&solData);
  ASSERT_EQ(solarPanelControlTCB.myTask, solarPanelControl);
  ASSERT_EQ(keyboardConsoleTCB.taskDataPtr, (void*)&kData);
  ASSERT_EQ(keyboardConsoleTCB.myTask, keyboardConsole);
  ASSERT_EQ(vehicleCommsTCB.taskDataPtr, (void*)&vData);
  ASSERT_EQ(vehicleCommsTCB.myTask, vehicleComms);
  ASSERT_EQ(thrusterSubsystemTCB.taskDataPtr, (void*)&tData);
  ASSERT_EQ(thrusterSubsystemTCB.myTask, thrusterSubsystem);
  ASSERT_EQ(satelliteComsTCB.taskDataPtr, (void*)&sData);
  ASSERT_EQ(satelliteComsTCB.myTask, satelliteComs);
  ASSERT_EQ(consoleDisplayTCB.taskDataPtr, (void*)&cData);
  ASSERT_EQ(consoleDisplayTCB.myTask, consoleDisplay);
  ASSERT_EQ(warningAlarmTCB.taskDataPtr, (void*)&wData);
  ASSERT_EQ(warningAlarmTCB.myTask, warningAlarm);
  // 3. Verify the Task Queue correctly initialized
  ASSERT_TRUE(queue);
  ASSERT_EQ(queue->num_tasks, 6U);
}

TEST(StartupTest, Test_ActivateTimeBase) {
  ActivateTimeBase();
  ASSERT_EQ(GLOBALCOUNTER, 0U);
}
