#include<Bounce2.h>
Bounce bounceObject=Bounce();

const int sensorPin=A0;
const int relay=2;
const int switchPin=12;
const float signalVoltage = 3.0; //use a multi-meter to measure the output signal voltage of PIR sensor when triggered

int state=LOW;
int timeStart=0;
int timeNow;
int time_interval=0;
int timeToSleep=5400; //time until light shuts off, in seconds (5400 = 1.5 hrs)

int s = map(signalVoltage, 0, 5, 0, 1023);


void setup() {
  Serial.begin(9600);
  pinMode(relay,OUTPUT);
  pinMode(switchPin,INPUT);
  bounceObject.attach(switchPin);
}

void loop() {

  if (analogRead(A0) > s && analogRead(A0) < 1.05*s){ //if sensor detects something, turn on lights
    
    timeStart=millis()/1000; //update timeStart
    state=HIGH;
    digitalWrite(relay,state);
    Serial.println("\n\n\n***Sensor tripped!***");
    printStatus();
    delay(1000*4); //sensor outputs high for about 2 seconds; wait for it to output low again
  }

  if (bounceObject.update() && bounceObject.read()==HIGH){ //if bedside switch is pressed
    state=!state; //turn lights on if they're off, turn off if they're on
    timeStart=millis()/1000; //update timeStart
    digitalWrite(relay,state);
    
    Serial.println("\n\n\n***Switch pressed***");
    printStatus();
  }
  
  timeNow=millis()/1000;
  
  time_interval=timeNow-timeStart;
  
  if(time_interval>timeToSleep){ 
    if (state==HIGH){
      state=LOW; //turn off the light
      digitalWrite(relay,state);
      Serial.println("\n\n\n***Time Expired***");
      printStatus();
    }
  }
}


void printStatus(){
  Serial.print("State: ");
  Serial.println(state);
  Serial.print("Interval Time: ");
  Serial.println(time_interval);
  Serial.print("Elapsed Time:");
  Serial.println(millis()/1000);
  Serial.print("Sensor Value: ");
  Serial.println(analogRead(A0));
}


