#ifndef Myriad_BT_h
#define Myriad_BT_h
// Throw Bluetooth.begin("Name"); into setup
// and then run BT.proc(); in loop
#include <Arduino.h>
#include <BluetoothSerial.h>
#ifndef FASTLED_VERSION
  #include <FastLED_Timers.h>
#endif
//BluetoothSerial Bluetooth;
void patcrossproc(int newpatnum);
extern const String BTname;

class Myriad_BT{
  private:////////////////////////////////////////////////////
    #define BTnumchars 20
    char BTreceivedchars[BTnumchars];
    bool BTnewdata = false;
    char BTprimary[BTnumchars] = {0};
    int BTsecondary;
    char BTmessage[BTnumchars];
    uint8_t BTupcount;
    uint16_t STATEreadinterval = 20;
    
  public:////////////////////////////////////////////////////
    BluetoothSerial Bluetooth;
    Myriad_BT();
    void receive();
    void parse();
    void select();
    void uplist();
    void proc();
};

#endif