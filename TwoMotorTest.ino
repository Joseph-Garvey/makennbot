#define enablemotora 9 //define motor 1 pinouts
#define dira1 5
#define dira2 7
#define enablemotorb 10 //define motor 2 pinouts
#define dirb1 6
#define dirb2 8
void setup() { 
  pinMode(enablemotora, OUTPUT); // define output pins motor 1
  pinMode(dira1,OUTPUT);
  pinMode(dira2,OUTPUT);
  pinMode(enablemotorb,OUTPUT); // define output pins motor 2
  pinMode(dirb1,OUTPUT);
  pinMode(dirb2,OUTPUT);
  Serial.begin(9600);
}
void loop() {
  Serial.println("Hope this works");
  digitalWrite(enablemotora, HIGH);
  digitalWrite(dira1, HIGH);
  digitalWrite(dira2, LOW);
  digitalWrite(enablemotorb, HIGH);
  digitalWrite(dirb1, HIGH);
  digitalWrite(dirb2, LOW);
  delay(1000);
  digitalWrite(dira1, LOW);
  digitalWrite(dira2, HIGH);
  digitalWrite(dirb1, LOW);
  digitalWrite(dirb2, HIGH);
  delay(1000);
}
