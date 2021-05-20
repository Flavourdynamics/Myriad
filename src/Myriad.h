#ifndef Myriad_h
#define Myriad_h
#include Arduino.h

class Myriad{
  public:
    Morse(int pin);
    void dot();
    void dash();
  private:
    int _pin;
};

#endif