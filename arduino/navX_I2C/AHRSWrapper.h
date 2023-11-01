#ifndef AHRSWRAPPER_H
#define AHRSWRAPPER_H

#include <Wire.h>

#include "AHRSProtocol.h"  // navX-Sensor Register Definition header file
#include <Arduino.h>

class AHRS {
 public:
  static AHRS getInstance();
  void transmit();
  void receive();
  float getYaw();
  float getPitch();
  float getRoll();
  float getHeading();

 private:
  AHRS();
  uint8_t data[512];
  int register_address = NAVX_REG_YAW_L;
  float yaw;
  float pitch;
  float roll;
  float heading;
};

#endif