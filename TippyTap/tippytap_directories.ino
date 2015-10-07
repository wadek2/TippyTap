

void directoriesSetup(){
 //const char* x;
 //x = todaysDate; 
    //Make the initial working directory
  if (!sd.mkdir("Data")) {           //Call it Data, this will be the Root Directory
    error("Create Data failed");
  }
  cout << F("Created Data\n");
  chdirRoot();   //Go into the Data Directory
  mkdateFolder(todaysDate); // Make a folder based on today's date
  //setTodaysDate();
  
}

void mkdateFolder(const char* x){
  if (!sd.mkdir(x)) {
    error("Creating today's date folder failed");
  }
  cout << F("Created todays date folder success!!");
   
}

void chdirFolder(const char* x){
    if (!sd.chdir(x)) {
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


