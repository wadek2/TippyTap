void initializeSDSetup (){

// Initialize the SD card at SPI_HALF_SPEED to avoid bus errors with
  // breadboards.  use SPI_FULL_SPEED for better performance.
  if (!sd.begin(SD_CHIP_SELECT, SPI_HALF_SPEED)) {
    sd.initErrorHalt();
  }
  
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

