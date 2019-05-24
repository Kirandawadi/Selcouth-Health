#include <Arduino.h>
#include <Selcouth.h>
#include <Wire.h>

unsigned int address = 0;
int i=0;
int Measuring_State = 0;

/**************Creating Objects************/
Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);

void setup()
{
 Wire.begin();
 Serial.begin(9600);
 bp.Pins_Initializations();
 bp.Turn_Off();
//This BP Turning On is realized manually by plugging the Power pin
// bp.Turn_On();
}

void loop()
{
  if(Serial.available())
  {
   char data  =char(Serial.read());
   if(data == '1')
   {
      bp.Start_Measuring();
      Measuring_State  =1;
   }
}

if(Measuring_State == 1)
{
  delay(50000);
  bp.Get_Data();
  Measuring_State = 0;
  bp.Eeprom_Erase(disk1,address,1);
}
}