#ifndef Myriad_BT_h
#define Myriad_BT_h
#include <Myriad_Types.h>
// Throw BT.Bluetooth.begin("Name"); into setup
// and then run BT.proc(); in loop
#include <Arduino.h>
#ifndef FASTLED_VERSION
  #include <FastLED_Timers.h>
#endif
#ifdef ESP32
  #include <BluetoothSerial.h>
#endif
//#ifdef TEENSY
  #define Bluetooth Serial1
  #define HWSERIAL Serial1
//#endif

void patchangeproc(int newpatnum);
extern const String BTname;

class Myriad_BT{
  private:////////////////////////////////////////////////////
    #define BTnumchars 500
    char BTreceivedchars[BTnumchars];
    bool BTnewdata = false;
    char BTprimary[BTnumchars];
    int BTsecondary;
    char BTmessage[BTnumchars];
    bool BTappneedspats = false;
    bool BTappneedspals = false;
    char startMarker = '<';
    char endMarker = '>';
    uint8_t BTupcount;
    uint16_t STATEBTinterval = 1000;

  public:////////////////////////////////////////////////////
    #ifdef ESP32
      BluetoothSerial Bluetooth;
    #endif
    Myriad_BT();
    void receive();       // Gather data from serial
    void parse();         // Clean recieved string
    void select();        // Process input
    void sendpalettes();  // Send palettes over serial
    void sendpatterns();  // Send patterns over serial
    void uplist();        // Cyclically send diferent pieces of data
    void bulkupdate();    // Send all status info in a single packet
    void proc();          // Full proceedure for main loop
};

#endif