

#ifndef SELCOUTH_H_
#define SELCOUTH_H_

#include "Arduino.h"
#include "pin_defines.h"

#define BUSY_STATE 1
#define FREE_STATE 0

void HandShake_Config(void);
void Ready_To_Send(void);

class States_Manager
{
  public:
  void Assign_State(int state);
};

class Temperature_Sensor
{
  public:
    Temperature_Sensor(int Chip_Power,int Eeprom_Power,int Trigger):
  	Chip_Power_Pin(Chip_Power) ,Eeprom_Power_Pin(Eeprom_Power),Gun_Trigger_Pin(Trigger){}
    void Pins_Initializations();
    void Turn_On_Gun();
    void Turn_Off_Gun();
    void Turn_On_Eeprom();
    void Turn_Off_Eeprom();
    void Eeprom_Erase(int deviceaddress, unsigned int eeaddress, byte data);
    void Get_Data();
  private:
    int Chip_Power_Pin,Eeprom_Power_Pin,Gun_Trigger_Pin;
};

class BP_Meter
{
  public:
  BP_Meter(int _Power_Pin,int _Measure_Pin):
  Power_Pin(_Power_Pin) , Measure_Pin(_Measure_Pin){}
  void Pins_Initializations();
  void Turn_On();
  void Turn_Off();
  void Start_Measuring();
  void Eeprom_Erase(int deviceaddress, unsigned int eeaddress, byte data );
  void Get_Data();
  int Systolic_Pressure, Diastolic_Pressure, Heart_Beat;
  private:
  int Power_Pin,Measure_Pin;
};

class Height_Measurement
{
  public:
  int Get_Height(int unit);
  private:
  int height_cms;
};

class BIA
{
  public:
  void Activate(void);
  void Pins_Initializations(void);

};

class ECG
{
  private:
  int L0_plus = 37, L0_minus = 39;
  int channel = PIN_A1;
  int relay = PIN_A15;
  public:
  void Send_Data(int SAMPLE);
  void Pins_Initializations(void);
  void Break_Connection(void);
  void Make_Connection(void);
};

void Emergency_Button_Initialization();
void Pressed(void); 
void my_delay(int msec);

void Begin_Sending(void);
void End_Sending(void);



#endif