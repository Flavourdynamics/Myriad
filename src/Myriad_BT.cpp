#include <Myriad_BT.h>

Myriad_BT::Myriad_BT(){
  //this->Bluetooth.begin("Starshroud");
}

void Myriad_BT::receive(){      // Gather data from serial
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
  strcpy(BTtempchars, BTreceivedchars);    // Copy to protect the original data because strtok() replaces the commas with \0
  
  char * strtokIndx;                       // Used by strtok() as an index
  strtokIndx = strtok(BTtempchars,",");    // Target the first segment
  strcpy(BTprimary, strtokIndx);           // Save first segment as a string
  
  strtokIndx = strtok(NULL, ",");          // Continue where the previous call left off
  strcpy(BTmessage, strtokIndx);           // Save second segment as a string
  BTsecondary = atoi(strtokIndx);          // Save second segment as an integer
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
  
  if (strcmp(BTprimary, "Power")  == 0){
    LEDonoff = !LEDonoff;
    FastLED.clear(true);
  }
  else if (strcmp(BTprimary, "Brightness") == 0){
    LEDtargbright = BTsecondary;
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
  else if (strcmp(BTprimary, "Patlist") == 0){
    BTappneedspats = true;
  }
  else if (strcmp(BTprimary, "Pallist") == 0){
    BTappneedspals = true;
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

  if(BTappneedspats == true){

  }
  else if(BTappneedspals == true){

  }
  switch(BTupcount){
    case 0:
      outdata = outdata + "<bright," + LEDcurbright + ">";
      break;
    case 1:
      outdata = outdata + "<bpm," + beat + ">";
      break;
    case 2:
      outdata = outdata + "<shuf_int," + STATEpatshuffleinterval + ">";
      break;
    case 3:
      outdata = outdata + "<pal_int," + STATEpalshuffleinterval + ">";
      break;
    case 4:
      outdata = outdata + "<fps," + framerate + ">";
      break;
    case 5:
      outdata = outdata + "<pattern," + Pattern_List[patternum].Name + ">";
      break;
    case 6:
      bipe = patshuffle;
      outdata = outdata + "<pat_tog," + bipe + ">";
      break;
    case 7:
      bipe = palshuff;
      outdata = outdata + "<pal_tog," + bipe + ">";
      break;
    case 8:
      bipe = palmatch;
      outdata = outdata + "<palmatch," + bipe + ">";
      break;
    case 9:
      bipe = palnum;
      outdata = outdata + "<palnum," + bipe + ">";
      break;
  }
  BTupcount++;
  if(BTupcount > 9){
    BTupcount = 0;
  }
  Bluetooth.print(outdata);
}

void Myriad_BT::bulkstatusupdate(){
  String outdata = "";
  uint8_t bipe; // Standin to cast bools

  // All values sent as strings
  // <status,LEDcurbright#framerate#STATEpatshuffleinterval#STATEpalshuffleinterval#patshuffle#palshuff#palmatch#patternum#palnum>
  outdata = outdata + "<status,";
  outdata = outdata + LEDcurbright + "#";
  outdata = outdata + framerate + "#";
  outdata = outdata + STATEpatshuffleinterval + "#";
  outdata = outdata + STATEpalshuffleinterval + "#";
  bipe = patshuffle;  // cast bool to byte
  outdata = outdata + bipe + "#";
  bipe = palshuff;    // cast bool to byte
  outdata = outdata + bipe + "#";
  bipe = palmatch;    // cast bool to byte
  outdata = outdata + bipe + "#";
  bipe = patternum;
  outdata = outdata + bipe + "#";
  bipe = palnum;      // cast int to byte
  outdata = outdata + bipe + ">"; // last value uses ">" end marker
  Bluetooth.print(outdata);
}

extern const uint16_t NUMpatterns;
void Myriad_BT::sendpatterns(){
  String outdata = "";
  for(uint16_t x = 0; x < NUMpatterns; x++){
    outdata = outdata + Pattern_List[x].Name + "#";
  }
  Bluetooth.print("<pat_list," + outdata + ">");
  EVERY_N_SECONDS(1){
    Serial.println("Sending pat list");
  }
  BTappneedspats = false;
}

void Myriad_BT::sendpalettes(){
  String outdata = "";
  for(uint16_t x = 0; x < NUMpalettes; x++){
    outdata = outdata + Palette_List[x].Name + "#";
  }
  Bluetooth.print("<pal_list," + outdata + ">");
  EVERY_N_SECONDS(1){
    Serial.println("Sending pal list");
  }
  BTappneedspals = false;
}

#ifdef ESP32
void Myriad_BT::proc(){
  EVERY_N_MILLIS(STATEBTinterval){
    if(Bluetooth.connected()){    // Only run this if connection established
      this->receive();            // Get data
      if(BTappneedspats){
        this->sendpatterns();
        BTappneedspats = false;
      }
      if(BTappneedspals){
        this->sendpalettes();
        BTappneedspals = false;
      }
      if (BTnewdata == true) {    // If there is new data available
        this->parse();            // Get data and validate it
        this->select();           // Process valid data
      }
      this->uplist();             // Spam the app with status updates
    }
  }
}
#endif
#ifdef TEENSY
void Myriad_BT::proc(){
  EVERY_N_MILLIS(STATEBTinterval){
    this->receive();            // Get data
    if(BTappneedspats){
      this->sendpatterns();
      BTappneedspats = false;
    }
    if(BTappneedspals){
      this->sendpalettes();
      BTappneedspals = false;
    }
    if (BTnewdata == true) {    // If there is new data available
      this->parse();            // Get data and validate it
      this->select();           // Process valid data
    }
    this->bulkstatusupdate();   // Send all status info as a single packet
    //this->uplist();             // Spam the app with status updates
  }
}
#endif