/*

  startupTest.cc - This file contains
  unit tests for the startup task.

*/

#include "gtest/gtest.h"
#include <iostream>
extern "C" {
    #include <stdbool.h>
    #include "startup.h"
    #include "scheduler.h"
    #include "dataStructs.h"
    #include "TCB.h"
}

// This tests the "initialize" method in "startup.c"
TEST(StartupTest, Test_Initialization) {
  // 0. Declare extern variables
  extern unsigned int thrusterCommand;
  extern unsigned short batteryLvl;
  extern unsigned short fuelLvl;
  extern unsigned short pConsume;
  extern unsigned short pGenerate;
  extern bool solarPanelState;
  extern bool fuelLow;
  extern bool batteryLow;
  extern bool solarPanelDeploy;
  extern bool solarPanelRetract;
  extern bool motorInc;
  extern bool motorDec;
  extern char command;
  extern char response;
  extern TCB powerSubsystemTCB;
  extern TCB solarPanelControlTCB;
  extern TCB keyboardConsoleTCB;
  extern TCB vehicleCommsTCB;
  extern TCB thrusterSubsystemTCB;
  extern TCB satelliteComsTCB;
  extern TCB consoleDisplayTCB;
  extern TCB warningAlarmTCB;
  extern powerData pData;
  extern solarData solData;
  extern keyboardData kData;
  extern vehicleData vData;
  extern thrustData tData;
  extern satData sData;
  extern consoleData cData;
  extern warnData wData;
  extern TaskQueue queue;
  // 2. Initialize variables
  Initialize();
  // 3. Verify correct initial variable values
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
  // 4. Verify the TCBs correctly initialized
  ASSERT_EQ(powerSubsystemTCB.taskDataPtr, (void*)&pData);
  ASSERT_EQ(solarPanelControlTCB.taskDataPtr, (void*)&solData);
  ASSERT_EQ(keyboardConsoleTCB.taskDataPtr, (void*)&kData);
  ASSERT_EQ(vehicleCommsTCB.taskDataPtr, (void*)&vData);
  ASSERT_EQ(thrusterSubsystemTCB.taskDataPtr, (void*)&tData);
  ASSERT_EQ(satelliteComsTCB.taskDataPtr, (void*)&sData);
  ASSERT_EQ(consoleDisplayTCB.taskDataPtr, (void*)&cData);
  ASSERT_EQ(warningAlarmTCB.taskDataPtr, (void*)&wData);
  // 5. Verify the Task Queue correctly initialized
  ASSERT_TRUE(queue);
  ASSERT_EQ(NumTasksInTaskQueue(queue), 8U);
}

TEST(StartupTest, Test_ActivateTimeBase) {
  extern unsigned long GLOBALCOUNTER;
  ActivateTimeBase();
  ASSERT_EQ(GLOBALCOUNTER, 0U);
}
