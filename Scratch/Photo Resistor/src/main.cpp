#include <Arduino.h>

unsigned long currentTime;
unsigned long pastTime;
unsigned long diff;

int currentVoltage;
int pastVoltage; 

void setup() {
  //initalize pin setup 
  //initalize variables 
  Serial.begin(9600);

  pastTime = 0;
}

float lam = 0.955;

void loop() {
  // put your main code here, to run repeatedly:
  currentVoltage = (unsigned long) (lam*currentVoltage + (1 - lam) * analogRead(A1));
  //Serial.println(currentVoltage);
  if((currentVoltage <= 245 && pastVoltage > 245) || (pastVoltage <= 245 && currentVoltage > 245)){
    currentTime = millis();
    diff = currentTime - pastTime;
    pastTime = currentTime;
    Serial.println(diff);
    pastVoltage = currentVoltage;
    
  }
  delay(1);

  


}
