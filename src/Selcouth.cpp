
#include "Arduino.h"
#include "Selcouth.h"
#include <Wire.h>

int data_array[20] = {0};
extern char *char_array;
extern void Ready_To_Send(void);
extern float temperature;
#define SAMPLE 1
#define SEND 0

int ecg_ADC = 0;

void HandShake_Config(void)
{
  pinMode(INTERRUPT,OUTPUT);
  digitalWrite(INTERRUPT,HIGH);
}

void Ready_To_Send(void)
{
 digitalWrite(INTERRUPT,LOW);
 delay(1000);
 digitalWrite(INTERRUPT,HIGH);
}

void Temperature_Sensor::Pins_Initializations()
{
  pinMode(Gun_Power_Pin,OUTPUT);
  pinMode(Gun_Eeprom_Power_Pin,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(Gun_Trig_Pin,OUTPUT);
  digitalWrite(Gun_Eeprom_Power_Pin,LOW);
  digitalWrite(Gun_Power_Pin,LOW);
  digitalWrite(Gun_Trig_Pin,LOW);

  //Turn SDA SCL lines LOW at first, so that Display goes off
  pinMode(20,OUTPUT);
  pinMode(21,OUTPUT);
  digitalWrite(20,LOW);
  digitalWrite(21,LOW);
}

void Temperature_Sensor::Turn_On_Gun()
{
  pinMode(Gun_Power_Pin,OUTPUT);
  digitalWrite(Gun_Power_Pin,HIGH);
}

void Temperature_Sensor::Turn_Off_Gun()
{
  pinMode(Gun_Power_Pin,OUTPUT);
  digitalWrite(Gun_Power_Pin,LOW);
}

void Temperature_Sensor::Turn_On_Eeprom()       //Check Later if Eeprom OFF requires high Z state or not!!!!!!!!!!!!!!!!!
{
  digitalWrite(Gun_Eeprom_Power_Pin,HIGH);
}

void Temperature_Sensor::Turn_Off_Eeprom()
{
  digitalWrite(Gun_Eeprom_Power_Pin,LOW);
}

void Temperature_Sensor::Eeprom_Erase(int deviceaddress, unsigned int eeaddress, byte data)
{
  for(int i=0; i<10; i++)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
    eeaddress = i ;
   Serial.print(i);
    Serial.write("Erasing");
  }
}

void Temperature_Sensor::Get_Data()
{
  int x = 0;
  uint8_t rdata = 0xFF;
  Serial.println("Receiving from Eeprom \n");
    for(int i=79; i<83; i++)
    {
      x= i-79;
      Wire.beginTransmission(disk1);
      Wire.write((int)(i)); // LSB
      Wire.endTransmission();
      Wire.requestFrom(disk1,1 );
      rdata  =Wire.read(); 
      delay(10);
      //Serial.print(rdata);
     data_array[x] = rdata;
    } 
   temperature = ((data_array[2]<<8)|(data_array[1]))/100.0;
   Serial.print("Data Taken  ");
   Serial.print(temperature);
   if(((int)temperature>42) || ((int)temperature <34))
    {
      delay(1000);
      Serial.print("Retaking Data!");
      Get_Data();
    }
}

void BP_Meter::Pins_Initializations()
{
  pinMode(BP_Power_Pin,OUTPUT);
  pinMode(BP_Measure_Pin,OUTPUT);
  digitalWrite(BP_Measure_Pin,HIGH); //Try INPUT_PULLUP also here!!!!!!!!!!!!!!!!!!!!!!
  digitalWrite(BP_Power_Pin,LOW);
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
   delay(1000);
   digitalWrite(BP_Measure_Pin,HIGH);
}

void BP_Meter::Eeprom_Erase(int deviceaddress, unsigned int eeaddress, byte data ) 
{
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
    Serial.write("Written 1");
    delay(10);
}

void BP_Meter::Get_Data()
{
  int x;
  uint8_t rdata = 0xFF;
  Serial.println("Receiving from Eeprom \n");
    for(int i=0; i<15; i++)
    {
      Wire.beginTransmission(disk1);
      Wire.write((int)(i)); // LSB
      Wire.endTransmission();
     
      Wire.requestFrom(disk1,1 );
      Serial.print(i);
      Serial.print('\t');
      rdata  =Wire.read();
     Serial.print(rdata,DEC);
      Serial.print('\n');  
      // x = i-8;
      // data_array[x] = rdata;
      data_array[i]=rdata;
      delay(10);
    }

    Systolic_Pressure = data_array[8];
    Diastolic_Pressure = data_array[9];
    Heart_Beat = data_array[10];

   delay(1000);
   sprintf(char_array,"%ds%dd%dh",Systolic_Pressure,Diastolic_Pressure,Heart_Beat);
   Serial.write(char_array);

   Serial1.write(char_array);
   Ready_To_Send();
   Serial1.write(char_array);
}

int Height_Measurement::Get_Height(int unit)
{
  return 0;
  
}

void BIA::Activate(void)
{
  pinMode(BIA_Trigger_Pin,OUTPUT);
  digitalWrite(BIA_Trigger_Pin,HIGH);
  delay(500);
  digitalWrite(BIA_Trigger_Pin,LOW);
}

void BIA::Pins_Initializations(void)
{

}

void ECG::Pins_Initializations(void)
{
  pinMode(37, INPUT); // Setup for leads off detection LO +
  pinMode(39, INPUT); // Setup for leads off detection LO -
}

void ECG::Send_Data(int sample)
{
    if((digitalRead(37) == 1)||(digitalRead(39) == 1)){
      Serial.println('!');
    }
    else
    {
      //ecg_ADC = analogRead(A8);
  
      // send the value of analog input 8 to Raspberry Pi
        Serial3.print(analogRead(A8));
        Serial3.print('o');

      // send the value of analog input 8 to serial monitor
     Serial.println(analogRead(A8));
    }
    //Wait for a bit to keep serial data from saturating
    delay(1);
}