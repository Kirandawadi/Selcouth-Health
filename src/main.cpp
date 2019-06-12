#include <Arduino.h>
#include <SlowSoftI2CMaster.h>
#include <Wire.h>    
#include "Selcouth.h"
#define disk1 0x50    //Address of 24LC256 eeprom chip 
char data = 0;
unsigned int address = 0;
extern int data_array[];
float temperature = 0;
char char_array[100];
int to_send = 0;

int counter= 0,checker = 0;
int Temp_Measuring_State = 0;
 int BP_Measuring_State = 0;
 int Systolic_Pressure = 120, Diastolic_Pressure = 80, Heart_Beat = 75;

void readEEPROM(int deviceaddress, unsigned int eeaddress );
void eraseEEPROM(int deviceaddress, unsigned int eeaddress, byte data );
void Read_hardware_I2C();
void Erase_hardware_I2C();

 //**************Creating Objects***********
Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);
SlowSoftI2CMaster si = SlowSoftI2CMaster(24, 26, true);
SlowSoftI2CMaster gun_I2C = SlowSoftI2CMaster(20, 21, true);

//*******************Gun Specials********************

void setup(void)
{
  HandShake_Config();
  Serial.begin(9600);
  Serial3.begin(9600);
  //Wire.begin(disk1);  

  temp.Pins_Initializations();
  bp.Pins_Initializations();
  bp.Turn_Off();
  temp.Turn_Off_Gun();
  //delay(3000);
  Serial.print("Resetted");

}
 
void loop(){ 

 if(Serial.available())
  {
    data = char(Serial.read());
    if(data == '0')
    {
      delay(2000);
      Ready_To_Send();
      Serial3.print("173h");
      Ready_To_Send();
    }

    else if(data == '2')
   {
  //    Serial.write(char_array);

  //  Ready_To_Send();
  //  Serial3.write(char_array);
  //  Ready_To_Send();
      Serial.write("Entered");
      bp.Turn_On();
      delay(8000);
      Serial.print("Measuring ON");
      bp.Start_Measuring();
      BP_Measuring_State  =1;
   }

    else if(data == '1')
    {
      temp.Turn_On_Eeprom();
      Serial.print("Perform Relay Action;Join the Power Pin");
      delay(2000);
      temp.Turn_On_Gun();
      Serial.print("Joined");
      Temp_Measuring_State = 1;
    } 

    else if (data == 'r')
    {
      Serial.print("R in");
      temp.Turn_On_Eeprom();
      delay(2000);
      readEEPROM(disk1, address );
      temperature = ((data_array[2]<<8)|(data_array[1]))/100.0;
      Serial.print("The temperature is ");
      Serial.print(temperature);
      to_send = int(temperature*100);
      Ready_To_Send();
      Serial3.print(to_send);
      Serial3.print("t");
      Ready_To_Send();
      eraseEEPROM(disk1, address,0);
        }
  }

if(Temp_Measuring_State == 1)
{
  Serial.print("Waiting for triggering");
  pinMode(Gun_Trig_Pin,INPUT);
  digitalWrite(Gun_Trig_Pin,INPUT_PULLUP);
  delay(100);

  while(digitalRead(Gun_Trig_Pin) !=0)
  {
    Serial.print("I am HIGH\n");
    delay(100);
  }
  Serial.print("I am pressed\n");

  Serial.print("Now its time to turn off relay");
  delay(6000);
  temp.Turn_Off_Gun();
  delay(1000);
  if (!gun_I2C.i2c_init()) // initialize I2C module
  Serial.println("I2C init failed");

  if (!gun_I2C.i2c_start((disk1<<1)|I2C_WRITE)) { // init transfer
        Serial.println("I2C device busy");
        return;
    }

    for(int i=80;i<95;i++)
    {
    gun_I2C.i2c_write(i); // send memory to device
    gun_I2C.i2c_rep_start((disk1<<1)|I2C_READ); // restart for reading
    data_array[i] = gun_I2C.i2c_read(true); // read one byte and send NAK afterwards
    gun_I2C.i2c_stop(); // stop communication
    Serial.print(i);
    Serial.print("  ");
    Serial.println(data_array[i],HEX);
    delay(10);
    }
    temperature = ((data_array[93]<<8)|(data_array[92]))/100.0;
    Serial.print("The temperature is ");
    Serial.print(temperature);

      if (!gun_I2C.i2c_start((disk1<<1)|I2C_WRITE)) { // init transfer
        Serial.println("I2C device busy");
        return;
    }

for(int i = 0;i<11;i++)
{
    gun_I2C.i2c_write(i); // send memory to device
    gun_I2C.i2c_write(0);
    gun_I2C.i2c_stop(); // stop communication
    Serial.write("Erased");
}
  // Ready_To_Send();1
  // Serial3.print("y");
  // Ready_To_Send();
  
  Temp_Measuring_State = 0;
  }

  else if(BP_Measuring_State == 1)
{
  pinMode(26,INPUT);
  digitalWrite(26,INPUT_PULLUP);
  delay(6000);
  
    while(digitalRead(26) !=0)
  {
    Serial.print("High\n");
    //delay(100);
  }

   Serial.print("Out of LOOP\n");
  delay(2000);
  if (!si.i2c_init()) // initialize I2C module
  Serial.println("I2C init failed");

  Read_hardware_I2C();
  BP_Measuring_State = 0;
  Erase_hardware_I2C();
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
      rdata = Wire.read();
      delay(10);
      data_array[x] = rdata;
    }
}

void Read_hardware_I2C()
{
  Serial.println("Receiving from Eeprom \n");
  if (!si.i2c_start((disk1<<1)|I2C_WRITE)) { // init transfer
        Serial.println("I2C device busy");
        return;
    }

    for(int i=0;i<15;i++)
    {
    si.i2c_write(i); // send memory to device
    si.i2c_rep_start((disk1<<1)|I2C_READ); // restart for reading
    data_array[i] = si.i2c_read(true); // read one byte and send NAK afterwards
    si.i2c_stop(); // stop communication
    Serial.print(i);
    Serial.print("  ");
    Serial.println(data_array[i]);
    }

    if(data_array[0]>20)
    {
    Systolic_Pressure = data_array[0];
    Diastolic_Pressure = data_array[1];
    Heart_Beat = data_array[2];
    }
    else
    {
    Systolic_Pressure = data_array[8];
    Diastolic_Pressure = data_array[9];
    Heart_Beat = data_array[10]; 
}
    
   delay(1000);
   sprintf(char_array,"%ds%dd%dh",Systolic_Pressure,Diastolic_Pressure,Heart_Beat);
   Serial.write(char_array);

   Ready_To_Send();
   Serial3.write(char_array);
   Ready_To_Send();
}

void Erase_hardware_I2C()
{
 if (!si.i2c_start((disk1<<1)|I2C_WRITE)) { // init transfer
        Serial.println("I2C device busy");
        return;
    }
    si.i2c_write(0); // send memory to device
    si.i2c_write(1);
    si.i2c_stop(); // stop communication
    Serial.write("Written 1");
}