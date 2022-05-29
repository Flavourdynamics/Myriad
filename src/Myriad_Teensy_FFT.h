#ifndef Myriad_Teensy_FFT_h
#define Myriad_Teensy_FFT_h
#include <Arduino.h>
#ifndef FASTLED_VERSION
  //#include <FastLED_Timers.h>
#endif
#include <Myriad_Types.h>
#include <Statistic.h>
#define EQbins         14    // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define EQsamples      256   // Must be a power of 2
#define EQreadint      10    // How often data is gathered and processed
#define EQdeclineint   5     // How often to flat decline data
#define sampleruntime  4500  // 

extern float fftdata[];
//extern uint32_t sampleruntime;

// Usable output
extern uint16_t EQscaled[];       // EQ values scaled to LEDSy
extern uint16_t EQ10000scaled[];  // EQ values scaled to 10000 for more precise mapping
extern uint16_t EQflatdecline[];  // EQ values that decay at 1 LED every run
extern uint16_t EQsummed10000;
// Beat output
extern uint8_t EQbeatDetected[];
extern bool EQbeat;
// FFT Buffers and boundaries
// FFT parameters
extern float EQnoisefloor[EQbins];
extern float EQmintops[EQbins];
extern float EQbuff[];    // Input buffer collects data directly from the FFT
extern float EQdecay[];   // Version of FFT data that 
extern float EQmaxes[];   // A moving maxiumum for each FFT band
extern float EQmins[];
extern Statistic EQstatstotal[];
extern float EQaverage[];
extern float EQstDev[];
// Beat
extern uint16_t EQbeatInterval[];
extern uint16_t EQbeatIntervalOld[];
extern uint16_t EQconstantBeatCounter[];
//elapsedMillis EQbeatTimer[EQbins];
extern const uint8_t ledPin;
extern bool ledState;
extern uint16_t fftindex[];
extern uint16_t fftindex256[];
extern uint16_t fftindex1024[];

extern bool ledState;
void EQsetup();

void EQdofft();
void EQnoisegate();
void EQupdatevalues();
void EQstats();

void EQbeatDetection();
void EQbeatBuckets();
void EQbeatBlink();
void EQprintDetectedBeats();

float EQfindE(uint16_t bands, uint16_t bins); // Used for determining FFT binning
void EQcalcbins();                 // Used 
void EQcalibration();   // Calibrate values for noisethresh() gate function
void EQprintone(uint8_t target);
void EQprintall();
void EQproc();
void EQproc(bool); // calibration overload
void EQproc(byte, bool, uint8_t); // ([0=no, 1=buffer, 2=scaled], printone, target)

#endif