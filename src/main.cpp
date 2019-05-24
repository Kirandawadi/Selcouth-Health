#include <Arduino.h>
#include <Selcouth.h>


unsigned int address = 0;

Temperature_Sensor temp(Gun_Power_Pin,Gun_Eeprom_Power_Pin,Gun_Trig_Pin);
BP_Meter bp(BP_Power_Pin,BP_Measure_Pin);

int main(void)
{
  Serial.begin(9600);
 // bp.Eeprom_Erase(disk1,uint8_t(address),0);
  //bp.Get_Data();
  Serial.print("Hello check");
  return 0;
}
