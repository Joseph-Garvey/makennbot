#define enablemotorA 9 //define motor 1 pinouts
#define dirLF 5
#define dirLB 7
#define enablemotorB 10 //define motor 2 pinouts
#define dirRF 6
#define dirRB 8
int LeftLDR = A4;
int RightLDR = A5;
int L = 0;
int R = 0;
int LeftMotorOut = 0;
int RightMotorOut = 0;
int LeftMotorPower = 0;
int RightMotorPower = 0;
void setup() {
  pinMode(enablemotorA, OUTPUT); // define output pins motor 1
  pinMode(dirLF,OUTPUT);
  pinMode(dirLB,OUTPUT);
  pinMode(enablemotorB,OUTPUT); // define output pins motor 2
  pinMode(dirRF,OUTPUT);
  pinMode(dirRB,OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int left = constrain(analogRead(LeftLDR), 50, 400);
  int right = constrain(analogRead(RightLDR), 50, 400);
  L = map (left, 50, 400, 0, 100);
  R = map (right, 50, 400, 0, 100);
  LeftMotorOut = map(L, 0, 100, 0 , 2);
  RightMotorOut = map(R, 0 ,100, 0, 2);
  if (LeftMotorOut < 1){
    LeftMotorPower = map(LeftMotorOut, 1, 0, 0, 255);
    digitalWrite(dirLF, LOW);
    digitalWrite(dirLB, HIGH);
    analogWrite(enablemotorA, LeftMotorPower);
    Serial.print("LeftReverse");
    Serial.println(LeftMotorPower);
  }
  else{
    LeftMotorPower = map(LeftMotorOut, 1, 2, 0, 255);
    digitalWrite(dirLF, HIGH);
    digitalWrite(dirLB, LOW);
    analogWrite(enablemotorA, LeftMotorPower);
    Serial.print("LeftForward");
    Serial.println(LeftMotorPower);
  }
  if (RightMotorOut < 1){
    RightMotorPower = map(RightMotorOut, 1, 0, 0, 255);
    digitalWrite(dirRF, LOW);
    digitalWrite(dirRB, HIGH);
    analogWrite(enablemotorB, RightMotorPower);
    Serial.print("RightReverse");
    Serial.println(RightMotorPower);
  }
  else{
    RightMotorPower = map(RightMotorOut, 1, 2, 0, 255);
    digitalWrite(dirRF, HIGH);
    digitalWrite(dirRB, LOW);
    analogWrite(enablemotorB, RightMotorPower);
    Serial.print("RightForward");
    Serial.println(RightMotorPower);
  }
 delay(2000);
}
