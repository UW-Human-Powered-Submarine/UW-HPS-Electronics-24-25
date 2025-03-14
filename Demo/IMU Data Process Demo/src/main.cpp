#include "main.h"
#ifndef BLINKING_TEST

void setup() {
    // begins the serial port
    Serial.begin(9600);

    // Begin the I2 communication
    Wire.begin();
    // begin communication with the IMU
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x6B); // data from registor that we want, the gyroscope
    Wire.write(0); // wakes up the 6050
    // can begin or stop transmission from the arduino to IMU
    Wire.endTransmission(true);
}

void loop() {
    // put your main code here, to run repeatedly:
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(0x3B); // gets data from registor we want
    Wire.endTransmission(false); // restart arudnio, but keep up communications
    Wire.requestFrom(MPU_ADDR, 7*2, true); // requesting 14 registors 

    // reads the  acceleration
    x_acceleration = Wire.read() << 8 | Wire.read(); 
    y_acceleration = Wire.read() << 8 | Wire.read();
    z_acceleration = Wire.read() << 8 | Wire.read();

    temperature = Wire.read()<<8 | Wire.read(); // reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
    x_gyro = Wire.read()<<8 | Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
    y_gyro = Wire.read()<<8 | Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
    z_gyro = Wire.read()<<8 | Wire.read(); 

    // Serial.print(convert_int16_to_str(x_acceleration));
    Serial.print(x_acceleration);
    Serial.print(",");
    Serial.print(y_acceleration);
    // Serial.print(convert_int16_to_str(y_acceleration));
    Serial.print(",");
    Serial.print(z_acceleration);
    // Serial.print(convert_int16_to_str(z_acceleration));
    Serial.println();

    delay(REFRESH_PERIOD);
}


char* convert_int16_to_str(int16_t i) { // converts int16 to string
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

#else
//  BLINKING TEST CODE
#define DELAY_MS 500
#define LED_PIN 13
void setup() {
    pinMode(LED_PIN, OUTPUT);
}
void loop() {
    digitalWrite(LED_PIN, HIGH);
    delay(DELAY_MS);
    digitalWrite(LED_PIN, LOW);
    delay(DELAY_MS);
}
#endif  //  BLINKING_TEST