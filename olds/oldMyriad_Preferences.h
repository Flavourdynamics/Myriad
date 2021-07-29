#ifndef Myriad_Preferences_h
#define Myriad_Preferences_h

#include "Preferences.h"  // Library for storing preferences between sessions, functions below
Preferences preferences; // Create preferences object

void preferences_setup(){
  preferences.begin("my-app", false); // RW-mode (second parameter has to be false).
  //preferences.clear();
  //preferences.remove("counter"); // remove specific preference
  LEDtargbright = preferences.getUInt("LEDtargbright", 15);
  patternum = preferences.getUInt("patternum", 0);
  palnum = preferences.getUInt("palettenum", 0);
  STATEloopinterval = preferences.getUInt("loopint", 8);
  STATEpatshuffleinterval = preferences.getUInt("shuffleint", 10);
  STATEpalshuffleinterval = preferences.getUInt("palshuffleint", 10);
  patshuffle = preferences.getBool("patshuffle", false);
  palshuff = preferences.getBool("palshuff", false);
  palmatch = preferences.getBool("palmatch", false);
  //preferences.end();
}

void preferences_update(){
  EVERY_N_SECONDS(30){
    //Serial.println("Updating preferences");
    preferences.putUInt("LEDtargbright", LEDtargbright);
    preferences.putUInt("patternum", patternum);
    preferences.putUInt("palettenum", palnum);
    preferences.putUInt("loopint", STATEloopinterval);
    preferences.putUInt("shuffleint", STATEpatshuffleinterval);
    preferences.putUInt("palshuffleint", STATEpalshuffleinterval);
    preferences.putBool("patshuffle", patshuffle);
    preferences.putBool("palshuff", palshuff);
    preferences.putBool("palmatch", palmatch);
  }
}

#endif
/*
  Char	          putChar(const char* key, int8_t value)
  Unsigned Char	  putUChar(const char* key, int8_t value)
  Short	          putShort(const char* key, int16_t value)
  Unsigned Short	putUShort(const char* key, uint16_t value)
  Int	            putInt(const char* key, int32_t value)
  Unsigned Int	  putUInt(const char* key, uint32_t value)
  Long	          putLong(const char* key, int32_t value)
  Unsigned Long	  putULong(const char* key, uint32_t value)
  Long64	        putLong64(const char* key, int64_t value)
  Unsigned Long64	putULong64(const char* key, uint64_t value)
  Float	          putFloat(const char* key, const float_t value)
  Double	        putDouble(const char* key, const double_t value)
  Bool	          putBool(const char* key, const bool value)
  String	        putString(const char* key, const String value)
  Bytes	          putBytes(const char* key, const void* value, size_t len)

  Char	          getChar(const char* key, const int8_t defaultValue)
  Unsigned Char	  getUChar(const char* key, const uint8_t defaultValue)
  Short	          getShort(const char* key, const int16_t defaultValue
  Unsigned Short	getUShort(const char* key, const uint16_t defaultValue)
  Int	            getInt(const char* key, const int32_t defaultValue)
  Unsigned Int	  getUInt(const char* key, const uint32_t defaultValue)
  Long	          getLong(const char* key, const int32_t defaultValue)
  Unsigned Long	  getULong(const char* key, const uint32_t defaultValue)
  Long64	        getLong64(const char* key, const int64_t defaultValue)
  Unsigned Long64	gettULong64(const char* key, const uint64_t defaultValue)
  Float	          getFloat(const char* key, const float_t defaultValue)
  Double	        getDouble(const char* key, const double_t defaultValue)
  Bool	          getBool(const char* key, const bool defaultValue)
  String	        getString(const char* key, const String defaultValue)
  String	        getString(const char* key, char* value, const size_t maxLen)
  Bytes	          getBytes(const char* key, void * buf, size_t maxLen)

// Reset namespaces
#include <nvs_flash.h>

void setup() {
  nvs_flash_erase(); // erase the NVS partition and...
  nvs_flash_init(); // initialize the NVS partition.
  while(true);
}
*/