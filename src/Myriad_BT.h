#ifndef Myriad_BT_h
#define Myriad_BT_h

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

void patcrossproc(int newpatnum){                                // Every time you switch patterns run this to begin crossfading
  Serial.print("proc - new pat num: ");
  Serial.println(newpatnum);
  targfade = faderlookup[newpatnum];
  oldpattern = patternum;                           // set the current pattern to be the old one so we can make it use the same variables
  crossct = 0;                                      // reset the blend amount
  patternum = newpatnum;                                      // increase pattern number   -> this just goes in sequence, but the best part of this is that you can control this by remote or any other system  

  rowcount[0] = rowcount[1];          // copy row status to default rows, and reset them for the new pattern
  rowcount[1] = 0;
  colcount[0] = colcount[1];
  colcount[1] = 0;
  count[0] = count[1];
  count[1] = 0;
  hue[0] = hue[1];
  hue[1] = 0;
}

void BTselect(){               // or strcpy(STATEloopval, BLEprimary);
  Serial.print(BTprimary);
  Serial.print("-");
  Serial.println(BTsecondary);
  
  if (strcmp(BTprimary, "Power") == 0){
    if(LEDamps > 0){
      LEDamps = 0;
    } else {
      LEDamps = 3000;
    }
    FastLED.setMaxPowerInVoltsAndMilliamps(5, LEDamps);
    FastLED.clear();
    FastLED.show();
  }
  else if (strcmp(BTprimary, "Brightness") == 0){
    LEDtargbright = BTsecondary;
    //FastLED.setBrightness(LEDcurbright);
    FastLED.show();
  }
  else if (strcmp(BTprimary, "Amps") == 0){
    LEDamps = BTsecondary;
    FastLED.setMaxPowerInVoltsAndMilliamps(5, LEDamps);
  }
  else if (strcmp(BTprimary, "Speed") == 0){
    STATEloopinterval = BTsecondary;
  }
  else if (strcmp(BTprimary, "PalSpd") == 0){
    STATEpalshuffleinterval = BTsecondary;
  }
  else if (strcmp(BTprimary, "ShuffSpd") == 0){
    STATEshuffleinterval = BTsecondary;     // Time is sent in seconds
  }
  else if (strcmp(BTprimary, "Shuffle") == 0){
    patshuffle = !patshuffle;
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
  /*
  outdata = outdata + "<bright," + LEDbright + ",amps," + LEDamps + ",speed," + STATEloopinterval + ",bpm," + beat + ",bpm," 
  + beat + ",shufint," + STATEshuffleinterval + ",palshuf," + STATEpalshuffleinterval + ",fps," + LEDS.getFPS() + ",pattern," + PATTERNnum + ">";
  */  
  switch(BTupcount){
    case 0:
      outdata = outdata + "<bright," + LEDcurbright + ">";
    break;
    case 1:
      outdata = outdata + "<amps," + LEDamps + ">";
    break;
    case 2:
      outdata = outdata + "<speed," + STATEloopinterval + ">";
    break;
    case 3:
      outdata = outdata + "<bpm," + beat + ">";
    break;
    case 4:
      outdata = outdata + "<shufint," + STATEshuffleinterval + ">";
    break;
    case 5:
      outdata = outdata + "<palshuf," + STATEpalshuffleinterval + ">";
    break;
    case 6:
      outdata = outdata + "<fps," + LEDS.getFPS() + ">";
    break;
    case 7:
      outdata = outdata + "<pattern," + patternum + ">";
    break;
  }
  
  BTupcount++;
  if(BTupcount > 7){
    BTupcount = 0;
  }
  Bluetooth.print(outdata);
}

void BTproc(){
  EVERY_N_MILLIS_I(readloop, STATEreadinterval){
    //if(Bluetooth.connected()){
      BTreceive();
      if (BTnewdata == true) {          
        BTparse();
        BTselect(); 
        BTnewdata = false;
      }
      BTuplist();
    //}
  }
  readloop.setPeriod(STATEreadinterval);
}

#endif