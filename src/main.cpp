#include <Arduino.h>
#include <Selcouth.h>

unsigned int address = 0;
int i=0;

/**************Creating Objects************/
Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);

void setup()
{

 Serial.begin(9600);
 bp.Pins_Initializations();
 digitalWrite(BP_Measure_Pin,HIGH);
  bp.Turn_Off();

//This BP Turning On is realized manually by plugging the Power pin
// bp.Turn_On();
 
 // bp.Get_Data();
}

void loop()
{
  if(Serial.available())
  {
   char data  =char(Serial.read());
   if(data == 'a')
   {
      bp.Start_Measuring();
   }
}
}