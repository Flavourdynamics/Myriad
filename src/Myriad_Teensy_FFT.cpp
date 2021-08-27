#include <Myriad_Teensy_FFT.h>

Myriad_Teensy_FFT::Myriad_Teensy_FFT(){
  AudioMemory(12);
  this->FFT.windowFunction(AudioWindowHanning1024);   //myFFT.windowFunction(NULL);
  this->FFT.averageTogether(1);
  //fftdata[13];
  AudioConnection patchCord1(this->i2s, 0, this->mixer, 0);
  AudioConnection patchCord2(this->i2s, 1, this->mixer, 1);
  AudioConnection patchCord3(this->mixer, 0, this->FFT, 0);
}

void Myriad_Teensy_FFT::teensyfft(){
  if (FFT.available()) {
    Serial.println();
    fftdata[0] = FFT.read(0,0);
    fftdata[1] = FFT.read(1,1);
    fftdata[2] = FFT.read(2,3);
    fftdata[3] = FFT.read(4,5);
    fftdata[4] = FFT.read(6,8);
    fftdata[5] = FFT.read(9,12);
    fftdata[6] = FFT.read(13,17);
    fftdata[7] = FFT.read(18,23);
    fftdata[8] = FFT.read(24,31);
    fftdata[9] = FFT.read(32,42);
    fftdata[10] = FFT.read(43,56);
    fftdata[11] = FFT.read(57,74);
    fftdata[12] = FFT.read(75,97);
    fftdata[13] = FFT.read(98,127);
  }
}

void Myriad_Teensy_FFT::teensyprint(){
  for(int i = 0; i < 14; i++){
    Serial.print(fftdata[i]);
    Serial.print("  ");
  }
  Serial.println();
}