#include "SR04.h"
#define triggerpin 13 //define input/output pins
#define echopin 12 
SR04 sr04 = SR04(echopin,triggerpin);
long distance; //define variable for distance
void setup() {
  Serial.begin(9600); // allows me to see current distance.
  delay (1000); // gives one second before startup
}

void loop() {
  distance=sr04.Distance(); // writes output distance to the variable.
  Serial.print(distance);
  Serial.println("cm");
  delay(250); // sets refresh at 4hz
}
