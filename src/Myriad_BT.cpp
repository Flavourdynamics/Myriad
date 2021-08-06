#include <Myriad_BT.h>

Myriad_BT::Myriad_BT(){
  //Bluetooth.begin(BTname);
}

void Myriad_BT::receive(){
  static bool recvInProgress = false;
  static uint16_t ndx = 0;
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

void Myriad_BT::parse() {
  char BTtempchars[BTnumchars];
  strcpy(BTtempchars, BTreceivedchars);     // this temporary copy is necessary to protect the original data because strtok() replaces the commas with \0
  
  char * strtokIndx;                          // this is used by strtok() as an index  
  strtokIndx = strtok(BTtempchars,",");      // get the first part - the string
  strcpy(BTprimary, strtokIndx);             // copy it to messageFromPC
  
  if(strcmp(BTprimary, "X") == 0){           
    strtokIndx = strtok(NULL, ",");           // this continues where the previous call left off
    strcpy(BTmessage, strtokIndx);           // get a text string if app is sending text
    Serial.print("String received: ");
    Serial.println(BTmessage);
  } else {
    strtokIndx = strtok(NULL, ",");             
    BTsecondary = atoi(strtokIndx);          // convert this part to an integer
  }
}

void patchangeproc(int newpatnum);
extern bool LEDonoff;
extern uint8_t LEDtargbright;
extern uint16_t STATEloopinterval;
extern uint16_t STATEpalshuffleinterval;
extern uint16_t STATEpatshuffleinterval;
extern bool patshuffle;
extern bool palshuff;
extern bool palmatch;
extern uint8_t palnum;
extern uint8_t beat;
#ifdef ESP32Virtual
void Myriad_BT::select(){             // or strcpy(STATEloopval, BLEprimary);
  
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
    STATEshuffleinterval = BTsecondary;     // Time is sent in seconds
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
  BTnewdata = false;
}
#else
void Myriad_BT::select(){             // or strcpy(STATEloopval, BLEprimary);
  
  Serial.print(BTprimary);
  Serial.print("-");
  Serial.println(BTsecondary);
  
  if (strcmp(BTprimary, "Power") == 0){
    LEDonoff = !LEDonoff;
    //FastLED.clear(true);
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
    palmatch = false;
    palshuff = !palshuff;
  }
  else if (strcmp(BTprimary, "PalMatch") == 0){
    palmatch = !palmatch;
    palshuff = false;
  }
  else if (strcmp(BTprimary, "Palette") == 0){
    palmatch = false;
    palnum = BTsecondary;
  }
  else if (strcmp(BTprimary, "BPM") == 0){
    beat = BTsecondary;
  }
  else if (strcmp(BTprimary, "Pattern") == 0){
    patshuffle = false;
    patchangeproc(BTsecondary);
  }
  else if (strcmp(BTprimary, "Listme") == 0){
    BTappneedslists = true;
  }
  BTnewdata = false;
}
#endif

extern uint8_t LEDcurbright;
extern uint32_t framerate;
extern uint8_t patternum;
void Myriad_BT::uplist(){
  String outdata = "";
  uint8_t bipe; 
  /*
  outdata = outdata + "<bright," + LEDbright + ",amps," + LEDamps + ",speed," + STATEloopinterval + ",bpm," + beat + ",bpm," 
  + beat + ",shufint," + STATEshuffleinterval + ",palshuf," + STATEpalshuffleinterval + ",fps," + LEDS.getFPS() + ",pattern," + PATTERNnum + ">";
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
      outdata = outdata + "<pattern," + Pattern_List[patternum].Name + ">";
    break;
    case 7:
      bipe = patshuffle;
      outdata = outdata + "<patshuftog," + bipe + ">";
    break;
    case 8:
      bipe = palshuff;
      outdata = outdata + "<palshuftog," + bipe + ">";
    break;
    case 9:
      bipe = palmatch;
      outdata = outdata + "<palmatch," + bipe + ">";
    break;
  }
  BTupcount++;
  if(BTupcount > 9){
    BTupcount = 0;
  }
  Bluetooth.print(outdata);
}
extern const uint16_t NUMpatterns;
void Myriad_BT::sendpatterns(){
  String outdata = "";
  for(uint16_t x = 0; x < NUMpatterns; x++){
    outdata = outdata + Pattern_List[x].Name + "#";
  }
  Bluetooth.print("<patternlist," + outdata + ">");
  EVERY_N_SECONDS(1){
    Serial.println("Sending pat list");
  }
}

void Myriad_BT::sendpalettes(){
  String outdata = "";
  for(uint16_t x = 0; x < NUMpalettes; x++){
    outdata = outdata + Palette_List[x].Name + "#";
  }
  Bluetooth.print("<palettelist," + outdata + ">");
  EVERY_N_SECONDS(1){
    Serial.println("Sending pal list");
  }
}

void Myriad_BT::proc(){
  EVERY_N_MILLIS(STATEBTinterval){
    if(Bluetooth.connected()){    // Only run this if connection established
      this->receive();            // Get data
      if(BTappneedslists == true){    // If the app does not have the pattern list
        this->sendpatterns();     // Send the pattern list
        this->sendpalettes();
        BTappneedslists = false;
      }
      if (BTnewdata == true) {    // If there is new data available
        this->parse();            // Get data and validate it
        this->select();           // Process valid data
      }
      this->uplist();             // Spam the app with status updates
    }
  }
}