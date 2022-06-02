#include <Myriad_Teensy_FFT.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <EEPROM.h>

//#define FFTanalog // Analog Microphone
#define FFTi2s // I2S Microphone
#define FFT256
//#define FFT1024
 
AudioMixer4         mixer;
#ifdef FFTi2s
  AudioInputI2S       i2s; // BCLK 21, MCLK 23, (RX/DOUT) 8, LRCLK 20
  AudioConnection     patchCord1(i2s, 0, mixer, 0);
  AudioConnection     patchCord2(i2s, 1, mixer, 1);
#endif
#ifdef FFTanalog
  AudioInputI2S       i2s; // BCLK 21, MCLK 23, (RX/DOUT) 8, LRCLK 20
  AudioInputAnalog    adc(A2); // Default 16/A2
  AudioConnection     patchCord1(adc, 0, mixer, 0);
  AudioConnection     patchCord2(adc, 1, mixer, 1);
#endif
#ifdef FFT256
  AudioAnalyzeFFT256  FFT;
#endif
#ifdef FFT1024
  AudioAnalyzeFFT1024  FFT;
#endif
AudioConnection     patchCord3(mixer, 0, FFT, 0);

float fftdata[EQbins];
//uint32_t sampleruntime = 1000; // How many samples to take during calibration

// Usable output
uint16_t EQscaled[EQbins];        // EQ values scaled to LEDSy
uint16_t EQ10000scaled[EQbins];   // EQ values scaled to 1000
uint16_t EQflatdecline[EQbins];   // EQ values that decay at 1 LED every run
uint16_t EQsummed10000;
// Beat output
uint8_t EQbeatDetected[EQbins];
bool EQbeat;
// FFT Buffers and bo// FFT parameters
float EQnoisefloor[EQbins];
float EQmintops[EQbins];
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
const uint8_t ledPin = 25;
extern bool ledState;
bool ledState;
uint16_t fftindex[EQbins*2];

uint16_t fftindex256[] = {
  0,0,
  1,1,
  2,3,
  4,5,
  6,8,
  9,12,
  13,17,
  18,23,
  24,31,
  32,42,
  43,56,
  57,74,
  75,97,
  98,127,
};
uint16_t fftindex1024[] = {
  0,1,
  2,2,
  3,4,
  4,5,
  6,8,
  9,12,
  18,24,
  25,34,
  35,48,
  49,67,
  68,94,
  95,131,
  132,183,
  184,511,
};

void EQsetup(){
  AudioMemory(12);
  EQcalcbins();
  //mixer.gain(0, 1);
  //mixer.gain(1, 1);

  // Get calibrated noise floor and mintops from non-volatile EEPROM
  Serial.println("Getting noise floor from EEPROM:");
  for (uint16_t i = 0; i < EQbins; i++){
    Serial.print(EEPROM.get(i*4, EQnoisefloor[i]), 16);
    if (i < (EQbins -1)) Serial.print(", ");
  }
  Serial.println();
  Serial.println("Getting min peaks from EEPROM:");
  for (uint16_t i = 0; i < EQbins; i++){
    Serial.print(EEPROM.get(i*4+EQbins*20, EQmintops[i]), 16);
    if (i < (EQbins -1)) Serial.print(", ");
  }
  Serial.println();

  #ifdef FFT256
    //FFT.windowFunction(AudioWindowHanning256);
    FFT.windowFunction(AudioWindowBlackmanHarris256);
    FFT.averageTogether(1);
  #endif
  #ifdef FFT1024
    //FFT.windowFunction(AudioWindowHanning1024);
    FFT.windowFunction(AudioWindowBlackmanHarris1024);
  #endif
}
#ifdef ESP32
void EQdofft(){
  // Reset buffer
  for (int i = 0; i < EQbins; i++){
    EQbuff[i] = 0;
  }
  
  // Sample the audio pin
  for (int i = 0; i < EQsamples; i++) {
    EQtimer = micros();
    EQreal[i] = analogRead(EQsourcepin); // A conversion takes about 9.7uS on an ESP32
    EQimag[i] = 0;
    while (micros() < (EQtimer + EQsampletimer)) {} // Do nothing waiting for new input
  }

  // Compute FFT
  FFT.DCRemoval();
  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  // Analyse FFT results
  for (int i = 2; i < (EQsamples/2); i++){       // Don't use sample 0 and only first SAMPLES/2 are usable. Each array element represents a frequency bin and its value the amplitude.
    
    //14 bands, 256 samples
    if (i <= 2)               EQbuff[0]  += (int)EQreal[i];
    if (i >= 3 && i <= 3)     EQbuff[1]  += (int)EQreal[i];
    if (i >= 4 && i <= 5)     EQbuff[2]  += (int)EQreal[i];
    if (i >= 6 && i <= 7)     EQbuff[3]  += (int)EQreal[i];
    if (i >= 8 && i <= 10)    EQbuff[4]  += (int)EQreal[i];
    if (i >= 11 && i <= 14)   EQbuff[5]  += (int)EQreal[i];
    if (i >= 15 && i <= 19)   EQbuff[6]  += (int)EQreal[i];
    if (i >= 20 && i <= 25)   EQbuff[7]  += (int)EQreal[i];
    if (i >= 26 && i <= 33)   EQbuff[8]  += (int)EQreal[i];
    if (i >= 34 && i <= 44)   EQbuff[9]  += (int)EQreal[i];
    if (i >= 45 && i <= 58)   EQbuff[10]  += (int)EQreal[i];
    if (i >= 59 && i <= 76)   EQbuff[11]  += (int)EQreal[i];
    if (i >= 77 && i <= 99)   EQbuff[12]  += (int)EQreal[i];
    if (i >= 100)             EQbuff[13]  += (int)EQreal[i];
    /*
    //14 bands 128 samples
    if (i <= 2)               EQbuff[0]  += (int)EQreal[i];
    if (i == 3)               EQbuff[1]  += (int)EQreal[i];
    if (i >= 4 && i <= 5)     EQbuff[2]  += (int)EQreal[i];
    if (i >= 6 && i <= 7)     EQbuff[3]  += (int)EQreal[i];
    if (i >= 8 && i <= 10)    EQbuff[4]  += (int)EQreal[i];
    if (i >= 11 && i <= 14)   EQbuff[5]  += (int)EQreal[i];
    if (i >= 15 && i <= 19)   EQbuff[6]  += (int)EQreal[i];
    if (i >= 20 && i <= 25)   EQbuff[7]  += (int)EQreal[i];
    if (i >= 26 && i <= 33)   EQbuff[8]  += (int)EQreal[i];
    if (i >= 34 && i <= 43)   EQbuff[9]  += (int)EQreal[i];
    if (i >= 44 && i <= 57)   EQbuff[10]  += (int)EQreal[i];
    if (i >= 58 && i <= 75)   EQbuff[11]  += (int)EQreal[i];
    if (i >= 76 && i <= 98)   EQbuff[12]  += (int)EQreal[i];
    if (i >= 99)              EQbuff[13]  += (int)EQreal[i];
    */
     /*
    //14 bands, 512 samples
    if (i <= 2)               EQbuff[0]  += (int)EQreal[i];
    if (i == 3)               EQbuff[1]  += (int)EQreal[i];
    if (i >= 4 && i <= 5)     EQbuff[2]  += (int)EQreal[i];
    if (i >= 6 && i <= 8)     EQbuff[3]  += (int)EQreal[i];
    if (i >= 9 && i <= 12)    EQbuff[4]  += (int)EQreal[i];
    if (i >= 13 && i <= 17)   EQbuff[5]  += (int)EQreal[i];
    if (i >= 18 && i <= 24)   EQbuff[6]  += (int)EQreal[i];
    if (i >= 25 && i <= 34)   EQbuff[7]  += (int)EQreal[i];
    if (i >= 35 && i <= 48)   EQbuff[8]  += (int)EQreal[i];
    if (i >= 49 && i <= 67)   EQbuff[9]  += (int)EQreal[i];
    if (i >= 68 && i <= 94)   EQbuff[10]  += (int)EQreal[i];
    if (i >= 95 && i <= 131)  EQbuff[11]  += (int)EQreal[i];
    if (i >= 132 && i <= 183) EQbuff[12]  += (int)EQreal[i];
    if (i >= 184)             EQbuff[13]  += (int)EQreal[i];
    
    *//*
    //14 bands, 12kHz top band
    if (i <= 2)               EQbuff[0]  += (int)EQreal[i];
    if (i == 3)               EQbuff[1]  += (int)EQreal[i];
    if (i == 4)               EQbuff[2]  += (int)EQreal[i];
    if (i >= 5 && i <= 6)     EQbuff[3]  += (int)EQreal[i];
    if (i >= 7 && i <= 11)    EQbuff[4]  += (int)EQreal[i];
    if (i >= 12 && i <= 18)   EQbuff[5]  += (int)EQreal[i];
    if (i >= 19 && i <= 29)   EQbuff[6]  += (int)EQreal[i];
    if (i >= 30 && i <= 45)   EQbuff[7]  += (int)EQreal[i];
    if (i >= 46 && i <= 68)   EQbuff[8]  += (int)EQreal[i];
    if (i >= 69 && i <= 103)  EQbuff[9]  += (int)EQreal[i];
    if (i >= 104 && i <= 154) EQbuff[10]  += (int)EQreal[i];
    if (i >= 155 && i <= 230) EQbuff[11]  += (int)EQreal[i];
    if (i >= 231 && i <= 343) EQbuff[12]  += (int)EQreal[i];
    if (i >= 344)             EQbuff[13]  += (int)EQreal[i];
    */  
  }
}
#endif
#ifdef TEENSY
void EQdofft(){
  for(uint16_t i = 0; i < EQbins; i++){
    #ifdef FFT256
      //uint16_t binDelta = fftindex256[i*2];
      EQbuff[i] = FFT.read(fftindex256[i*2], fftindex256[i*2+1]);
    #endif
    #ifdef FFT1024
      //uint16_t binDelta = fftindex1024[i*2];
      EQbuff[i] = FFT.read(fftindex1024[i*2], fftindex1024[i*2+1]);
    #endif
  }
}
#endif

void EQnoisegate(){
  extern const uint16_t LEDSy;      // I can't seem to pass the LEDSy define from config to this library

  // 1024 Adafruit
  //float noisefloor[EQbins] = {0.027404785156250,  0.018676757812500,  0.016479492187500,  0.016235351562500,  0.010742187500000,  0.005859375000000,  0.004150390625000,  0.003051757812500,  0.002319335937500,  0.002380371093750,  0.002319335937500,  0.002624511718750,  0.001770019531250,  0.001037597656250};
  //float mintops[EQbins] =     { 0.066284179687500,  0.060791015625000,  0.057189941406250,  0.050842285156250,  0.034362792968750,  0.028076171875000,  0.019897460937500,  0.009948730468750,  0.005493164062500,  0.009094238281250,  0.005554199218750,  0.005859375000000,  0.005310058593750,  0.006408691406250};
  
  // 1024 Randanalog
  //float noisefloor[EQbins] = {};
  //float mintops[EQbins] =     {};
  
  // 1024 I2S
  //float noisefloor[EQbins] = {0.046447753906250,  0.029479980468750,  0.029602050781250,  0.031677246093750,  0.019287109375000,  0.011352539062500,  0.005004882812500,  0.004943847656250,  0.001220703125000,  0.001953125000000,  0.003234863281250,  0.001586914062500,  0.002807617187500,  0.001403808593750};
  //float mintops[EQbins] =     {0.064514160156250,  0.051330566406250,  0.047119140625000,  0.045227050781250,  0.025390625000000,  0.015136718750000,  0.006896972656250,  0.007141113281250,  0.002197265625000,  0.002746582031250,  0.004028320312500,  0.001953125000000,  0.003784179687500,  0.001892089843750};
  for(int i = 0; i < EQbins; i++){
    //uint32_t x = _max(noisefloor[i], EQmins[i]);
    //uint32_t y = _max(mintops[i], );
    float a = _max(EQnoisefloor[i], EQmins[i]);             // Use the highest min value coded or caluclated
    float x = _max(a, 0); // Use the highest min value coded, calculated, or 2.5 std devs away from average  uint32_t x = max(a, (EQaverage[i] - (2.5 *EQstDev[i]))); // Use the highest min value coded, calculated, or 2.5 std devs away from average
    float b = _max(EQmintops[i], EQmaxes[i]);                // Use the highest max value coded or calculated
    float y = _max(b, (EQaverage[i] + (2.5 *EQstDev[i]))); // Use the highest max value coded, calculated, or 2.5 std devs away from average
    //uint32_t z = _max(EQbuff[i], EQdecay[i]);
    float z = _max(EQbuff[i], 0);

    if(z >= EQnoisefloor[i]){
      EQscaled[i] = map(z, x, y, 0, LEDSy);  //(input, inmin, inmax, outmin, outmax)
      EQ10000scaled[i] = map(z, x, y, 0, 10000);
      EQsummed10000 += EQ10000scaled[i];
    } else {
      EQscaled[i] = 0;        // Make sure to true 0 the scaled bands
      EQ10000scaled[i] = 0;
    }
    if(EQscaled[i] >= EQflatdecline[i]){ // If the scaled value is now higher than the flat decline, up the flat decline value
      EQflatdecline[i] = EQscaled[i];
    }
  }
  EQupdatevalues();
}

void EQstats(){
  float ultralow = 0.000000000000001;
  for(int i = 0; i < EQbins; i++){    
    if( EQbuff[i] >= EQdecay[i]){              // Reset decaying value to smooth out drops
      EQdecay[i] = EQbuff[i];
    } else {
      EQdecay[i] = EQdecay[i] - _max(ultralow, EQdecay[i] * .01);  // Decay must be relative to the frequency, could change to flat value
    }

    if( EQbuff[i] >= EQmaxes[i] ){             // Set maximum volume level for scaling       
      EQmaxes[i] = EQbuff[i];
    } else {
      EQmaxes[i] = EQmaxes[i] - _max(.1, EQmaxes[i] * .01);        // need to make this based on std deviation
    }

    if( EQbuff[i] <= EQmins[i] ){             // Set minimum volume level for scaling   
      EQmins[i] = EQbuff[i];
    } else {
      EQmins[i] = EQmins[i] + _max(ultralow, EQaverage[i]*ultralow);    // Can't be zero        // need to make this based on std deviation
    }

    EQstatstotal[i].add(EQbuff[i]);  // Take stats for beat detection
    EQaverage[i] = EQstatstotal[i].average();
    EQstDev[i] = EQstatstotal[i].pop_stdev();
  }
}

void EQupdatevalues(){
  EVERY_N_MILLIS(EQdeclineint){
    for(int i = 0; i < EQbins; i++){
      if(EQflatdecline[i] > 0) {
        //EQscaled[i] = EQflatdecline[i];
        EQflatdecline[i]--;
      }
      if(EQ10000scaled[i] > 10){
        EQ10000scaled[i] -= 10;
      } else {
        EQ10000scaled[i] = 0;
      }
    }
  }
}

void EQprintone(uint8_t target){ // Print detailed info on a single band
  Serial.print("EQbuff: ");
  Serial.println(EQbuff[target], 6);
  Serial.print("EQmins: ");
  Serial.println(EQmins[target], 6);
  Serial.print("EQmaxes: ");
  Serial.println(EQmaxes[target], 6);
  Serial.print("EQscaled: ");
  Serial.println(EQscaled[target]);
  Serial.print("EQdecay: ");
  Serial.println(EQdecay[target], 6);
  Serial.print("EQflatdecline: ");
  Serial.println(EQflatdecline[target], 6);
  Serial.println();
}

void EQprintallbuff(){
  for(int i = 0; i < EQbins; i++){
    Serial.print(EQbuff[i], 6);
    if(i < EQbins -1) Serial.print(", ");
  }
  Serial.println();
}

void EQprintallscaled(){
  for(int i = 0; i < EQbins; i++){
    Serial.printf("%2d ", EQscaled[i]);
  }
  Serial.println();
}

void EQprintflatdecline(){
  for(int i = 0; i < EQbins; i++){
    Serial.printf("%2d ", EQflatdecline[i]);
  }
  Serial.println();
}

void EQbeatBlink(){
  if (EQbeat == true){
    ledState = HIGH;
  } else {
    ledState = LOW;
  }
  digitalWrite(ledPin, ledState);
}

//uint32_t a = (255 - EQ7avg[s])/2; // How much handicapping should be applied based on how low avg is, increase divisor to decrease strength
//int fq = 255*(log(1+(EQbuff[target]/255)*a)/log(1+a));  // Apply handicapping 
//EQ7fqamp[s] = map(fq, 0, 255, 0, LEDSy - 1);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EQprintDetectedBeats() {
  for(int i = 0; i < EQbins; i++){
    // These print statements will print the beat status of each bin
    if (i == EQbins - 1) {
      Serial.println(EQbeatDetected[i]);
    }
    else {
      Serial.print(EQbeatDetected[i]);
      Serial.print("\t");
    }
  }
}

void EQbeatDetection() {
  for(int i = 0; i < EQbins; i++){
    // 0 = no beat detected
    // 1 = beat hasn't reset yet
    // 2 = beat detected
    // Beat is detected here. Must be greater than the average+(2.3*st.dev) and greater than 0.004212 which may need to be adjusted
    if (EQbuff[i] > EQaverage[i] + 2.5 * EQstDev[i] && EQbuff[i] > 0.004212) {
      if (EQbeatDetected[i] == 2){
        EQbeatDetected[i] = 1;
      }
      if (EQbeatDetected[i] == 0){
        EQbeatDetected[i] = 2;
      }
      // if it's == 1, it stays 1.
    }
    // This is where 1's get reset to 0. If beat is not detected in that frequency, set it's status to 0.
    else {
      if (EQbeatDetected[i] == 2){ // not entirely sure why this is here
        EQbeatDetected[i] = 1;
      }
      if (EQbeatDetected[i] == 1) {
        // else, don't be ready for the next beat until the frequency has hit the running average.
        // This prevents multiple beats being triggered when they aren't well defined
        if (EQbuff[i] <= EQaverage[i]){
          EQbeatDetected[i] = 0;
        }
      }
    }
  }
}

void EQbeatBuckets(){
  uint8_t sum = 0;
  bool hit = false;
  EQbeat = false;
  for(int i = 0; i < 4; i++){
    sum = sum + EQbeatDetected[i];
    if(EQbeatDetected[i] == 2){
      hit = true;
    }
  }
  if(sum >= 2 && hit == true){
    EQbeat = true;
    //Serial.println("beat");
  } else {
   //Serial.println("nope");
  }
}

void EQcalibration(){   // Calibrate values for noisefloor() gate function
  if (millis() > 5000){   // Wait 5 seconds before running to allow electrical and mechanical noise to settle
    // Counter for sampling run triggers each of the 5 phases:
    // Gather data for noise floor -> save noise floor -> gather data for min peaks -> save min peaks -> print info
    static uint32_t calibcounter;   
    static bool initmins = false;    // Have to set a flag to initialize the maximum values to something high
    static float calibmins[EQbins];   // Store minimum values
    static float calibmaxes[EQbins];    // Store maxiumum values
    calibcounter++;     // Increase the counter
    if(initmins == false){
      Serial.println("Beginning calibration. \nFirst, set noise floor by being silent until notified.");
      for (uint8_t i = 0; i < EQbins; i++){
        calibmins[i] = 3.4028235E+38f;      // Initialize all mins to highest possible value
      }   
      initmins = true;               // Only initialize once
    }

    // Gather noise floor data
    if(calibcounter < sampleruntime/2){
      for(uint8_t i = 0; i < EQbins; i++){
        if(EQbuff[i] < calibmins[i]){   // If read value is below current minimum
          calibmins[i] = EQbuff[i];     // Set that value to new minimum
        }
        if(EQbuff[i] > calibmaxes[i]){
          calibmaxes[i] = EQbuff[i];
        }
      }
    }
    // Save noise floor data to EEPROM
    else if (calibcounter == sampleruntime/2){
      Serial.println("Noise floor saved to EEPROM");
      for (uint16_t i = 0; i < EQbins; i++){
        Serial.print(EEPROM.put(i*4, calibmaxes[i]), 16);
        if (i < (EQbins -1)) Serial.print(", ");
      }
      Serial.println();
      Serial.println("Make a moderate amount of noise to set minimum noise peaks.");
      delay(3000);
    }
    // Gather min peak data
    else if(calibcounter > sampleruntime/2 && calibcounter < sampleruntime){
      for(uint8_t i = 0; i < EQbins; i++){
        if(EQbuff[i] < calibmins[i]){   // If read value is below current minimum
          calibmins[i] = EQbuff[i];     // Set that value to new minimum
        }
        if(EQbuff[i] > calibmaxes[i]){
          calibmaxes[i] = EQbuff[i];
        }
      }
    }
    // Save min peaks
    else if (calibcounter == sampleruntime){
      Serial.println("Min peaks saved to EEPROM");
      for (uint16_t i = 0; i < EQbins; i++){
        Serial.print(EEPROM.put(i*4+EQbins*20, calibmaxes[i]), 16);
        if (i < (EQbins -1)) Serial.print(", ");
      }
      Serial.println();
      Serial.println("Sampling complete");
    }
    /////////////////////// Save to EEPROM and print to serial ////////////////////////////////
    else if(calibcounter  ==  sampleruntime+1){
      Serial.println("Calibration complete. The following values are saved to EEPROM");
      Serial.println("Noise floor from EEPROM");
      for (uint16_t i = 0; i < EQbins; i++){
        Serial.print(EEPROM.get(i*4, EQnoisefloor[i]), 16);
        if (i < (EQbins -1)) Serial.print(", ");
      }
      Serial.println();
      Serial.println("Minimum peaks from EEPROM");
      for (uint16_t i = 0; i < EQbins; i++){
        Serial.print(EEPROM.get(i*4+EQbins*20, EQmintops[i]), 16);
        if (i < (EQbins -1)) Serial.print(", ");
      }
      Serial.println();
    }
  }
}

float EQfindE(int bands, int bins) {
  float increment=0.1, eTest, n;
  int b, count, d;

  for (eTest = 1; eTest < bins; eTest += increment) {     // Find E through brute force calculations
    count = 0;
    for (b = 0; b < bands; b++) {                         // Calculate full log values
      n = pow(eTest, b);
      d = int(n + 0.5);
      count += d;
    }
    if (count > bins) {     // We calculated over our last bin
      eTest -= increment;   // Revert back to previous calculation increment
      increment /= 10.0;    // Get a finer detailed calculation & increment a decimal point lower
    }
    else
      if (count == bins)    // We found the correct E
        return eTest;       // Return calculated E
    if (increment < 0.0000001)        // Ran out of calculations. Return previous E. Last bin will be lower than (bins-1)
      return (eTest - increment);
  }
  return 0;                 // Return error 0
}

void EQcalcbins(){ // Calculates the boundaries of each EQbin with EQbins, and EQ samples
  float e, n;
  int b, count=0, d;  // bands is EQ bins and bins is EQ samples 

  e = EQfindE(EQbins, EQsamples);         // Find calculated E value
  if (e) {                                // If a value was returned continue
    Serial.printf("E = %4.4f\n", e);      // Print calculated E value
    for (b = 0; b < EQbins; b++) {         // Test and print the bins from the calculated E
      n = pow(e, b);
      d = int(n + 0.5);
      fftindex[b*2] = count;
      Serial.print(fftindex[b*2]);
      Serial.print(", ");
      //Serial.printf( "%4d ", count);      // Print low bin
      count += d - 1;
      fftindex[b*2+1] = count;
      Serial.println(fftindex[b*2+1]);
      //Serial.printf( "%4d\n", count);     // Print high bin
      ++count;
    }
  }
  else {
    Serial.println("Error\n");            // Error, something is wrong with 'e'
  }
}

void EQproc(){
  EVERY_N_MILLIS(EQreadint){
    EQdofft();            // get data
    EQstats();          // do the math
    EQnoisegate();      // apply filters
    EQupdatevalues();
    EQbeatDetection();  // find beat
    EQbeatBuckets();    // also find beat, i guess
    EQbeatBlink();    // basic blink on builtinled
  }
  EQupdatevalues();
}

void EQproc(bool calib){
  EVERY_N_MILLIS(10){       // uniform sample rate so calibration doesn't take forever
    EQdofft();          // get data
    EQstats();          // do the math
    EQnoisegate();      // apply filters
    EQupdatevalues();   // apply decay and move data from buffer to usable values
    EQbeatDetection();  // find beat
    EQbeatBuckets();    // also find beat, i guess
    EQcalibration();    // run this to set maxes and mins
    //EQbeatBlink();    // basic blink on builtinled
  }
  EQupdatevalues();
}

void EQproc(byte printmany, bool printone, uint8_t target){ // (1=buffer 2=scaled 3= flatdecline, printone, target for printone)
  EVERY_N_MILLIS(EQreadint){
    EQdofft();          // get data
    if(printmany == 1){
      EQprintallbuff();       // print raw data for all bands
    }
    EQstats();          // do the math
    EQnoisegate();      // apply filters
    EQupdatevalues();   // apply decay and move data from buffer to usable values
    EQbeatDetection();  // find beat
    EQbeatBuckets();    // also find beat, i guess
    //EQbeatBlink();    // basic blink on builtinled
    if(printmany == 2){
      EQprintallscaled();       // print raw data for all bands
    }
    if(printmany == 3){
      EQprintflatdecline();       // print raw data for all bands
    }
    if(printone == true){
      EQprintone(target); // print detailed output for a single band
    }
  }
}

/*
void beatTiming(int i){
  //int beatTimerStandin = beatTimer[i]; 
  if (EQbeatDetected[i] == 2) { // if beat has been detected
    if (EQbeatTimer[i] < 200){ // and it isn't 200 ms from the last one
      EQbeatDetected[i] = 1; // but if it is, cancel the beat
    }
    else {
      EQbeatInterval[i] = EQbeatTimer[i];  // a beat has been detected. Get the time since last beat.
      // could use absolute value below
      if ((EQbeatIntervalOld[i] - EQbeatInterval[i]) < 30 && (EQbeatIntervalOld[i] - EQbeatInterval[i]) > -30){ // If the time between the new interval and old interval is less than 30 ms 
        EQconstantBeatCounter[i]++;
      }
      EQbeatTimer[i] = 0;
      EQbeatIntervalOld[i] = EQbeatInterval[i];
    }
  }
  
  if (EQconstantBeatCounter[i] > 0 && EQbeatTimer[i] > EQbeatInterval[i] + 50){
    EQconstantBeatCounter[i] = 0; // clears beat counter when more time than the beat has passed
  }

  // These print statements will print the constant beat counter of each bin
  if (i == EQbins - 1){
    Serial.println(EQconstantBeatCounter[i]);
  }
  else {
    Serial.print(EQconstantBeatCounter[i]);
    Serial.print("\t");
  }

  if ( i < 4) {
    if (EQconstantBeatCounter[i] > EQmaxConstBeat) {
      EQmaxConstBeat = EQconstantBeatCounter[i];
      EQconstBeatBin = i;
    }
  }
  if (EQmaxConstBeat > 10){
    EQconstBeat = true;
  }
  else {
    EQconstBeat = false;
  }
}
    */