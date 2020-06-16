#define enablemotora 9 //define motor 1 pinouts
#define dira1 5
#define dira2 7
#define enablemotorb 10 //define motor 2 pinouts
#define dirb1 6
#define dirb2 8
#include "SR04.h"
#define triggerpin 13 //define input/output pins
#define echopin 12
SR04 sr04 = SR04(echopin,triggerpin);
long distance; //define variable for distance
void setup() {
  pinMode(enablemotora, OUTPUT); // define output pins motor 1
  pinMode(dira1,OUTPUT);
  pinMode(dira2,OUTPUT);
  pinMode(enablemotorb, OUTPUT); // define output pins motor 2
  pinMode(dirb1,OUTPUT);
  pinMode(dirb2,OUTPUT);
  Serial.begin(9600); // allows me to see current distance and what the robot is doing
  digitalWrite(enablemotora, HIGH);
  digitalWrite(enablemotorb, HIGH);
  delay (1000);
}
void loop() {
  digitalWrite(enablemotora, HIGH);
  digitalWrite(enablemotorb, HIGH);
  distance=sr04.Distance(); // writes output distance to the variable.
  Serial.print(distance);
  Serial.println("cm");
  if (distance <25){
    Serial.println("Turning");
    tone(3, 392, 250);
    digitalWrite(dira1, LOW);
    digitalWrite(dira2, HIGH);
    digitalWrite(dirb1, HIGH);
    digitalWrite(dirb2, LOW);
    delay (125); //This amount will be changed to the amount of time it takes to make a 90 degree turn when the robot is built.
    tone(3, 262, 250);
  }
  else
  {
    Serial.println("Moving Forward");
    digitalWrite(dira1, HIGH);
    digitalWrite(dira2, LOW);
    digitalWrite(dirb1, HIGH);
    digitalWrite(dirb2, LOW);
  }
  delay(250);
}
