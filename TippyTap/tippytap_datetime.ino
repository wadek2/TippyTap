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


  

  setTodaysDate();
}

void dailyAlarm(){
 //if there is an Alarm
  if ( RTC.alarm(ALARM_2) ) {     //has Alarm2 triggered?
    //yes, act on the alarm
    Serial.println("alarm triggered");
    //change the directory to Root
   // chdirRoot();
    //update todaysdate to todaysdate
//    mkdirFolder();
 //   chdirFolder();
   // Create a new folder named after today's date.
  }
}

// gives a const char* value to the variable todaysDate in tippytap_setup
void setTodaysDate(){
  tmElements_t tm;
  RTC.read(tm);
    // change todays date from an integer to a string
  //String str1 = String(tm.Day+'_'+tm.Month+'_'+tm.Year);     // for the study
  String str1 = String(tm.Minute);    // for testing
  // change todays date from a string to a constant character
  const char* string1 = str1.c_str();
  
  // Set today's Date
  todaysDate = string1; 
  Serial.println("updatetodaysDate");
  Serial.println(todaysDate);
  //mkdateFolder(todaysDate);
}

