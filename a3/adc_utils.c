/*
 * adc_utils.c - This file contains
 * the implementation for ADC utility functions.
 * (see adc_utils.h for specification)
 *
 * Inspired by Gavin Strunk:
 *  https://class.ee.washington.edu/474/peckol/code/BeagleBone/ForestExamples/Example_5_Analog/libBBB.c
 *
 */

bool adc_initialized = false;

#define DEVICES "/sys/devices"
#define HNUM 14
#define MGRNUM 9

bool initADC() {
	// 1. Declare necessary variables
	FILE *adc;
	char adc_path[50];
	// 2. Construct file path for the adc
	sprintf(adc_path, "%s/%s%i/slots", DEVICES, "bone_capemgr.", MGRNUM);
	// 3. Open the adc for writing
	adc = fopen(adc_path, "w");
	if(!adc) {
		fprintf(stderr, "Failed to initialize ADC\n");
		return false;
	}
	// 4. Configure the ADC
  fprintf(adc,"cape-bone-iio");
	fflush(adc);
	// 5. Clean up
	fclose(adc);
	return true;
}

int readADC(char *ach) {
  // 1. Declare necessary variables
  FILE *adc_val;
  int value;
  char adc_val_path[50];
  // 2. Construct file path to read from ach
  sprintf(adc_val_path, "%s/%s%i/%s", DEVICES, "ocp.3/helper.", HNUM, ach);
  // 3. Open the ach for reading
  adc_val = fopen(adc_val_path, "r");
  if(!adc_val) {
    fprintf(stderr, "Failed to read from %s\n", ach);
    return -1;
  }
  // 4. Read in the value from ach
  fscanf(adc_val, "%d", &value);
  fflush(adc_val);
  // 5. Clean up
  fclose(adc_val);
  return value;

}
