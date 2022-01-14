#include <SD.h>

File SDFile;
const int chipSelect = BUILTIN_SDCARD;
#define SDnumchars 1000 // An upper cut-off for how many chracters will be read from SDfile
char SDchars[SDnumchars];

int16_t LEDtargbright = 1550;
int16_t patternum = 760;
int16_t palnum = 350;
int16_t STATEpatshuffleinterval = 1200;
int16_t STATEpalshuffleinterval = 1000;
bool patshuffle = true;
bool palshuff = true;
bool palmatch = true;

void SDsetup(){
  while (!Serial){
    Serial.begin(9600);
  }
  Serial.print("Initializing SD card...");

  if (!SD.begin(chipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
}

void SDwrite(){
  SD.remove("settings.txt"); // remove old file
  SDFile = SD.open("settings.txt", FILE_WRITE);
  
  if (SDFile) { // if the file opened okay, write to it:
    Serial.print("Writing to settings.txt...");
    //SDFile.printf("%i,%i,%i,%i,%i,%i,%i,%i", LEDtargbright,patternum,palnum,STATEpatshuffleinterval,STATEpalshuffleinterval,patshuffle,palshuff,palmatch);
    SDFile.printf("%i,%i,%i,%i,%i,%i,%i,%i", LEDtargbright,patternum,palnum,STATEpatshuffleinterval,STATEpalshuffleinterval,patshuffle,palshuff,palmatch);
    SDFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening settings.txt"); // if the file didn't open
  }
}
  
void SDread(){
  SDFile = SD.open("settings.txt");
  
  if (SDFile) {
    Serial.println("Reading SD");
    uint16_t ndx = 0;
    
    while (SDFile.available()) { // read from the file until there's nothing else in it:
      SDchars[ndx] = SDFile.read();
      ndx++;
    }
    for(uint16_t i = 0; i <= ndx; i++){
      Serial.print(SDchars[i]);
    }
    Serial.println();
    SDFile.close();
    SDparse();
  } else { // if the file didn't open, print an error:
    Serial.println("error opening settings.txt");
  }
}

void SDparse(){
  int TLEDtargbright;
  int Tpatternum;
  int Tpalnum;
  int TSTATEpatshuffleinterval;
  int TSTATEpalshuffleinterval;
  bool Tpatshuffle;
  bool Tpalshuff;
  bool Tpalmatch;
  
  Serial.println(SDchars);
  sscanf(SDchars, "%i,%i,%i,%i,%i,%i,%i,%i\n", &TLEDtargbright, &Tpatternum, &Tpalnum, &TSTATEpatshuffleinterval, &TSTATEpalshuffleinterval, &Tpatshuffle, &Tpalshuff, &Tpalmatch);
  
  Serial.print("TLEDtargbright ");
  Serial.println(TLEDtargbright);
  Serial.print("Tpatternum ");
  Serial.println(Tpatternum);
  Serial.print("Tpalnum ");
  Serial.println(Tpalnum);
  Serial.print("TSTATEpatshuffleinterval ");
  Serial.println(TSTATEpatshuffleinterval);
  Serial.print("TSTATEpalshuffleinterval ");
  Serial.println(TSTATEpalshuffleinterval);
  Serial.print("Tpatshuffle ");
  Serial.println(Tpatshuffle);
  Serial.print("Tpalshuff ");
  Serial.println(Tpalshuff);
  Serial.print("Tpalmatch ");
  Serial.println(Tpalmatch);
}