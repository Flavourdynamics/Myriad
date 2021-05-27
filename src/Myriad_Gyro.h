#ifndef Myriad_Gyro_h
#define Myriad_Gyro_h

// Include order may need to be unified in sketch
#include "Wire.h"

const int MPUaddr=0x68;  // I2C address of the MPU-6050
float AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

void MPUsetup(){
  Wire1.begin(21,22);
  Wire1.beginTransmission(MPUaddr);
  Wire1.write(0x6B);  // PWR_MGMT_1 register
  Wire1.write(0);     // set to zero (wakes up the MPU-6050)
  Wire1.endTransmission(true);
}

void MPUprint(){
  Serial.print("Acc: ");
  Serial.print(AcX);
  Serial.print(",");
  Serial.print(AcY);
  Serial.print(",");
  Serial.println(AcZ);
  
  Serial.print("Gyro: ");
  Serial.print(GyX);
  Serial.print(",");
  Serial.print(GyY);
  Serial.print(",");
  Serial.println(GyZ);
  
  Serial.print(Tmp);
  Serial.println("°C");
}

void MPUread(){
  Wire1.beginTransmission(MPUaddr);
  Wire1.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire1.endTransmission(false);
  Wire1.requestFrom(MPUaddr, 14, true);  // request a total of 14 registers   Wire1.requestFrom(MPUaddr, 14, true);
  // ^^ can probably change that to a 1 later
  AcX=(int16_t)(Wire1.read()<<8|Wire1.read())/16384.00; // g // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY=(int16_t)(Wire1.read()<<8|Wire1.read())/16384.00; // g  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ=(int16_t)(Wire1.read()<<8|Wire1.read())/16384.00; // g  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp=(int16_t)(Wire1.read()<<8|Wire1.read())/340.00 + 36.53;  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L) 
  GyX=(int16_t)(Wire1.read()<<8|Wire1.read())/131.00;  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY=(int16_t)(Wire1.read()<<8|Wire1.read())/131.00;   // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ=(int16_t)(Wire1.read()<<8|Wire1.read())/131.00;   // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)'
}

#ifdef Myriad_Display
  void MPUoled(){
    Heltec.display->clear();
    Heltec.display->setLogBuffer(5, 30);
    Heltec.display->print("Acc: ");
    Heltec.display->print(AcX);
    Heltec.display->print(", ");
    Heltec.display->print(AcY);
    Heltec.display->print(", ");
    Heltec.display->println(AcZ);

    Heltec.display->print("Gyro: ");
    Heltec.display->print(GyX);
    Heltec.display->print(", ");
    Heltec.display->print(GyY);
    Heltec.display->print(", ");
    Heltec.display->println(GyZ);

    Heltec.display->print(Tmp);
    Heltec.display->println("°C");

    Heltec.display->drawLogBuffer(0, 0);
    Heltec.display->display();
  }
#endif
#endif