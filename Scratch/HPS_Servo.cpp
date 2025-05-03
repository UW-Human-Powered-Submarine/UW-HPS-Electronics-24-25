#include <Servo.h>

Servo leftServo;
Servo rightServo;
int pos = 90;
void setup() {
  leftServo.attach(9);  // attaches the servo on pin 9 to the servo object
  rightServo.attach(10);
}
  
void loop() {
  /*myservo.write(pos);
  secondServo.write(pos);*/
    for (pos = 90; pos <= 135; pos += 1) { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      leftServo.write(pos);              // tell servo to go to position in variable 'pos'
      Serial.write(pos);
      delay(15);                       // waits 15ms for the servo to reach the position
      rightServo.write(pos);
      delay(15);
    }
    for (pos = 135; pos >= 90; pos -= 1) { // goes from 180 degrees to 0 degrees
      leftServo.write(pos);              // tell servo to go to position in variable 'pos'
      delay(15);                       // waits 15ms for the servo to reach the position
      rightServo.write(pos);
      delay(15);
    }
    
}