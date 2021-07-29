#ifndef Myriad_BT_h
#define Myriad_BT_h
//#include "Wire.h"
#include <BluetoothSerial.h>

BluetoothSerial Bluetooth;
const byte BTnumchars = 20;
char BTreceivedchars[BTnumchars];
bool BTnewdata = false;
char BTprimary[BTnumchars] = {0};
int BTsecondary;
char BTmessage[BTnumchars];
uint8_t BTupcount;

void BTreceive(){
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char recievedchar;

  while (Bluetooth.available() > 0 && BTnewdata == false) {
    recievedchar = Bluetooth.read();

    if (recvInProgress == true) {
      if (recievedchar != endMarker) {
          BTreceivedchars[ndx] = recievedchar;
          ndx++;
          if (ndx >= BTnumchars) {
              ndx = BTnumchars - 1;
          }
      }
      else {
          BTreceivedchars[ndx] = '\0'; // terminate the string
          recvInProgress = false;
          ndx = 0;
          BTnewdata = true;
      }
    }
    else if (recievedchar == startMarker) {
        recvInProgress = true;
    }
  }
}

void BTparse() {
  char BTtempchars[BTnumchars];
  strcpy(BTtempchars, BTreceivedchars);     // this temporary copy is necessary to protect the original data because strtok() replaces the commas with \0
  
  char * strtokIndx;                          // this is used by strtok() as an index  
  strtokIndx = strtok(BTtempchars,",");      // get the first part - the string
  strcpy(BTprimary, strtokIndx);             // copy it to messageFromPC
  
  if(strcmp(BTprimary, "X") == 0){           
    strtokIndx = strtok(NULL, ",");           // this continues where the previous call left off
    strcpy(BTmessage, strtokIndx);           // get a text string if app is sending text
  } else {
    strtokIndx = strtok(NULL, ",");             
    BTsecondary = atoi(strtokIndx);          // convert this part to an integer
  }
}

void patcrossproc(int newpatnum);
void BTselect(){               // or strcpy(STATEloopval, BLEprimary);
  Serial.print(BTprimary);
  Serial.print("-");
  Serial.println(BTsecondary);
  
  if (strcmp(BTprimary, "Power") == 0){
    LEDonoff = !LEDonoff;
    FastLED.clear(true);
  }
  else if (strcmp(BTprimary, "Brightness") == 0){
    LEDtargbright = BTsecondary;
  }
  else if (strcmp(BTprimary, "Speed") == 0){
    STATEloopinterval = BTsecondary;
  }
  else if (strcmp(BTprimary, "PalSpd") == 0){
    STATEpalshuffleinterval = BTsecondary;
  }
  else if (strcmp(BTprimary, "ShuffSpd") == 0){
    STATEpatshuffleinterval = BTsecondary;     // Time is sent in seconds
  }
  else if (strcmp(BTprimary, "PatShuffleTog") == 0){
    patshuffle = !patshuffle;
  }
  else if (strcmp(BTprimary, "PalShuffleTog") == 0){
    palshuff = !palshuff;
  }
  else if (strcmp(BTprimary, "Palette") == 0){
    palnum = BTsecondary;
  }
  else if (strcmp(BTprimary, "BPM") == 0){
    beat = BTsecondary;
  }
  else if (strcmp(BTprimary, "Pattern") == 0){
    patcrossproc(BTsecondary);
  }
}

void BTuplist(){         
  String outdata = "";
  byte bipe; 
  /*
  outdata = outdata + "<bright," + LEDbright + ",amps," + LEDamps + ",speed," + STATEloopinterval + ",bpm," + beat + ",bpm," 
  + beat + ",shufint," + STATEpatshuffleinterval + ",palshuf," + STATEpalshuffleinterval + ",fps," + LEDS.getFPS() + ",pattern," + PATTERNnum + ">";
  */  
  switch(BTupcount){
    case 0:
      outdata = outdata + "<bright," + LEDcurbright + ">";
    break;
    case 1:
      outdata = outdata + "<speed," + STATEloopinterval + ">";
    break;
    case 2:
      outdata = outdata + "<bpm," + beat + ">";
    break;
    case 3:
      outdata = outdata + "<shufint," + STATEpatshuffleinterval + ">";
    break;
    case 4:
      outdata = outdata + "<palshuffint," + STATEpalshuffleinterval + ">";
    break;
    case 5:
      outdata = outdata + "<fps," + framerate + ">";
    break;
    case 6:
      outdata = outdata + "<pattern," + patternum + ">";
    break;
    case 7:
      bipe = palshuff;
      outdata = outdata + "<palshuftog," + bipe + ">";
    break;
    case 8:
      bipe = patshuffle;
      outdata = outdata + "<patshuftog," + bipe + ">";
    break;
  }
  
  BTupcount++;
  if(BTupcount > 8){
    BTupcount = 0;
  }
  Bluetooth.print(outdata);
}

void BTproc(){
  EVERY_N_MILLIS(STATEreadinterval){
    if(Bluetooth.connected()){
      BTreceive();
      if (BTnewdata == true) {          
        BTparse();
        BTselect(); 
        BTnewdata = false;
      }
      BTuplist();
    }
  }
}

#endif