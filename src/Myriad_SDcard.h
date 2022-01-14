#ifndef Myriad_SDcard_h
#define Myriad_SDcard_h

#include <SD.h>
File SDFile;
const int chipSelect = BUILTIN_SDCARD;
#define SDnumchars 1000 // An upper cut-off for how many chracters will be read from SDfile
char SDchars[SDnumchars];

int16_t LEDtargbright;
int16_t patternum;
int16_t palnum;
int16_t STATEpatshuffleinterval;
int16_t STATEpalshuffleinterval;
bool patshuffle;
bool palshuff;
bool palmatch;

void SDsetup();
void SDwrite();
void SDread();
void SDparse();

#endif