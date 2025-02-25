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
    
}
void loop() {
    
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