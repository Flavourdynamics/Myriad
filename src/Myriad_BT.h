#ifndef Myriad_BT_h
#define Myriad_BT_h
#include <Myriad_Types.h>
// Throw BT.Bluetooth.begin("Name"); into setup
// and then run BT.proc(); in loop
#include <Arduino.h>
#include <BluetoothSerial.h>
#ifndef FASTLED_VERSION
  #include <FastLED_Timers.h>
#endif
void patcrossproc(int newpatnum);
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
    BluetoothSerial Bluetooth;
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