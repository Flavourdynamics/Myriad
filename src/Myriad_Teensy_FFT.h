#ifndef Myriad_Teensy_Audio_h
#define Myriad_Teensy_Audio_h
#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

class Myriad_Teensy_FFT{
  private:
    AudioInputI2S       i2s;
    AudioMixer4         mixer;
    AudioAnalyzeFFT256  FFT;
    AudioConnection patchCord1();
    AudioConnection patchCord2();
    AudioConnection patchCord3();
    void teensyfft();
  public:
    Myriad_Teensy_FFT();
    void teensyprint();
    float fftdata[13];
};



#endif