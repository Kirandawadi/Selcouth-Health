// #ifndef PIN_DEFINES_H
// #define PIN_DEFINES_H

// #include "Arduino.h"

// /*************Old Pinouts Definition */
// #define Gun_Eeprom_Power_Pin 10
// #define Gun_Power_Pin 12
// #define Gun_Trig_Pin 11
// #define BP_Power_Pin 5
// #define BP_Measure_Pin 22
// #define BIA_Trigger_Pin 23

// #define INTERRUPT 3

// /*I2 - 2
//   I3 - 3
//  */

// #define disk1 0x50

// #endif

#ifndef PIN_DEFINES_H
#define PIN_DEFINES_H

#include "Arduino.h"

/*************New Pinouts Definition */
#define Gun_Eeprom_Power_Pin 23
#define Gun_Power_Pin 25
#define Gun_Trig_Pin 22
 #define BP_Power_Pin 6
 #define BP_Measure_Pin 5
 #define BIA_Trigger_Pin 7
//  #define BP_Power_Pin 56
//  #define BP_Measure_Pin 57
//  #define BIA_Trigger_Pin 58             //TESTSSSS

#define INTERRUPT 2

#define disk1 0x50

#endif