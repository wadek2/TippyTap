// Add to this sketch setting the date and time to the arduino

#include <SPI.h>
#include <SdFat.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

//----------------------------------------------------------------------------------------------------------------------



// Create a Serial output stream.
ArduinoOutStream cout(Serial);

// Buffer for Serial input.
char cinBuf[40];

// Create a serial input stream.
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));

// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))

// Interval between data records in milliseconds.
// The interval must be greater than the maximum SD write latency plus the
// time to acquire and write data to the SD to avoid overrun errors.
// Run the bench example to check the quality of your SD card.
const uint32_t SAMPLE_INTERVAL_MS = 200;      //change number here (error msg will remind you, perhaps in code). CODE THIS CODE THIS CODE THIS CODE THIS

// SD card chip select pin.
const uint8_t SD_CHIP_SELECT = SS;

// Permit SD to be wiped if ALLOW_WIPE is true.
const bool ALLOW_WIPE = true;

//-----------------------------------------------------------------------------------------------------------------------

// File system object.
SdFat sd;

// Use for file creation in folders.
SdFile file;

// accelerometer data
Adafruit_MMA8451 mma = Adafruit_MMA8451();

// constant character for todays date
//const char* todaysDate = "a";

//-----------------------------------------------------------------------------------------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {} // wait for Leonardo
  delay(1000);


  initializeSDSetup ();
  dateTimeSetup();
  directoriesSetup();
  fileSetup();
  accelerometerSetup();
  controllerSetup();
}


