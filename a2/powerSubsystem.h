/*

  powerSubsystem.h - This file contains the
  declaration & specification of powerSubsystem.
  (see powerSubsystem.c for implementation).

*/

#include <stdbool.h>

/*
  @param pData
    Struct containing necessary
    data for the powerSubsystem.
  @modifies
    *pData.batteryLvlPtr &&
    *pData.solarPanelStatePtr &&
    *pData.pConsumePtr &&
    *pData.pGeneratePtr
  @effects
    changes batteryLvlPtr based
    on solarPanelStatePtr, pConsumePtr,
    and pGeneratePtr.
    (see sub-methods for details.)
*/
void powerSubsystem(void *pData);

/*
  @param solarPanelState
    whether solar panel is ON or OFF
  @param pGenerate
    current value of power generation
  @param batteryLvl
    current value of the battery level
  @modifies
    pGenerate && solarPanelState
  @effects
    changes pGenerate based on solarPanelState
  @returns
    TRUE if the exit state of the solar panel is ON.
*/
bool useSolarPanels(bool *solarPanelState, unsigned short *pGenerate, unsigned short *batteryLvl);

/*
  @param pConsume
    current value of power consumption
  @modifies pConsume
  @effects
    increments pConsume by 2
    every even numbered function call, and
    decriments it by 1 otherwise. If pConsume
    exceeds 10, these conditions are reversed;
    after which, if pConsume falls below 5,
    the original conditions are reinstated.
*/
void powerConsumption(unsigned short *pConsume);

/*
  @param pGenerate
    current value of power generation
  @modifies pGenerate
  @effects
    If batteryLvl <= 50%, increments pGenerate by 2
    every even numbered function call, and by 1 every odd.
    If 50% < batteryLvl <= 95%, increments pGenerate by 2
    every even numbered function call.
    Otherwise, do nothing.
*/
void powerGeneration(unsigned short *pGenerate, unsigned short *batteryLvl);
