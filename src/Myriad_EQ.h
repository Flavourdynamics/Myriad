#ifndef Myriad_EQ_h
#define Myriad_EQ_h
#include <Arduino.h>
#include <Myriad_Types.h>
#ifndef FASTLED_VERSION
  #include <FastLED_Timers.h>
#endif

// This file contains functions for audio processing
// Output is in EQscaled[i] and EQdecay[i]

// These have to go in setup() for FFT and to enable beat blinker
//    EQsampletimer = round(1000000 * (1.0 / EQsamplefreq));
//    pinMode(ledPin, OUTPUT);

#include <Statistic.h>
#define EQbins         14    // To change this, you will need to change the bunch of if statements describing the mapping from bins to bands
#define EQsamples      256    // Must be a power of 2
#define EQreadint      50 // How often data is gathered and processed
#define EQdeclineint   5  // How often to flat decline data

#ifdef ESP32
  #include <arduinoFFT.h>
  #define EQsamplefreq   40000 //40000  // Hz, must be 40000 or less due to ADC conversion time. Determines maximum frequency that can be analysed by the FFT Fmax=sampleF/2.
  #define EQsourcepin    2              // Signal in on this pin
#endif

class Myriad_EQ {
  private://////////////////////////////////////////////////////////////
    // FFT parameters
    float EQpeak[EQbins];              // The length of these arrays must be >= NUM_BANDS
    float EQnoisecutoff[EQbins];
    // FFT Buffers and boundaries
    float EQbuff[EQbins];    // Input buffer collects data directly from the FFT
    float EQdecay[EQbins];   // Version of FFT data that 
    float EQmaxes[EQbins];   // A moving maxiumum for each FFT band
    float EQmins[EQbins] = {9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999, 9999999};
    // Math
    Statistic EQstatstotal[EQbins];
    float EQaverage[EQbins];
    float EQstDev[EQbins];
    // Beat
    uint16_t EQbeatInterval[EQbins];
    uint16_t EQbeatIntervalOld[EQbins];
    uint16_t EQconstantBeatCounter[EQbins];
    //elapsedMillis EQbeatTimer[EQbins];
    const uint8_t ledPin =  25;
    bool ledState = LOW; 
    #ifdef ESP32
      uint16_t EQsampletimer;
      double EQreal[EQsamples];
      double EQimag[EQsamples];
      unsigned long EQtimer;
      arduinoFFT FFT = arduinoFFT(EQreal, EQimag, EQsamples, EQsamplefreq);
    #endif

  public:///////////////////////////////////////////////////////////////
    Myriad_EQ();
    void dofft();
    void printone(uint8_t target);
    void printall();
    void beatBlink();
    void noisegate();
    void updatevalues();
    void stats();
    void printDetectedBeats();
    void beatDetection();
    void beatBuckets();
    void calibration();   // Calibrate values for noisethresh() gate function
    void proc();
    void proc(bool); // overload for calibration
    void proc(bool, bool, bool, uint8_t); // (calibrate, printall, printone, target)

    // Actual usable output
    uint16_t EQscaled[EQbins];      // EQ values scaled to LEDper
    uint16_t EQ10000scaled[EQbins];   // EQ values scaled to 1000
    uint16_t EQflatdecline[EQbins]; // EQ values that decay at 1 LED every run
    uint16_t EQsummed10000;
    // Beat output
    uint8_t EQbeatDetected[EQbins];
    bool EQbeat;
};

#endif