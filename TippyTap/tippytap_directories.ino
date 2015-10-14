

void directoriesSetup(){
 //const char* x;
 //x = todaysDate; 
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

//void mkdateFolder(){
//  tmElements_t tm;
//  RTC.read(tm);
//  if (!sd.mkdir_util(tm.Minute)) {
//    error("Creating today's date folder failed");
//  }
//  cout << F("Created todays date folder success!!");
//   
//}

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


