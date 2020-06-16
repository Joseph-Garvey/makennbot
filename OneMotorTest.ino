#define enablemotor1 3 //define motor 1 pinouts
#define dira1 2
#define dira2 1
void setup() { 
  pinMode(enablemotor1, OUTPUT); // define output pins motor 1
  pinMode(dira1,OUTPUT);
  pinMode(dira2,OUTPUT);
}

void loop() {
digitalWrite(enablemotor1, HIGH); //
digitalWrite(dira1, HIGH);
digitalWrite(dira2, LOW);
}
