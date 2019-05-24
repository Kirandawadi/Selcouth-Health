
#include "Arduino.h"
#include "Selcouth.h"
#include <Wire.h>

uint8_t data_array[3] = {0};

void States_Manager::Assign_State(int state)
{
  //Serial_Event
  //Interrupt_High
}

int Temperature_Sensor::Turn_On_Gun(int sec)
{
  return 0;
}

void Temperature_Sensor::Turn_Off_Gun()
{
}

void Temperature_Sensor::Turn_On_Eeprom(int sec)
{
}

void Temperature_Sensor::Turn_Off_Eeprom()
{
}

void Temperature_Sensor::Eeprom_Erase()
{
}

void Temperature_Sensor::Get_Data()
{
}

void BP_Meter::Pins_Initializations()
{
  pinMode(BP_Power_Pin,OUTPUT);
  pinMode(BP_Measure_Pin,OUTPUT);
}
void BP_Meter::Turn_On()
{
digitalWrite(BP_Power_Pin,HIGH);
}

void BP_Meter::Turn_Off()
{
 digitalWrite(BP_Power_Pin,LOW);
}

void BP_Meter::Start_Measuring()
{
   digitalWrite(BP_Measure_Pin,LOW);
   delay(200);
   digitalWrite(BP_Measure_Pin,HIGH);
}

void BP_Meter::Eeprom_Erase(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  for(int i=0; i<100; i++)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
    eeaddress = i ;
    delay(10);
  }
}

void BP_Meter::Get_Data()
{
  uint8_t rdata = 0xFF;
  Serial.println("Receiving from Eeprom \n");
    for(int i=0; i<3; i++)
    {
      Wire.beginTransmission(disk1);
      Wire.write((int)(i)); // LSB
      Wire.endTransmission();
     
      Wire.requestFrom(disk1,1 );
      Serial.print(i);
      Serial.print('\t');
      rdata  =Wire.read();
      Serial.print(rdata,HEX);
      Serial.print('\n');  

      data_array[i] = rdata;
      delay(10);
    }

    Systolic_Pressure = data_array[0];
    Diastolic_Pressure = data_array[1];
    Heart_Beat = data_array[2];


}

void ECG::Start_Converison()
{

}