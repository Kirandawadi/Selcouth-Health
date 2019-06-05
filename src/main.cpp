#include <Arduino.h>
#include <Wire.h>    
#include "Selcouth.h"
#define disk1 0x50    //Address of 24LC256 eeprom chip 

char data = 0;
unsigned int address = 0;
extern int data_array[];
float temperature = 0;
extern char *char_array;
int counter= 0,checker = 0;
int Temp_Measuring_State = 0;
int BP_Measuring_State = 0;
int initial_Systolic=0,initial_Diastolic=0,initial_HeartBeat=0;

void readEEPROM(int deviceaddress, unsigned int eeaddress );
void eraseEEPROM(int deviceaddress, unsigned int eeaddress, byte data );

 //*************Creating Objects************
Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);

//*******************Gun Specials*********************

void setup(void)
{
  Temp_Measuring_State = 0;
  BP_Measuring_State = 0;
  HandShake_Config();
   Serial.begin(9600);
  // Serial1.begin(9600);
  Wire.begin(disk1);  
  temp.Pins_Initializations();
  digitalWrite(Gun_Eeprom_Power_Pin,HIGH);
  // delay(2000);
  //  temp.Eeprom_Erase(disk1,address,0);
  //   Serial.print("Entered");
         
  //         delay(3000);
  //         temp.Get_Data();
   bp.Pins_Initializations();
  bp.Turn_On();
  //Take Initial Readings before measurement*********************************
  //delay(5000);
  //bp.Eeprom_Erase(disk1,address,1);
  // bp.Get_Data();
  // bp.Eeprom_Erase(disk1,address,1);
  //initial_Systolic = bp.Systolic_Pressure,initial_Diastolic = bp.Diastolic_Pressure,initial_HeartBeat = bp.Heart_Beat;
//Serial.print(temperature);

// temp.Get_Data();
}
 
void loop(){

  // if(digitalRead(Gun_Trig_Pin)==HIGH)
  // {
  //   Serial.print(1);
  // }

    if(Serial.available())
      {
    data = char(Serial.read());
    if(data == 'a')
    {
        bp.Get_Data();

    }
     if(data == 'b')
    {
        temp.Turn_Off_Gun();
    }
      if(data == 'c')
    {
       Serial.print("Entered");
         
          delay(3000);
          temp.Get_Data();
    }
    if(data == '1')
   {
    //  Serial.print("Entered");
    //  Serial1.write("37800t");
    //  Ready_To_Send();
    //  Serial1.write("37800t");
      //This BP Turning On is realized manually by plugging the Power pin
      Serial.write("Plug the Power Pin:Relay Action");
       bp.Turn_On();
      delay(10000);
      Serial.write("Pulling LOW measuring Pin");
      bp.Start_Measuring();
      BP_Measuring_State  = 1;
      Temp_Measuring_State = 0;
   }
    else if(data == '2')
    {
      temp.Turn_On_Eeprom();
      Serial.print("Perform Relay Action;Join the Power Pin");
      temp.Turn_On_Gun();
      delay(3000);
      Serial.print("Joined");
      Temp_Measuring_State = 1;
    }
  }

  if(BP_Measuring_State == 1)
  {
    delay(60000);
    bp.Get_Data();
    // if(bp.Systolic_Pressure == initial_Systolic)
    // {
    //   delay(8000);
    //   bp.Get_Data();
    // }
    bp.Eeprom_Erase(disk1,address,1);
    bp.Turn_Off();
    BP_Measuring_State = 0;
  }

  else if(Temp_Measuring_State == 1)
  {
    while(digitalRead(Gun_Trig_Pin) !=0)
    {
      Serial.print("I am HIGH\n");
      delay(25);
    }
    delay(1000);
    Serial.print("I am pressed\n");
    
    Serial.print("Now its time to turn off relay");
    delay(3000);

    temp.Get_Data();
    temperature*=100;
    Serial.print("The temperature is ");
    Serial.print(temperature);
    Ready_To_Send();
    Serial1.print(temperature);
    temp.Eeprom_Erase(disk1,address,0);
    Temp_Measuring_State = 0;
    }
    }