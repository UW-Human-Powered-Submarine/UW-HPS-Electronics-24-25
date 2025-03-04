#include "main.h"

#if defined(PRESSURE_LIB_TEST)
//  +-------------------------+
//  |    PRESSURE_LIB_TEST    |
//  +-------------------------+
void setup() {
  
  Serial.begin(9600);
  
  Serial.println("Starting");
  
  Wire.begin();

  sensor.setModel(MS5837::MS5837_02BA);
  sensor.init();
  
  sensor.setFluidDensity(997); // kg/m^3 (997 freshwater, 1029 for seawater)
}

void loop() {

  sensor.read();

  Serial.print("Pressure: "); 
  Serial.print(sensor.pressure()); 
  Serial.println(" mbar");
  
  Serial.print("Temperature: "); 
  Serial.print(sensor.temperature()); 
  Serial.println(" deg C");
  
  Serial.print("Depth: "); 
  Serial.print(sensor.depth()); 
  Serial.println(" m");
  
  Serial.print("Altitude: "); 
  Serial.print(sensor.altitude()); 
  Serial.println(" m above mean sea level");

  delay(1000);
}

#elif defined(PRESSURE_SCHEDULER_TEST)
//  +-------------------------+
//  | PRESSURE_SCHEDULER_TEST |
//  +-------------------------+
void setup() {

}

void loop() {

}

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