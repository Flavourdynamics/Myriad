#ifndef Myriad_SDcard_h
#define Myriad_SDcard_h

#include <CSV_Parser.h>
#include <SD.h>
File SDfile;
const int chipSelect  = BUILTIN_SDCARD;
// "sLdcfx-" = "my_strings,my_longs,my_ints,my_chars,my_floats,my_hex,my_to_be_ignored\n"
// Unsigned by preceding the integer based specifiers ("L", "d", "c", "x") with "u".
CSV_Parser cp(/*format*/ "sd", /*has_header*/ true, /*delim*/ ','); 

extern uint8_t LEDtargbright;
extern uint8_t patternum;
extern uint8_t palnum;
extern uint16_t STATEpatshuffleinterval;
extern uint16_t STATEpalshuffleinterval;
extern bool patshuffle;
extern bool palshuff;
extern bool palmatch;

void SDparse(){
  if (cp.readSDfile("settings.csv")) {
    //char **strings = (char**)cp["SDstrings"];
    int16_t *numbers = (int16_t*)cp["SDintegers"];
    LEDtargbright = numbers[0];
    patternum = numbers[1];
    palnum = numbers[2];
    STATEpatshuffleinterval = numbers[3];
    STATEpalshuffleinterval = numbers[4];
    patshuffle = numbers[5];
    palshuff = numbers[6];
    palmatch = numbers[7];
  }
}

void SDsetup(){
  Serial.print("Initializing SD card...");
  if(!Serial){
    Serial.begin(115200);
  }
  if (!SD.begin(chipSelect)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialization done.");
  SDparse();
}

void SDwriteCSV(){
  SD.remove("settings.csv"); // remove old file
  SDfile = SD.open("settings.csv", FILE_WRITE);
  
  if (SDfile) { // if the file opened okay, write to it:
    Serial.print("Writing to settings.csv...");
    //SDFile.printf("%s,%i\n%s,%i\n%s%i\n%s,%i\n%s,%i\n%s,%i\n%s,%i\n%s,%i\n%s,%i", LEDtargbright,patternum,palnum,STATEpatshuffleinterval,STATEpalshuffleinterval,patshuffle,palshuff,palmatch);
    SDfile.print("SDstrings,SDintegers\n");
    SDfile.printf("%s,%i\n", "LEDtargbright", LEDtargbright);
    SDfile.printf("%s,%i\n", "patternum", patternum);
    SDfile.printf("%s,%i\n", "palnum", palnum);
    SDfile.printf("%s,%i\n", "STATEpatshuffleinterval", STATEpatshuffleinterval);
    SDfile.printf("%s,%i\n", "STATEpalshuffleinterval", STATEpalshuffleinterval);
    SDfile.printf("%s,%i\n", "patshuffle", patshuffle);
    SDfile.printf("%s,%i\n", "palshuff", palshuff);
    SDfile.printf("%s,%i\n", "palmatch", palmatch);
    //SDFile.print(csv_str);
    SDfile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening settings.csv"); // if the file didn't open
  }
}

void SDreadCSV(){
  Serial.println("Reading SD");
  if (cp.readSDfile("settings.csv")) {
    /*
    Serial.println("Accessing values by column number:");
    char **column_1 = (char**)cp["column_1"];
    int16_t *column_2 = (int16_t*)cp["column_2"];
  
    if (column_1 && column_2) {
      for(int row = 0; row < cp.getRowsCount(); row++) {
        Serial.print("row = ");
        Serial.print(row, DEC);
        Serial.print(", column_1 = ");
        Serial.print(column_1[row]);
        Serial.print(", column_2 = ");
        Serial.println(column_2[row], DEC);
      }
    } else {
      Serial.println("ERROR: At least 1 of the columns was not found, something went wrong.");
    }
  */
    Serial.println("Accessing values by column name:");
    char **strings = (char**)cp["SDstrings"];
    int16_t *numbers = (int16_t*)cp["SDintegers"];
    for(int row = 0; row < cp.getRowsCount(); row++) {
      Serial.print(row, DEC);
      Serial.print(". String = ");
      Serial.println(strings[row]);
      Serial.print(row, DEC);
      Serial.print(". Number = ");
      Serial.println(numbers[row], DEC);
    }
    Serial.println();
    SDfile.close();
  } else { // if the file didn't open, print an error:
    Serial.println("error opening settings.csv");
  }
}

#endif