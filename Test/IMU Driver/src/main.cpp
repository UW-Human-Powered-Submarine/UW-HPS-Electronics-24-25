#include "main.h"

#if defined(IMU_LIB_TEST)
//  +-------------------------+
//  |      IMU_LIB_TEST       |
//  +-------------------------+

void setup() {
    Serial.begin(9600);

    imu.set_refresh_period_ms(500);
    imu.begin();
}

void loop() {
    imu.update();

    Serial.print("Time(ms):");
    Serial.println(millis());

    Serial.print("\txa=");
    Serial.print(imu.get_x_acceleration());
    Serial.print("\tya=");
    Serial.print(imu.get_y_acceleration());
    Serial.print("\tza=");
    Serial.println(imu.get_z_acceleration());

    Serial.print("\txg=");
    Serial.print(imu.get_x_gyro());
    Serial.print("\tyg=");
    Serial.print(imu.get_y_gyro());
    Serial.print("\tzg=");
    Serial.println(imu.get_z_gyro());

    Serial.print("\tT=");
    Serial.println(imu.get_temperature());

    Serial.println();
    delay(500);
}

//  endif IMU_LIB_TEST
#elif defined(IMU_PITCH_LIB_TEST)
//  +-------------------------+
//  |   IMU_PITCH_LIB_TEST    |
//  +-------------------------+

void setup() {
    Serial.begin(9600);

    imu.set_refresh_period_ms(500);
    imu.begin();

    pitch_reading.set_refresh_period_ms(500);
    pitch_reading.register_imu(&imu);

    Serial.println("Calibrate Gravity Vector...");
    delay(2000);
    imu.refresh();
    pitch_reading.calibrate_gravity(imu.get_acceleration_vec());
    pitch_reading.get_gravity_calibration().println();
    delay(2000);

    Serial.println("Calibrate Pitch Direction Vector...");
    delay(2000);
    imu.refresh();
    pitch_reading.calibrate_pitch_direction(imu.get_acceleration_vec());
    pitch_reading.get_pitch_direction_calibration().println();
    delay(2000);
}
void loop() {
    imu.update();
    pitch_reading.update();

    Serial.println();
    Vector3D::print_vector(imu.get_acceleration_vec().normalize());
    Serial.print("Pitch = ");
    Serial.println(pitch_reading.get_pitch_deg());
    
    delay(500);
}

//  endif IMU_PITCH_LIB_TEST
#elif defined(BLINKING_TEST)
//  +-------------------------+
//  |      BLINKING_TEST      |
//  +-------------------------+
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