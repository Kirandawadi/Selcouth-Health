#include <Arduino.h>
#include <SlowSoftI2CMaster.h>
#include <Wire.h>    
#include "Selcouth.h"
#include "tfMini.h"

#define disk1 0x50    //Address of 24LC256 eeprom chip 
#define SAMPLE 1
#define SEND 0

char data = 0;
unsigned int address = 0;
extern int data_array[];
float temperature = 0;
char char_array[100];
int to_send = 0;
float height = 0;
extern int ecg_data[];

int counter= 0,checker = 0;
int Temp_Measuring_State = 0;
int BP_Measuring_State = 0;
int Height_Measuring_State = 0;
int Systolic_Pressure = 120, Diastolic_Pressure = 80, Heart_Beat = 75; 

void readEEPROM(int deviceaddress, unsigned int eeaddress );
void eraseEEPROM(int deviceaddress, unsigned int eeaddress, byte data );
void Read_software_I2C();
void Erase_software_I2C();

//**************Creating Objects***********
Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);
BIA bia;
ECG ecg;
SlowSoftI2CMaster si = SlowSoftI2CMaster(24, 26, true);
SlowSoftI2CMaster gun_I2C = SlowSoftI2CMaster(20, 21, true);

void Random_Send(void)
{
   delay(500);
   for(int i=0;i<5;i++)
      {
      Serial3.print("a");
      delay(1);
      }
}

void setup(void)
{
  HandShake_Config();
  Serial.begin(9600);           //For Arduino
  Serial1.begin(115200);    //For LIDAR
  Serial2.begin(9600);       //For Bluetooth
  Serial3.begin(115200);        //From Raspberry Pi
  //Turning off I2C at first
  //Wire.begin(disk1);                  !!!!!!!!!!!!TWEAKING THIS THING 
  temp.Pins_Initializations();
  bp.Pins_Initializations();
  ecg.Pins_Initializations();
  tfMini_Initialize();
  bia.Pins_Initializations();
  bp.Turn_Off();
  
  temp.Turn_Off_Gun();
  temp.Turn_Off_Eeprom(); 

  Serial.print("Resetted");
}
 
void loop(){ 

 /* ecg.Break_Connection();
  ecg.Send_Data(2000);
  ecg.Make_Connection();
  ecg.Send_Data(2000);*/
  
 if(Serial3.available())
  {
    data = char(Serial3.read());
    if(data == '0')
    {
      Height_Measuring_State = 1;
      Serial.print("AAYO");
      height = get_Height();
      Ready_To_Send(); 
      Random_Send();
      Serial3.print(int(height));
      Serial3.print("h");
      //Ready_To_Send(); 
      Serial.print("\r\n The height is: ");
      Serial.print(height);
    }

    else if(data == '1')
    {
      temp.Turn_On_Eeprom();
      Serial.print("Perform Relay Action;Join the Power Pin");
      Serial2.print("Perform Relay Action;Join the Power Pin");
      delay(2000); 
      temp.Turn_On_Gun();
      Serial.print("Joined");
      Serial2.print("Joined");
      Temp_Measuring_State = 1;
    } 

     else if (data == '2')
  {
    Serial.print("Activated");
      delay(2000);
      bia.Activate();
  }

    else if(data == '3')
   {
      Serial.write("Entered");
      Serial2.write("Entered");
      bp.Turn_On();
      delay(2000);
      Serial.print("Measuring ON");
      Serial2.print("Measuring ON");
      bp.Start_Measuring();
      BP_Measuring_State  =1;
   }
   
    else if(data == '4')
    {
      ecg.Break_Connection();
      ecg.Send_Data(1000);
      ecg.Make_Connection();
    }

    else if (data == 'r')
    {
      temp.Turn_On_Eeprom();
      Wire.begin(disk1);
      delay(200);
      Serial.print("Reading EEprom");
      readEEPROM(disk1, address );
      /*****************Extras ******************/
      Serial.println(data_array[0]);
      Serial.println(data_array[1]);
      Serial.println(data_array[2]);
      /*****************Extras ******************/
      temperature = ((data_array[2]<<8)|(data_array[1]))/100.0;
      //Serial.print("The temperature is ");
      Serial.print(temperature);
      Serial2.print(temperature);
      to_send = int(temperature*100);
      Ready_To_Send(); 
      Random_Send();
      Serial3.print(to_send);
      Serial3.print("t");

      eraseEEPROM(disk1, address,0);
    }
  }

if(Temp_Measuring_State == 1)
{
  pinMode(20,INPUT);
  digitalWrite(20,INPUT_PULLUP);
  pinMode(21,INPUT);
  digitalWrite(21,INPUT_PULLUP);

  Serial.print("Waiting for triggering");
  pinMode(Gun_Trig_Pin,INPUT);
  digitalWrite(Gun_Trig_Pin,INPUT_PULLUP);
  delay(100);

  while(digitalRead(Gun_Trig_Pin) !=0)
  {
    Serial.print("I am HIGH\n");
    Serial2.print("I am HIGH\n");
    delay(100);
  }
  Serial.print("I am pressed\n");
  Serial2.print("I am pressed\n");

  Serial.print("Now its time to turn off relay");
  delay(7000);
  temp.Turn_Off_Gun();

  Ready_To_Send();
  Random_Send();
  Serial3.print("y");
 
  Serial.print("y");
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
    Serial2.print("High\n");
    //delay(100);
  }
  //delay(5000);
   Serial.print("Out of LOOP\n");
  delay(3000);
  if (!si.i2c_init()) // initialize I2C module1
  Serial.println("I2C init failed");

  Read_software_I2C();
  BP_Measuring_State = 0;
  Erase_software_I2C();
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
    // Serial.print(i);
    // Serial.write("E");
  }
    Serial.write("E");
}
 
void readEEPROM(int deviceaddress, unsigned int eeaddress ) 
{
  byte rdata = 0xFF;
  int x=0;
  
  Serial.print("R\n");
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

void Read_software_I2C()
{
  Serial.println("R\n");
  delay(300);
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
   Random_Send();
   Serial3.write(char_array);
}

void Erase_software_I2C()
{
 if (!si.i2c_start((disk1<<1)|I2C_WRITE)) { // init transfer
        Serial.println("I2C device busy");
        return;
    }
    si.i2c_write(0); // send memory to device
    si.i2c_write(1);
    si.i2c_stop(); // stop communication
    Serial.write("Written 1");
    Serial2.write("Written 1");
}