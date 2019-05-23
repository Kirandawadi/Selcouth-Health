

#ifndef SELCOUTH_H_
#define SELCOUTH_H_

#include "Arduino.h"


#define BUSY_STATE 1
#define FREE_STATE 0

class States_Manager
{
  public:
  void Assign_State(int state);
};

class Temperature_Sensor
{
  public:
    Temperature_Sensor(int Chip_Power,int Eeprom_Power):
  	Chip_Power_Pin(Chip_Power) ,Eeprom_Power_Pin(Eeprom_Power){}
    void Turn_On_Gun(int sec);
    void Turn_Off_Gun();
    void Turn_On_Eeprom(int sec);
    void Turn_Off_Eeprom();
    void Eeprom_Erase();
    void Get_Data();
  private:
    int Chip_Power_Pin,Eeprom_Power_Pin;
};

class BP_Meter
{
  public:
  BP_Meter(int _Power_Pin,int _Measure_Pin):
  Power_Pin(_Power_Pin) , Measure_Pin(_Measure_Pin){}
  void Turn_On();
  void Turn_Off();
  void Start_Measuring(int sec);
  void Eeprom_Erase();
  void Get_Data();
  private:
  int Power_Pin,Measure_Pin;
};

class ECG
{
  public:
  void Start_Converison();

  private:
  
};
#endif