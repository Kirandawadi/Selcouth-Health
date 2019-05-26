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

void readEEPROM(int deviceaddress, unsigned int eeaddress );
void eraseEEPROM(int deviceaddress, unsigned int eeaddress, byte data );

Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);

void setup(void)
{
  Wire.begin(disk1);  
  Serial.begin(9600);

  temp.Pins_Initializations();
  //temp.Turn_On_Eeprom();
  //temp.Turn_On_Gun();
}
 
void loop(){
  //  Serial.print(digitalRead(Gun_Trig_Pin));
  //  delay(500);
 if(Serial.available())
  {
    data = char(Serial.read());
    if(data == '2')
    {
      temp.Turn_On_Eeprom();
      Serial.print("Perform Relay Action;Join the Power Pin");
      delay(8000);
      Serial.print("Joined");
      Temp_Measuring_State = 1;
    }
  }

if(Temp_Measuring_State == 1)
{
  while(digitalRead(Gun_Trig_Pin) !=0)
  {
    Serial.print("I am HIGH\n");
    delay(25);
  }
  delay(1000);
  Serial.print("I am pressed\n");

  Serial.print("Now its time to turn off relay");
  delay(10000);
  readEEPROM(disk1, address );
  temperature = ((data_array[2]<<8)|(data_array[1]))/100.0;
  Serial.print("The temperature is ");
  Serial.print(temperature);
  eraseEEPROM(disk1, address,0);
   /* switch(data)
    {
      case 'e':
            eraseEEPROM(disk1, address,0);
            Serial.write("Erased");
            break;

      case 'r':
            readEEPROM(disk1, address );
            temperature = ((data_array[2]<<8)|(data_array[1]))/100.0;
            Serial.print(temperature);
            eraseEEPROM(disk1, address,0);
            break;
            
      case '0':
             temp.Turn_Off_Gun();
             Serial.write("I am low");
             break;

      case '1':
       temp.Turn_On_Gun();
       Serial.write("I am high");
       break;

      default:
      break;
    }*/
  }
  }
 
void eraseEEPROM(int deviceaddress, unsigned int eeaddress, byte data ) 
{
  for(int i=0; i<10; i++)
  {
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(data);
    Wire.endTransmission();
    eeaddress = i ;
   // delay(1);
   Serial.print(i);
    Serial.write("Erasing");
  }
}
 
void readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
  int x=0;
  
  Serial.print("Reading from Eeprom\n");
    for(int i=79; i<83; i++)
    {
      x= i-79;
      Wire.beginTransmission(deviceaddress);
      Wire.write((int)(i)); // LSB
      Wire.endTransmission();
     
      Wire.requestFrom(deviceaddress,1 );
//      Serial.print(i);
//      Serial.print('\t');
      rdata = Wire.read();
      //Serial.print(Wire.read(),HEX);
      // Serial.print(rdata,HEX);
      // Serial.print('\n');  
      delay(10);
      data_array[x] = rdata;
    }
   // Serial.print("I am out");
}
// #include <Arduino.h>
// #include <Selcouth.h>
// #include <Wire.h>

// unsigned int address = 0;
// int i=0;
// int Temp_Measuring_State = 0;
// int BP_Measuring_State = 0;

// /**************Creating Objects************/
// Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
// BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);

// void setup()
// {
//   Wire.begin(disk1);
//   Serial.begin(9600);
//   temp.Pins_Initializations();
//   temp.Turn_On_Eeprom();
//   delay(1000);
//   // temp.Get_Data();
//   // delay(1000);
//   //temp.Eeprom_Erase(disk1,address,3);
//   //
// }

// void loop()
// {
//   // Wire.requestFrom(disk1,6);
//   //     while(Wire.available()){
//   //       char c= Wire.read();
//   //       Serial.print(c);
//   //     }
//   //     delay(500);
//   //Serial.print(digitalRead(Gun_Trig_Pin));
//   if(Serial.available())
//   {
//     char data1 = char(Serial.read());
//     if( data1 == 'r')
//     {
//       temp.Turn_On_Gun();
//       delay(500);
//       Temp_Measuring_State = 1;
//     }
//   }

//   if(Temp_Measuring_State == 1)
//   {
//     digitalWrite(LED_BUILTIN,HIGH);
//     //wait until the trigger is pulled LOW
//     for(int i=0;i<40;i++)
//     {
//       Serial.print(digitalRead(Gun_Trig_Pin));
//     }

//     while(digitalRead(Gun_Trig_Pin) ==1);
//     //Wait again for it to be high
//     delay(10000);
//     pinMode(Gun_Power_Pin,INPUT);
//   //  temp.Turn_Off_Gun();
//     delay(1000);
//     temp.Get_Data();
//     Serial.print("Data got");
//     delay(2000);
//     // temp.Eeprom_Erase(disk1,address,0);
//     // Serial.print("Erased");
//     Temp_Measuring_State = 0;
//   }
// }

/********************************WORKING CODE************************/
// /*
// void setup()
// {
//  Wire.begin();
//  Serial.begin(9600);
//  bp.Pins_Initializations();
//  bp.Turn_Off();
// }

// void loop()
// {
//   if(Serial.available())
//   {
//    char data  =char(Serial.read());
//    if(data == '1')
//    {
  // //This BP Turning On is realized manually by plugging the Power pin
// // bp.Turn_On();
//  delay(2000);
//       bp.Start_Measuring();
//       BP_Measuring_State  =1;
//    }
// }

// if(BP_Measuring_State == 1)
// {
//   delay(50000);
//   bp.Get_Data();
//   BP_Measuring_State = 0;
//   bp.Eeprom_Erase(disk1,address,1);
// }
// }
// */