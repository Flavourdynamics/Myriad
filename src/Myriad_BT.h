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
#ifdef TEENSY
  #define Bluetooth Serial1
  #define HWSERIAL Serial1
#endif

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
    bool BTappneedslists = false;;
    char startMarker = '<';
    char endMarker = '>';
    uint8_t BTupcount;
    uint16_t STATEBTinterval = 20;

  public:////////////////////////////////////////////////////
    #ifdef ESP32
      BluetoothSerial Bluetooth;
    #endif
    Myriad_BT();
    void receive();
    void parse();
    void select();
    void sendpalettes();
    void sendpatterns();
    void uplist();
    void proc();
};

#endif