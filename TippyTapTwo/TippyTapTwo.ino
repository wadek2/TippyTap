/*
 * Simple data logger.
 */
#include <SPI.h>
#include <SdFat.h>
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>

// SD chip select pin.  Be sure to disable any other SPI devices such as Enet.
const uint8_t chipSelect = SS;

// Interval between data records in milliseconds.
// The interval must be greater than the maximum SD write latency plus the
// time to acquire and write data to the SD to avoid overrun errors.
// Run the bench example to check the quality of your SD card.
const uint32_t SAMPLE_INTERVAL_MS = 200;

// Log file base name.  Must be six characters or less.
//------------------------------------------------------------------------------
// File system object.
SdFat sd;

// Log file.
SdFile file;

// Time in micros for next data record.
uint32_t logTime;


//--------------------------------------------------------------------------------
// Create a Serial output stream.
ArduinoOutStream cout(Serial);

// Buffer for Serial input.
char cinBuf[40];

// Create a serial input stream.
ArduinoInStream cin(Serial, cinBuf, sizeof(cinBuf));


// Error messages stored in flash.
#define error(msg) sd.errorHalt(F(msg))

//=============================================================================
//declare accelerometer
Adafruit_MMA8451 mma = Adafruit_MMA8451();

//==============================================================================
//------------------------------------------------------------------------------
void setup() {

  Serial.begin(9600);
  while (!Serial) {} // wait for Leonardo
  delay(1000);


  // Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }
  dateTimeSetup();
  accelerometerSetup();
  initializeSDSetup();
  directoriesSetup();
   openFile();
   // Write data header.
     writeHeader();

  // Start on a multiple of the sample interval.
  logTime = micros() / (1000UL * SAMPLE_INTERVAL_MS) + 1;
  logTime *= 1000UL * SAMPLE_INTERVAL_MS;


  
}
//==============================================================================
//loop: check for alarms, read accelerometer data, and etc.
//------------------------------------------------------------------------------
void loop() {
  
  // Time for next record.
  logTime += 1000UL * SAMPLE_INTERVAL_MS;

  // Wait for log time.
  int32_t diff;
  do {
    diff = micros() - logTime;
  } while (diff < 0);

  // Check for data rate too high.
  if (diff > 10) {
    error("Missed data record");
  }
  // log data into csv
  writeAccelerometerToFile();
  //print accelerometer data into serial monitor
  accelerometerSerial();
  // Force data to SD and update the directory entry to avoid data loss.
  if (!file.sync() || file.getWriteError()) {
    error("write error");
  }

  if (Serial.available()) {
    // Close file and stop.
    file.close();
    Serial.println(F("Done"));
    while (1) {}
  }
}

//=========================================================================
// initialize the SD

void initializeSDSetup (){

const bool ALLOW_WIPE = true;
// Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
//  if (!sd.begin(SD_CHIP_SELECT, SPI_HALF_SPEED)) {
//    sd.initErrorHalt();
//  }
//  
    // Check for empty SD.
  if (file.openNext(sd.vwd(), O_READ)) {
    cout << F("Found files/folders in the root directory.\n");
    if (!ALLOW_WIPE) {
      error("SD not empty, use a blank SD or set ALLOW_WIPE true.");
    } else {
      cout << F("Type: 'WIPE' to delete all SD files.\n");
      char buf[10];
      cin.readline();
      cin.get(buf, sizeof(buf));
      if (cin.fail() || strncmp(buf, "WIPE", 4) || buf[4] >= ' ') {
        error("Invalid WIPE input");
      }
      file.close();
      if (!sd.vwd()->rmRfStar()) {
        error("wipe failed");
      }
      cout << F("***SD wiped clean.***\n\n");
    }
  }
}


//==========================================================================
// set up the clock, reading time, and alarms
void dateTimeSetup(){  

//We need to write code that allows the user to set the date and time.

  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus() != timeSet) 
  Serial.println("Unable to sync with the RTC");
  else
  Serial.println("RTC has set the system time");   

        // Set an alarm
  //RTC.setAlarm(ALM2_EVERY_MINUTE,0,0,0,0); //ALM every minute
  //RTC.setAlarm(ALM2_MATCH_MINUTES, 0, 1, dowFriday); //ALM 1 time per an hour on Friday
  //RTC.setAlarm(ALM2_MATCH_MINUTES, 0);   //ALM 1 time per an hour
  RTC.setAlarm(ALM2_MATCH_HOURS, 0,0,0,0);   //ALM 1 time per a day
  

//  setTodaysDate();
}


void dailyAlarm(){
 //if there is an Alarm
  if ( RTC.alarm(ALARM_2) ) {     //has Alarm2 triggered?
    //yes, act on the alarm
    Serial.println("alarm triggered");
  }
}

//================================================================================
// Open File, if there is no file then Create File
void openFile (){  
  tmElements_t tm;
  RTC.read(tm);
    // change todays date from an integer to a string
  //String str1 = String(tm.Day+'_'+tm.Month+'_'+tm.Year);     // for the study
  String str1 = String(tm.Hour);    // for testing
  String str2 = ".csv";
  String str3 = String (str1 + str2);
  // change todays date from a string to a constant character
  const char* fileName = str3.c_str();
  if (!file.open(fileName, O_CREAT | O_WRITE | O_EXCL)) {     //if todays date doesn't open send an error
    error("file.open");
  }
  do {
    delay(10);
  } while (Serial.read() >= 0);
  
  Serial.print(F("Logging to: "));
  Serial.println(fileName);
  Serial.println(F("Type any character to stop"));
}

// Write data header in the file that you have created
void writeHeader() {
  Serial.print("writing header");
  file.print(F("DateTime"));
  file.print(F(",X"));
  file.print(F(",Y"));
  file.print(F(",Z"));
  file.println();
}

//====================================================================
// set up the directories structure, name the directories, switch between directories

void directoriesSetup(){

    //Make the initial working directory
  if (!sd.mkdir("Data")) {           //Call it Data, this will be the Root Directory
    error("Create Data failed");
  }
  cout << F("Created Data\n");
  chdirRoot();   //Go into the Data Directory
  mkdateFolder(); // Make a folder based on today's date
  chdirFolder();
  //setTodaysDate();
}

void mkdateFolder(){
  tmElements_t tm;
  RTC.read(tm);
    // change todays date from an integer to a string
  //String str1 = String(tm.Day+'_'+tm.Month+'_'+tm.Year);     // for the study
  String str1 = String(tm.Hour);    // for testing
  // change todays date from a string to a constant character
  const char* folderName = str1.c_str();
  
  if (!sd.mkdir(folderName)) {
    error("Creating today's date folder failed");
  }
  cout << F("Created todays date folder success!!");
   
}

void chdirFolder(){
  tmElements_t tm;
  RTC.read(tm);
    // change todays date from an integer to a string
  //String str1 = String(tm.Day+'_'+tm.Month+'_'+tm.Year);     // for the study
  String str1 = String(tm.Hour);    // for testing
  // change todays date from a string to a constant character
  const char* folderName = str1.c_str();
    if (!sd.chdir(folderName)) {
    error("chdir failed for todaysDate.\n");
  }
  cout << F("chdir to todaysDate\n");   
}
 
void chdirRoot(){
// Change volume working directory back to Data.
  if (!sd.chdir("/Data")) {
    error("chdir failed for Data.\n");
  }
  cout << F("chdir to Data\n");    
} 

//==================================================================================
//set up the accelerometer
//write the accelerometer data to the sd card

void accelerometerSetup(){
  Serial.println("Adafruit MMA8451 test!");
  

  if (! mma.begin()) {
    Serial.println("Couldnt start");
    while (1);
  }
  Serial.println("MMA8451 found!");
  
  mma.setRange(MMA8451_RANGE_2_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
 
}

void writeAccelerometerToFile(){
   tmElements_t tm;
   RTC.read(tm);
  String str1 = "Date";    
  String str2 = "Time";
  String str3 = String(tm.Day);
  String str4 = String(tm.Month);
  String str5 = String(tm.Year);
  String str6 = String(tm.Hour);
  String str7 = String(tm.Minute);
  String str8 = String(tm.Second);
  String str9 = "/";
  String str10 = ":";
  String str11 = "=";
  String str12 = String(str1 + str10 +  str3 + str9 + str4 + str9 + str5 + str2 + str11 + str6 + str10 + str7 + str10 + str8);
  // change dateTime from a string to a constant character
  const char* dateTime = str12.c_str();
   file.print( dateTime);
  // file.print("wade");
   file.write(',');
   mma.read();
   file.print(mma.x);
   file.write(','); 
   file.print(mma.y); 
   file.write(',');
   file.print(mma.z);
   file.println(); 
   sensors_event_t event; 
   mma.getEvent(&event);
   
}

//print accelerometer data to serial
void accelerometerSerial() {
  // Read the 'raw' data in 14-bit counts
  mma.read();
  Serial.print("X:\t"); Serial.print(mma.x); 
  Serial.print("\tY:\t"); Serial.print(mma.y); 
  Serial.print("\tZ:\t"); Serial.print(mma.z); 
  Serial.println();

  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  Serial.println();
  delay(100);
  
}

