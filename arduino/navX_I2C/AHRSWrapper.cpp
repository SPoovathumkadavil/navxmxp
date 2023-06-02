#include "AHRSWrapper.h"

#define NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT 0x32
#define NUM_BYTES_TO_READ 8

int defined = 0;
static AHRS instance;

static AHRS AHRS::getInstance() {
  if (defined == 0) {
    instance = AHRS();
    defined = 1;
  }
  return instance;
}

/**
 * Make sure you do not call this. Only call getInstance()
 */
AHRS::AHRS() {
  Wire.begin();  // join i2c bus (address optional for master)
  for (int i = 0; i < sizeof(data); i++) {
    data[i] = 0;
  }
}

void AHRS::transmit() {
  /* Transmit I2C data request */
  Wire.beginTransmission(
      NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT);  // Begin transmitting to
                                             // navX-Sensor
  Wire.write(register_address);              // Sends starting register address
  Wire.write(NUM_BYTES_TO_READ);             // Send number of bytes to read
  Wire.endTransmission();                    // Stop transmitting
}

void AHRS::receive() {
  int i = 0;
  /* Receive the echoed value back */
  Wire.beginTransmission(
      NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT);  // Begin transmitting to
                                             // navX-Sensor
  Wire.requestFrom(NAVX_SENSOR_DEVICE_I2C_ADDRESS_7BIT,
                   NUM_BYTES_TO_READ);  // Send number of bytes to read
  delay(1);
  while (Wire.available()) {  // Read data (slave may send less than requested)
    data[i++] = Wire.read();
  }
  Wire.endTransmission();  // Stop transmitting

  /* Decode received data to floating-point orientation values */
  yaw = IMURegisters::decodeProtocolSignedHundredthsFloat(
      (char *)&data[0]);  // The cast is needed on arduino
  roll = IMURegisters::decodeProtocolSignedHundredthsFloat(
      (char *)&data[2]);  // The cast is needed on arduino
  pitch = -IMURegisters::decodeProtocolSignedHundredthsFloat(
      (char *)&data[4]);  // The cast is needed on arduino
  heading = IMURegisters::decodeProtocolUnsignedHundredthsFloat(
      (char *)&data[6]);  // The cast is needed on arduino
}

float AHRS::getYaw() {
  transmit();
  receive();
  return yaw;
}

float AHRS::getPitch() {
  transmit();
  receive();
  return pitch;
}

float AHRS::getRoll() {
  transmit();
  receive();
  return roll;
}

float AHRS::getHeading() {
  transmit();
  receive();
  return heading;
}
