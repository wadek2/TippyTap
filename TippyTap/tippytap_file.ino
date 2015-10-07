void fileSetup(){
 createFile();
 openFile();
 writeHeader();

}

void createFile() {
  while (sd.exists(todaysDate)) {  //call file name todaysDate
    if (!todaysDate) {   // if it doesn't exist make it
    } else {
      error("Can't create file name");
    }
  }
}

void openFile (){  
  if (!file.open(todaysDate, O_CREAT | O_WRITE | O_EXCL)) {     //if todays date doesn't open send an error
    error("file.open");
  }
  do {
    delay(10);
  } while (Serial.read() >= 0);
}


void writeHeader() {
  file.print(F("DateTime"));
  file.print(F(",X"));
  file.print(F(",Y"));
  file.print(F(",Z"));
  file.println();
}



void writeAccelerometerToFile(){
   tmElements_t tm;
   RTC.read(tm);
   file.print(tm.Year + '/' + tm.Month + '/' + tm.Day + ':' + tm.Hour + ':' + tm.Minute + ':' + tm.Second);
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
