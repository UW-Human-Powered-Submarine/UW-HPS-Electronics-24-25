#include <Arduino.h>

unsigned long currentTime;
unsigned long pastTime;
unsigned long diff;

int thresholdVoltage;
int totReadings;
int currentVoltage;
int pastVoltage; 


//enum States {idle, findingThreshold, findingRPM};
//unsigned char currState; // keeping track of current state
unsigned long timerMillis; // keeping track of time
unsigned long time;

void setup() {
  //initalize pin setup 
  //initalize variables 
  Serial.begin(9600);

  pastTime = 0;
  timerMillis = 0;
  thresholdVoltage = 0;
  totReadings = 1;
  //currState = idle;
}

float lam = 0.955;

void loop() {
  
  //Testing code
  if((millis() - timerMillis) >= 2000){
    Serial.println("threshold Voltage: ");
    Serial.println(thresholdVoltage);
    timerMillis = millis();
  }
  
  int newThresholdVoltage = (unsigned long) (lam*thresholdVoltage + (1 - lam) * analogRead(A1));
  Serial.println(newThresholdVoltage);
  thresholdVoltage = (newThresholdVoltage + thresholdVoltage) / totReadings;
  totReadings++;
  
  /*
  switch (currState) {
    // idle state(nothing is happening)
    case idle:
      // current nothing will happen, for now, wait 3 seconds before switching to nxt state
      totReadings = 1;
      if ((millis() - timerMillis) == 3000){
        currState = findingThreshold;
        timerMillis = millis();
      }

    // findingThreshold (find the threshold voltage by using the mean)
    case findingThreshold:
      // if the calibration time is 3 sec, will switch to findingRPM
      if((millis() - timerMillis) == 3000) {
        timerMillis = millis();
        currState = findingRPM;
      }
      int newThresholdVoltage = (unsigned long) (lam*thresholdVoltage + (1 - lam) * analogRead(A1));
      thresholdVoltage = (newThresholdVoltage + thresholdVoltage) / totReadings;
      totReadings++;




    // findingRPM use the white/black to find RPM
    case findingRPM:
      currentVoltage = (unsigned long) (lam*currentVoltage + (1 - lam) * analogRead(A1));
      //Serial.println(currentVoltage);
      if((currentVoltage <= thresholdVoltage && pastVoltage > thresholdVoltage) || (pastVoltage <= thresholdVoltage && currentVoltage > thresholdVoltage)){
        currentTime = millis();
        diff = currentTime - pastTime;
        pastTime = currentTime;
        Serial.println(diff);
        pastVoltage = currentVoltage;
      }

  }*/
  

  /* put your main code here, to run repeatedly:
  currentVoltage = (unsigned long) (lam*currentVoltage + (1 - lam) * analogRead(A1));
  //Serial.println(currentVoltage);
  if((currentVoltage <= 245 && pastVoltage > 245) || (pastVoltage <= 245 && currentVoltage > 245)){
    currentTime = millis();
    diff = currentTime - pastTime;
    pastTime = currentTime;
    Serial.println(diff);
    pastVoltage = currentVoltage;
    
  }*/
  delay(10);

  


}
/*#include <Arduino.h>

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

  


}*/