# include "Wire.h"
//import MPU6050_DMP6;

const int MPU_ADDR= 0x68;

// These variables are 16 bit binary numbers(I assume)
// variables for acceleration in x,y and z, only these variables are ones to consider
//int16_t x_acceleration, y_acceleration, z_acceleration;  
float x_acceleration, y_acceleration, z_acceleration;  
// variables for the gyroscope
int16_t x_gyro, y_gyro, z_gyro;
// variable for temperature
// don't really need
float pitch;
int16_t temperature;

int ax, ay,az, temp, gx, gy, gz;

// only used to read data values
char tmp_str[7];
char* convert_int16_to_str(int16_t i) { // converts int16 to string
  sprintf(tmp_str, "%6d", i);
  return tmp_str;
}

// make sure to test this out, may need to do osmething to acceleration
float pitchAngle(float x_acceleration, float y_accerleration, float z_acceleration) {
  return atan2(x_acceleration , sqrt(y_acceleration * y_acceleration + z_acceleration * z_acceleration)) * (180.0 / PI);
}

void setup() {
  // put your setup code here, to run once:
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

  // convert int16 ot integers


  /*
  Potential formulas to calculate pitch:
  pitch = 0.96 * gyroAngleY + 0.04 * accAngleY;
  pitch = arcSin(accerlationX/9.81)
  */
  //pitch = 0.96 * y_gyro + 0.04 * y_acceleration;
  //pitch = atan2(y_acceleration, sqrt(x_acceleration * x_acceleration +  z_acceleration *  z_acceleration)) * (180.0 / PI);
  // real pitch formula, only reads +-60 degrees
  float x_accel = x_acceleration / 16384.0; // Convert to g (1g = 9.81 m/s²)
  float y_accel = y_acceleration / 16384.0;
  float z_accel = z_acceleration / 16384.0;
  pitch = atan2(x_accel , sqrt(y_accel * y_accel + z_accel * z_accel)) * (180.0 / PI);

  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print(" x accerlation: ");
  Serial.print(convert_int16_to_str(x_acceleration));
  Serial.print(" y accerlation: ");
  Serial.print(convert_int16_to_str(y_acceleration));
  Serial.print(" z accerlation: ");
  Serial.print(convert_int16_to_str(z_acceleration));
  Serial.print(" temperature");
  Serial.print(convert_int16_to_str(temperature));
  Serial.print(" x gryoscope: ");
  Serial.print(convert_int16_to_str(x_gyro));
  Serial.print(" y gryoscope: ");
  Serial.print(convert_int16_to_str(y_gyro));
  Serial.print(" z gryoscope: ");
  Serial.print(convert_int16_to_str(z_gyro));
  Serial.println();

  delay(1000);

}