#include <Myriad_Teensy_FFT.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
AudioInputI2S       i2s; // BCLK 21, MCLK 23, (RX/DOUT) 8, LRCLK 20
AudioInputAnalog    adc; // Default 16/A2
AudioMixer4         mixer;
AudioAnalyzeFFT1024  FFT;
//AudioAnalyzeFFT256  FFT;
AudioConnection     patchCord1(adc, 0, mixer, 0);
//AudioConnection     patchCord1(i2s, 0, mixer, 0);
//AudioConnection     patchCord2(i2s, 1, mixer, 1);
AudioConnection     patchCord3(mixer, 0, FFT, 0);

float fftdata[EQbins];
// Usable output
uint16_t EQscaled[EQbins];      // EQ values scaled to LEDper
uint16_t EQ10000scaled[EQbins];   // EQ values scaled to 1000
uint16_t EQflatdecline[EQbins]; // EQ values that decay at 1 LED every run
uint16_t EQsummed10000;
// Beat output
uint8_t EQbeatDetected[EQbins];
bool EQbeat;
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
const uint8_t ledPin = 25;
bool ledState;

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

extern bool ledState;

void EQsetup(){
  AudioMemory(12);
  mixer.gain(0, 2);
  mixer.gain(1, 2);
  //FFT.windowFunction(AudioWindowHanning256);
  FFT.windowFunction(AudioWindowBlackmanHarris256);
  //FFT.windowFunction(AudioWindowHanning1024);
  //FFT.windowFunction(AudioWindowBlackmanHarris1024);
  FFT.averageTogether(50);
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
    uint16_t binDelta = fftindex1024[i*2];
    //uint16_t binDelta = fftindex256[i*2];
    EQbuff[i] = FFT.read(binDelta, binDelta+1);
  }
}
#endif

// Mins:  0.331848144531250,  0.000122070312500,  0.000000000000000,  0.000061035156250,  0.000122070312500,  0.000915527343750,  0.000122070312500,  0.000061035156250,  0.000122070312500,  0.000122070312500,  0.000000000000000,  0.000000000000000,  0.000061035156250,  0.000061035156250
// Maxes:  0.382812500000000,  0.267272949218750,  0.285217285156250,  0.187377929687500,  0.057800292968750,  0.048522949218750,  0.034667968750000,  0.015991210937500,  0.016113281250000,  0.008361816406250,  0.011413574218750,  0.004821777343750,  0.001770019531250,  0.000671386718750
void EQnoisegate(){
  extern const uint16_t LEDper;      // I can't seem to pass the LEDper define from config to this library

  float noisethresh[EQbins] = {0.539001464843750,  0.282653808593750,  0.069396972656250,  0.000366210937500,  0.000305175781250,  0.000244140625000,  0.000244140625000,  0.001892089843750,  0.000183105468750,  0.000183105468750,  0.000122070312500,  0.000122070312500,  0.000000000000000,  0.000000000000000};
  float mintops[EQbins] =     {0.146667480468750,  0.085449218750000,  0.028625488281250,  0.004211425781250,  0.002197265625000,  0.001831054687500,  0.002624511718750,  0.000976562500000,  0.000244140625000,  0.000183105468750,  0.000061035156250,  0.000244140625000,  0.000000000000000,  0.000000000000000};
  //float noisethresh[EQbins] = {0.063232421875000,  0.000122070312500,  0.000122070312500,  0.000122070312500,  0.000122070312500,  0.000122070312500,  0.000000000000000,  0.000000000000000,  0.000061035156250,  0.000061035156250,  0.000000000000000,  0.000000000000000,  0.000000000000000,  0.000000000000000};
  //float mintops[EQbins] =     {0.096313476562500,  0.020080566406250,  0.019653320312500,  0.013305664062500,  0.008850097656250,  0.011352539062500,  0.005615234375000,  0.008605957031250,  0.001831054687500,  0.001831054687500,  0.002563476562500,  0.001342773437500,  0.001647949218750,  0.002136230468750};
  for(int i = 0; i < EQbins; i++){
    //uint32_t x = _max(noisethresh[i], EQmins[i]);
    //uint32_t y = _max(mintops[i], );
    float a = _max(noisethresh[i], EQmins[i]);             // Use the highest min value coded or caluclated
    float x = _max(a, 0); // Use the highest min value coded, calculated, or 2.5 std devs away from average  uint32_t x = max(a, (EQaverage[i] - (2.5 *EQstDev[i]))); // Use the highest min value coded, calculated, or 2.5 std devs away from average
    float b = _max(mintops[i], EQmaxes[i]);                // Use the highest max value coded or calculated
    float y = _max(b, (EQaverage[i] + (2.5 *EQstDev[i]))); // Use the highest max value coded, calculated, or 2.5 std devs away from average
    //uint32_t z = _max(EQbuff[i], EQdecay[i]);
    float z = _max(EQbuff[i], 0);

    if(z >= noisethresh[i]){
      EQscaled[i] = map(z, x, y, 0, LEDper);  //(input, inmin, inmax, outmin, outmax)
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

void EQprintone(uint8_t target){
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
//EQ7fqamp[s] = map(fq, 0, 255, 0, LEDper - 1);

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
      if (EQbeatDetected[i] == 2){
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

void EQcalibration(){   // Calibrate values for noisethresh() gate function
  if (millis() > 5000){   // Wait 5 seconds before running to allow electrical and mechanical noise to settle
    static uint32_t calibcounter;   // Persistent counter for sampling run
    uint32_t sampleruntime = 3000; // How many samples to take
    static bool initmins = false;    // Have to set a flag to initialize the maximum values to something high
    static float calibmins[EQbins];   // Store minimum values  
    calibcounter++;     // Increase the counter
    if(initmins == false){     
      for (uint8_t i = 0; i < EQbins; i++){
        calibmins[i] = 3.4028235E+38f;      // Initialize all mins to highest possible value
      }   
      initmins = true;               // Only initialize once
    }
    static float calibmaxes[EQbins];    // Store maxiumum values

    // Gather data
    if(calibcounter < sampleruntime){
      for(uint8_t i = 0; i < EQbins; i++){
        if(EQbuff[i] < calibmins[i]){   // If read value is below current minimum
          calibmins[i] = EQbuff[i];     // Set that value to new minimum
        }
        if(EQbuff[i] > calibmaxes[i]){
          calibmaxes[i] = EQbuff[i];
        }
      }
      EVERY_N_MILLIS(1000){
        Serial.print("Calibration listen. Sample: ");
        Serial.println(calibcounter);
      }
    }
    // Print data
    if(calibcounter >= sampleruntime){    // If we've taken enough samples
      EVERY_N_MILLIS(1000){
        Serial.print("// Mins:");
        for(uint8_t i = 0; i < EQbins; i++){    // Print the mins
          Serial.print("  ");
          //Serial.print(i);
          Serial.print(calibmins[i],16);
          Serial.print(",");   
        }
        Serial.println();

        Serial.print("// Maxes:");
        for(uint8_t i = 0; i < EQbins; i++){    // Print the maxes
          Serial.print("  ");
          //Serial.print(i);
          Serial.print(calibmaxes[i],16);
          Serial.print(",");
        }
        Serial.println();
      }
    }
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
    EQupdatevalues();
    EQbeatDetection();  // find beat
    EQbeatBuckets();    // also find beat, i guess
    EQcalibration();  // run this to set maxes and mins
    //EQbeatBlink();    // basic blink on builtinled
  }
  EQupdatevalues();
}

void EQproc(byte printmany, bool printone, uint8_t target){
  EVERY_N_MILLIS(EQreadint){
    EQdofft();          // get data
    if(printmany == 1){
      EQprintallbuff();       // print raw data for all bands
    }
    EQstats();          // do the math
    EQnoisegate();      // apply filters
    EQupdatevalues();
    EQbeatDetection();  // find beat
    EQbeatBuckets();    // also find beat, i guess
    //EQbeatBlink();    // basic blink on builtinled
    EVERY_N_MILLIS(100){
      if(printmany == 2){
        EQprintallscaled();       // print raw data for all bands
      }
      if(printone == true){
        EQprintone(target); // print detailed output for a single band
      }
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
    
/*
 * // Process the FFT data into bar heights
  for (byte band = 0; band < NUM_BANDS; band++) {
    
    // Scale the bars for the display
    int barHeight = bandValues[band] / AMPLITUDE;
    if (barHeight > TOP) barHeight = TOP;

    // Small amount of averaging between frames
    barHeight = ((oldBarHeights[band] * 1) + barHeight) / 2;

    // Move peak up
    if (barHeight > peak[band]) {
      peak[band] = min(TOP, barHeight);
    }


    EVERY_N_SECONDS(10){
      randle = random8(6);
    }
    switch (randle) {
      FastLED.clear();
      case 0:
        rainbowBars(band, barHeight);
        break;
      case 1:
        // No bars on this one
        break;
      case 2:
        purpleBars(band, barHeight);
        break;
      case 3:
        centerBars(band, barHeight);
        break;
      case 4:
        changingBars(band, barHeight);
        break;
      case 5:
        waterfall(band);
        break;
    }

    // Draw peaks
    switch (randle) {
      case 0:
        whitePeak(band);
        break;
      case 1:
        outrunPeak(band);
        break;
      case 2:
        whitePeak(band);
        break;
      case 3:
        // No peaks
        break;
      case 4:
        // No peaks
        break;
      case 5:
        // No peaks
        break;
    }

    // Save oldBarHeights for averaging later
    oldBarHeights[band] = barHeight;
  }
  
  // Decay peak
  EVERY_N_MILLISECONDS(60) {
    for (byte band = 0; band < NUM_BANDS; band++) 
      if (peak[band] > 0) peak[band] -= 1;
    colorTimer++;
  }

  // Used in some of the patterns
  EVERY_N_MILLISECONDS(10) {
    colorTimer++;
  }
  
  FastLED.show();
}

// PATTERNS BELOW //

void rainbowBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds[XY(x,y)] = CHSV((x / BAR_WIDTH) * (255 / NUM_BANDS), 255, 255); 
    }
  }
}

void purpleBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds[XY(x,y)] = ColorFromPalette(purplePal, y * (255 / barHeight));
    }
  }
}

void changingBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    for (int y = 0; y < barHeight; y++) {
      leds[XY(x,y)] = CHSV(y * (255 / kMatrixHeight) + colorTimer, 255, 255); 
    }
  }
}

void centerBars(int band, int barHeight) {
  int xStart = BAR_WIDTH * band;
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    if (barHeight % 2 == 0) barHeight--;
    int yStart = ((kMatrixHeight - barHeight) / 2 );
    for (int y = yStart; y <= (yStart + barHeight); y++) {
      int colorIndex = constrain((y - yStart) * (255 / barHeight), 0, 255);
      leds[XY(x,y)] = ColorFromPalette(heatPal, colorIndex);
    }
  }
}

void whitePeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = peak[band];
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    leds[XY(x,peakHeight)] = CRGB::White;
  }
}

void outrunPeak(int band) {
  int xStart = BAR_WIDTH * band;
  int peakHeight = peak[band];
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    //leds[XY(x,peakHeight)] = CHSV(peakHeight * (255 / kMatrixHeight), 255, 255);
    leds[XY(x,peakHeight)] = ColorFromPalette(outrunPal, peakHeight * (255 / kMatrixHeight));
  }
}

void waterfall(int band) {
  int xStart = BAR_WIDTH * band;
  double highestBandValue = 60000;        // Set this to calibrate your waterfall

  // Draw bottom line
  for (int x = xStart; x < xStart + BAR_WIDTH; x++) {
    leds[XY(x,0)] = CHSV(constrain(map(bandValues[band],0,highestBandValue,160,0),0,160), 255, 255);
  }

  // Move screen up starting at 2nd row from top
  if (band == NUM_BANDS - 1){
    for (int y = kMatrixHeight - 2; y >= 0; y--) {
      for (int x = 0; x < kMatrixWidth; x++) {
        leds[XY(x,y+1)] = leds[XY(x,y)];
      }
    }
  }
}
*/