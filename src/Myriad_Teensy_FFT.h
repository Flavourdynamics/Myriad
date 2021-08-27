#ifndef Myriad_Teensy_Audio_h
#define Myriad_Teensy_Audio_h
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
AudioInputI2S       i2s;
AudioMixer4         mixer;
AudioAnalyzeFFT256  FFT;
AudioConnection patchCord1(i2s, 0, mixer, 0);
AudioConnection patchCord2(i2s, 1, mixer, 1);
AudioConnection patchCord3(mixer, 0, FFT, 0);

#endif