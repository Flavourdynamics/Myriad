#ifndef Myriad_Display_h
#define Myriad_Display_h

#include "Adafruit_GFX.h"
// https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives
#include "Adafruit_SSD1306.h" // Driver for the hardware
#include "Wire.h"
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

extern uint16_t EQscaled[EQbins];

void DISPLAYbarz(){
  int barwideness = SCREEN_WIDTH/14;
  for(int i = 0; i < EQbins; i++){
    int barhightness = map(EQscaled[i], 0, LEDper, 0, SCREEN_HEIGHT); 
    display.fillRect(i*barwideness, SCREEN_HEIGHT-barhightness, barwideness-1, barhightness, 1); // void fillRect(uint16_t x0, uint16_t y0, uint16_t width, uint16_t height);  
  }
  //display.display();
}

void DISPLAYstartcircle(){
  display.clearDisplay();
  for (int z = 0; z < SCREEN_WIDTH; z++){
    for (int16_t i=1; i < 15; i+=1) {
      display.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, z/i, 1);
      display.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH-z/i, 1);
    }
    display.clearDisplay();
    display.display();
  }
  for (int z = SCREEN_WIDTH; z > 0; z--){
    for (int16_t i=1; i < 15; i+=1) {
      display.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, z/i, 1);
      display.drawCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH-z/i, 1);
    }
    display.clearDisplay();;
    display.display();       
    
  }
  display.clearDisplay();;
  display.display();
}

void DISPLAYperiodic(){
  display.clearDisplay();
  //display.setLogBuffer(5, 30);
  display.print("FPS: ");
  display.print(LEDS.getFPS());
  display.print(" PWR: ");
  display.println(calculate_unscaled_power_mW(leds, LEDtotal)/5/100);
  //display.drawLogBuffer(0, 0);
  display.display();
}

void DISPLAYsetup(){
  display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS);
  //display.begin(true /*DisplayEnable Enable*/, false /*LoRa Disable*/, true /*Serial Enable*/);
  display.clearDisplay();
  display.display();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  //display.drawBitmap(0, 0, songle, 89, 64, SSD1306_WHITE);
  display.display();
  display.clearDisplay();
  //display.display->setLogBuffer(5, 30);
  //OLEDstartcircle();
}

#endif