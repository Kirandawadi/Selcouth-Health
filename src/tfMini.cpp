
#include "TFMPlus.h" // Include TFMini Plus Library v1.3.4
#include "printf.h"   // May not work corrrectly with Intel devices

#include "tfMini.h"     //Our Own Library

/*  - - - - -  A few useful Lidar commands    - - - - - */

// Try three times to get the firmware version number
// which is saved as 'tfmp.version', a three byte array.
TFMPlus tfmP;         // Create a TFMini Plus object

uint16_t tfDist;       // Distance measurement in centimeters (default)
uint16_t tfFlux;       // Luminous flux or intensity of return signal
uint16_t tfTemp;       // Temperature in degrees Centigrade (coded)
uint16_t loopCount;    // Loop counter (1-20)
long sum = 0;
uint16_t HEIGHT = 0;

void firmwareVersion()
{
    for( uint8_t fvi = 1; fvi < 4; ++fvi)
    {
        if( tfmP.sendCommand( OBTAIN_FIRMWARE_VERSION, 0))
        {
            // If successful, display the version number...
            printf( "Lidar firmware version: %1u.%1u.%1u\r\n",
                tfmP.version[ 0], tfmP.version[ 1], tfmP.version[ 2]);
            break;      // and brreak out of loop.
        }
        else
        {
            // If not successful, display the attempt number
            // and the error: HEADER, CHERCKSUM, SERIAL, tec.
            printf( "Get firmware version failed. "); // Display error message...
            printf( "Attempt: %u", fvi);              // attempt number..
            tfmP.printStatus( false);                 // and error status.
            printf("\r\n");
        }
        delay(100);  // Wait to try again
    }
}

void factoryReset()
{
    printf( "Lidar factory reset ");
    if( tfmP.sendCommand( RESTORE_FACTORY_SETTINGS, 0))
    {
        printf( "passed.\r\n");
    }
    else
    {
        printf( "failed.");
        tfmP.printStatus( false);
    }
}

void frameRate( uint16_t rate)
{
    printf( "Lidar frame rate ");
    if( tfmP.sendCommand( SET_FRAME_RATE, rate))
    {
        printf( "set to %2uHz.\r\n", rate);
    }
    else
    {
        printf( "command failed.");
        tfmP.printStatus( false);
    }
}

void saveSettings()
{
    printf( "Lidar device settings ");
    if( tfmP.sendCommand( SAVE_SETTINGS, 0))
    {
        printf( "saved.\r\n");
    }
    else
    {
        printf( "not saved.");
        tfmP.printStatus( false);
    }
}

/*  - - -   End of useful Lidar commands   - - - -   */

void tfMini_Initialize(void)
{
    printf_begin();          // Initialize printf.
    printf("\r\nTFMPlus Library Example - 07JUN2019\r\n");  // say 'hello'

    Serial1.begin( 115200);  // Initialize TFMPLus device serial port.
    delay(20);               // Give port time to initalize
    tfmP.begin( &Serial1);   // Initialize device library object and...
                             // pass device serial port to the object.

    // Send commands to device during setup.
    firmwareVersion();
    frameRate(20);
    //saveSettings();
    //factoryReset();

    // Initialize the variables for this example
    loopCount = 0;         // Reset loop counter.
    tfDist = 0;            // Clear device data variables.
    tfFlux = 0;
    tfTemp = 0;

    delay(500);            // And wait for half a second.
}

float get_Height(void)
{
  float h = 0;

  delay(3000);

  for( uint8_t fvi = 1; fvi <=12; ++fvi)
  {
      HEIGHT = 219.00 - (tfmP.getData( tfDist, tfFlux, tfTemp));

      Serial.println(HEIGHT);

      delay(10);
  }
  return HEIGHT;
}