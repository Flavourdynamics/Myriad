#ifndef Myriad_Time_h
#define Myriad_Time_h
#ifdef ESP32
  #include <Arduino.h>
  #include <time.h>
  #ifndef FASTLED_VERSION
    #include <FastLED_Timers.h>
  #endif

class Myriad_Time {
  private:
    int8_t timezone = -7; 
    int8_t daysavetime = 1;
    
  public:
    Myriad_Time();
  
    uint8_t sleepstarthour;
    uint8_t sleepstartminute;
    uint8_t sleependhour;
    uint8_t sleependminute;
    bool sleeping;

    struct tm tmstruct;

    void gettime();

    void initialize();

    void sleepcheck();
};
  #endif
#endif