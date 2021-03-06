
#include "Arduino.h"
#include "Selcouth.h"
#include <Wire.h>

volatile char to_Send = '0';
const byte interruptPin = 56;

int data_array[20] = {0};
extern char char_array[100];
extern void Begin_Sending(void);
extern float temperature;
#define SAMPLE 1
#define SEND 0

int ecg_ADC = 0;

int ecg_data[1200]=
{324,
329,
330,
330,
335,
336,
333,
338,
339,
336,
343,
345,
343,
348,
351,
371,
372,
372,
373,
373,
374,
377,
379,
381,
382,
384,
385,
386,
387,
387,
387,
394,
397,
395,
398,
397,
391,
389,
385,
378,
372,
366,
357,
349,
346,
340,
331,
331,
327,
321,
323,
321,
318,
319,
319,
315,
316,
317,
316,
315,
318,
319,
316,
319,
319,
315,
319,
319,
317,
319,
320,
317,
317,
319,
318,
315,
318,
316,
313,
316,
316,
311,
314,
315,
311,
314,
315,
312,
312,
313,
313,
310,
314,
313,
309,
313,
312,
309,
313,
312,
310,
313,
313,
310,
310,
311,
309,
309,
311,
310,
307,
310,
310,
306,
309,
309,
306,
308,
309,
308,
313,
317,
315,
311,
314,
313,
309,
310,
308,
303,
305,
305,
301,
305,
305,
302,
303,
311,
335,
383,
427,
418,
316,
228,
198,
212,
251,
280,
297,
308,
311,
311,
314,
316,
317,
316,
320,
321,
320,
325,
327,
325,
331,
333,
332,
338,
342,
343,
347,
353,
355,
360,
367,
371,
372,
379,
382,
380,
385,
384,
378,
377,
372,
364,
359,
353,
345,
337,
334,
328,
320,
320,
315,
309,
311,
309,
304,
306,
305,
303,
304,
305,
304,
303,
306,
306,
303,
307,
307,
304,
308,
308,
305,
309,
309,
306,
307,
308,
306,
305,
307,
306,
303,
307,
307,
303,
306,
306,
303,
306,
305,
302,
304,
306,
304,
303,
306,
306,
303,
306,
306,
302,
306,
307,
304,
307,
307,
304,
306,
307,
305,
305,
307,
307,
304,
308,
308,
304,
308,
308,
304,
308,
308,
306,
308,
311,
314,
315,
317,
315,
312,
316,
314,
309,
309,
308,
304,
308,
308,
305,
307,
308,
313,
342,
393,
431,
422,
331,
239,
208,
235,
271,
294,
316,
321,
321,
325,
327,
327,
328,
331,
332,
331,
336,
338,
336,
341,
344,
343,
349,
352,
353,
358,
362,
365,
371,
378,
382,
384,
391,
395,
395,
401,
401,
397,
398,
394,
387,
385,
378,
370,
363,
357,
350,
342,
341,
336,
329,
330,
328,
323,
325,
325,
321,
323,
323,
322,
322,
324,
324,
322,
325,
325,
322,
325,
326,
323,
326,
325,
322,
324,
324,
322,
321,
322,
321,
319,
321,
320,
316,
320,
320,
317,
319,
320,
318,
319,
319,
317,
318,
320,
318,
316,
319,
318,
315,
319,
319,
316,
318,
317,
315,
316,
316,
315,
315,
317,
316,
315,
316,
317,
312,
316,
316,
312,
315,
316,
313,
316,
317,
315,
316,
319,
319,
322,
326,
325,
323,
326,
327,
322,
323,
320,
317,
319,
320,
319,
319,
321,
320,
326,
363,
413,
449,
440,
336,
241,
223,
250,
281,
308,
325,
331,
334,
337,
337,
338,
342,
343,
342,
347,
349,
348,
353,
355,
355,
359,
363,
364,
368,
374,
376,
380,
387,
392,
394,
402,
406,
406,
413,
414,
411,
412,
410,
405,
401,
397,
389,
381,
376,
368,
359,
357,
352,
344,
344,
341,
336,
336,
335,
332,
332,
333,
331,
330,
332,
332,
329,
333,
332,
328,
332,
332,
329,
332,
331,
329,
330,
330,
329,
327,
328,
328,
324,
327,
326,
323,
325,
324,
320,
322,
322,
319,
320,
319,
318,
318,
320,
319,
316,
318,
319,
315,
318,
318,
315,
318,
317,
315,
316,
317,
315,
315,
316,
317,
314,
316,
316,
312,
315,
315,
312,
315,
316,
315,
319,
321,
320,
320,
323,
321,
316,
318,
316,
309,
311,
311,
309,
311,
311,
309,
317,
341,
389,
435,
439,
341,
237,
201,
217,
249,
284,
305,
312,
320,
322,
321,
325,
327,
326,
328,
331,
332,
331,
336,
338,
337,
343,
346,
345,
351,
354,
355,
360,
365,
368,
374,
380,
384,
385,
390,
391,
388,
392,
389,
383,
381,
375,
365,
360,
353,
344,
337,
331,
325,
319,
319,
316,
310,
311,
311,
305,
307,
305,
302,
304,
305,
303,
303,
305,
304,
301,
304,
305,
301,
304,
304,
300,
303,
303,
300,
301,
302,
299,
299,
300,
299,
296,
299,
298,
294,
297,
297,
294,
296,
296,
294,
296,
296,
295,
294,
295,
295,
293,
295,
296,
292,
296,
296,
291,
295,
295,
293,
295,
295,
293,
294,
296,
294,
292,
295,
297,
293,
298,
300,
296,
301,
304,
302,
304,
304,
300,
298,
297,
296,
294,
298,
297,
294,
298,
306,
333,
388,
425,
406,
306,
221,
199,
224,
261,
287,
303,
312,
313,
311,
317,
318,
315,
321,
323,
321,
325,
327,
328,
331,
334,
336,
338,
343,
347,
347,
355,
359,
360,
368,
373,
374,
382,
386,
386,
389,
391,
390,
386,
386,
381,
372,
369,
363,
352,
350,
343,
335,
332,
328,
323,
321,
320,
318,
316,
318,
316,
313,
316,
316,
313,
317,
317,
315,
317,
318,
316,
318,
320,
319,
318,
320,
320,
316,
319,
319,
315,
319,
318,
315,
317,
317,
314,
315,
315,
314,
313,
316,
315,
312,
315,
316,
311,
316,
316,
312,
315,
315,
313,
314,
314,
313,
312,
315,
315,
313,
316,
317,
314,
317,
317,
314,
317,
317,
314,
316,
318,
316,
316,
318,
319,
317,
321,
322,
320,
325,
326,
324,
327,
328,
324,
324,
322,
320,
319,
320,
320,
318,
322,
321,
328,
369,
419,
449,
428,
318,
236,
226,
257,
289,
313,
329,
334,
333,
339,
341,
338,
343,
345,
343,
348,
350,
349,
353,
356,
357,
360,
365,
368,
368,
376,
381,
381,
389,
393,
394,
402,
407,
408,
413,
416,
414,
412,
412,
407,
399,
398,
391,
379,
375,
368,
358,
356,
351,
343,
341,
338,
333,
332,
332,
331,
327,
330,
330,
326,
330,
331,
327,
330,
330,
328,
330,
331,
329,
329,
330,
330,
326,
329,
329,
324,
327,
327,
322,
325,
325,
322,
323,
323,
320,
320,
321,
320,
317,
320,
320,
316,
320,
320,
315,
318,
319,
316,
317,
318,
316,
315,
316,
315,
312,
315,
315,
311,
315,
315,
311,
314,
314,
312,
315,
316,
315,
316,
318,
318,
316,
319,
320,
318,
322,
322,
316,
318,
316,
311,
313,
313,
311,
311,
313,
313,
321,
363,
418,
449,
419,
303,
218,
210,
240,
273,
302,
319,
324,
327,
331,
331,
331,
335,
336,
333,
339,
341,
338,
344,
346,
345,
350,
352,
353,
359,
364,
367,
370,
378,
383,
385,
393,
397,
397,
404,
406,
403,
404,
401,
395,
392,
387,
379,
369,
365,
359,
349,
346,
341,
333,
333,
331,
326,
327,
325,
322,
323,
324,
322,
320,
322,
322,
320,
324,
323,
320,
324,
325,
322,
324,
324,
321,
323,
324,
322,
320,
322,
322,
317,
320,
321,
315,
319,
318,
314,
316,
317,
314,
316,
316,
314,
313,
315,
315,
311,
315,
314,
309,
313,
312,
309,
312,
312,
310,
311,
312,
311,
310,
312,
312,
308,
312,
313,
309,
314,
315,
311,
315,
317,
316,
317,
315,
311,
308,
309,
308,
305,
309,
309,
305,
310,
321,
355,
413,
445,
397,
286,
213,
202,
228,
265,
292,
305,
316,
319,
316,
321,
323,
320,
324,
325,
324,
327,
329,
329,
330,
334,
337,
337,
344,
347,
346,
354,
359,
359,
367,
372,
374,
380,
384,
385,
386,
389,
387,
381,
381,
376,
366,
363,
355,
345,
341,
335,
327,
323,
320,
316,
313,
312,
311,
306,
310,
310,
305,
308,
309,
305,
308,
308};

void HandShake_Config(void)
{
  pinMode(INTERRUPT,OUTPUT);
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
   Begin_Sending();
   Serial1.write(char_array);
}

int Height_Measurement::Get_Height(int unit)
{
  return 0;
  
}

void BIA::Activate(void)
{
  delay(2500);
  pinMode(BIA_Trigger_Pin,OUTPUT);
  digitalWrite(BIA_Trigger_Pin,HIGH);
  delay(500);
  digitalWrite(BIA_Trigger_Pin,LOW);
}

void BIA::Pins_Initializations(void)
{
  pinMode(BIA_Trigger_Pin,OUTPUT);
  digitalWrite(BIA_Trigger_Pin,LOW);
}

void ECG::Pins_Initializations(void)
{
  pinMode(L0_plus, INPUT); // Setup for leads off detection LO +
  pinMode(L0_minus, INPUT); // Setup for leads off detection LO -
}

void ECG::Send_Data(int sample)
{
  // Serial3.end();
  // delay(200);
  // Serial3.begin(115200);
  
  Serial.println("Sending ECG data");
  Begin_Sending();
  for(int i = 0;i<1200;i++)
   {
  //   if((digitalRead(L0_plus) == 1)||(digitalRead(L0_minus) == 1)){
  //     Serial.println('!');
  //   }
  //   else
  //   {
      //ecg_ADC = analogRead(A8);
  
      // send the value of analog input 8 to Raspberry Pi
        // Serial3.print(analogRead(channel));
        // Serial3.print('o');

      // send the value of analog input 8 to serial monitor
     Serial3.print(ecg_data[i]);
     Serial3.print(",");

     Serial.println(ecg_data[i]);

     //Wait for a bit to keep serial data from saturating
     delay(1);
    }
    End_Sending();

  // Serial3.end();
  // delay(200);
  // Serial3.begin(9600);
}

void ECG::Break_Connection(void)
{
  pinMode(relay , OUTPUT);
  digitalWrite(relay,HIGH);
  delay(1000);
}

void ECG::Make_Connection(void)
{
  pinMode(relay , OUTPUT);
  digitalWrite(relay,LOW);
}


void Begin_Sending(void)
{
  //For callback On the other Side
   digitalWrite(INTERRUPT,LOW);
   delay(1000);
   digitalWrite(INTERRUPT,HIGH);

  //  delay(100);
  //  for(int i=0;i<5;i++)
  //     {
  //     Serial3.print("a");
  //     }
}

void End_Sending(void)
{
  Serial3.print("z");
}

void Emergency_Button_Initialization(void)
{
  pinMode(interruptPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), Pressed, LOW);
}

void Pressed() {

detachInterrupt(digitalPinToInterrupt(interruptPin));

to_Send = 'r';
my_delay(1300);
if(digitalRead(interruptPin) == LOW)
{
  to_Send = 's';
}

Serial.println(to_Send);
Begin_Sending();
Serial3.println(to_Send);

to_Send  = '0';
while((digitalRead(interruptPin) == LOW));
attachInterrupt(digitalPinToInterrupt(interruptPin), Pressed, LOW);
}

void my_delay(int msec)
{
    for(int i =0;i<msec;i++)
{
  delayMicroseconds(1000);
}
}