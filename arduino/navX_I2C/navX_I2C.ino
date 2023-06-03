#include <Wire.h>
#include "AHRSWrapper.h"

#define ITERATION_DELAY_MS 10

AHRS ahrs = AHRS::getInstance();

void setup() {
  Serial.begin(115200);
}

int register_address = NAVX_REG_YAW_L;

void loop() {

  /* Display orientation values */
  Serial.print("yaw:  ");
  Serial.print(ahrs.getYaw(), 2);
  Serial.print("  pitch:  ");
  Serial.print(ahrs.getPitch(), 2);
  Serial.print("  roll:  ");
  Serial.print(ahrs.getRoll(), 2);
  Serial.print("  heading:  ");
  Serial.print(ahrs.getHeading(), 2);
  Serial.println("");

  delay(ITERATION_DELAY_MS);
}
