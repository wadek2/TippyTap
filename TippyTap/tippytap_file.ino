# define DEFINTETEST "Wade.csv"

void fileSetup(){
 //createFile();
 openFile();
 writeHeader();
// logTime = micros()/(1000UL*SAMPLE_INTERVAL_MS) + 1;
// logTime *= 1000UL*SAMPLE_INTERVAL_MS;
}

//void createFile() {
//  tmElements_t tm;
//  RTC.read(tm);
//    // change todays date from an integer to a string
//  //String str1 = String(tm.Day+'_'+tm.Month+'_'+tm.Year);     // for the study
//  String str1 = String(tm.Hour);    // for testing
//  String str2 = ".csv";
//  String str3 = String (str1 + str2);
//  Serial.println(str3);
//  // change todays date from a string to a constant character
//  const char* fileName = str3.c_str();
//  Serial.println(fileName);
//  
//  if (sd.exists(fileName)) {  //create file name todaysDate
//    if(fileName){
//      Serial.println("FILE    created!!!!!!");
//    } 
//    else {
//      error("Can't create file name");
//    }
//  }
//}

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
  if (!file.open(DEFINTETEST, O_CREAT | O_WRITE | O_EXCL)) {     //if todays date doesn't open send an error
    error("file.open");
  }
  do {
    delay(10);
  } while (Serial.read() >= 0);
  
  Serial.print(F("Logging to: "));
  Serial.println(DEFINTETEST);
  Serial.println(F("Type any character to stop"));

 logTime = micros()/(1000UL*SAMPLE_INTERVAL_MS) + 1;
 logTime *= 1000UL*SAMPLE_INTERVAL_MS;

  //writeHeader(); 
  
}


void writeHeader() {
  Serial.print("writing header");
  file.print(F("DateTime"));
  file.print(F(",X"));
  file.print(F(",Y"));
  file.print(F(",Z"));
  file.println();
}



void writeAccelerometerToFile(){
   tmElements_t tm;
   RTC.read(tm);
   file.print( 'Date ' + tm.Day + '/' + tm.Month + '/' + tm.Year + 'Time ' + tm.Hour + ':' + tm.Minute + ':' + tm.Second);
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
