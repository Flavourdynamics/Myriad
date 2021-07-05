#ifndef Myriad_Time_h
#define Myriad_Time_h
#include <Arduino.h>
#include <time.h>
#ifndef __INC_FASTSPI_LED2_H
  #include <FastLED_Timers.h>
#endif

class Myriad_Time {
  private:
    int8_t timezone = -7; 
    int8_t daysavetime = 1;
    
  public:
    Myriad_Time();
  
    uint8_t sleephour;
    uint8_t sleepminute;

    struct tm tmstruct;

    void gettime();

    void initialize();
};

#endif