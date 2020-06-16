int LeftLDR = A4;
int RightLDR = A5;
int L = 0;
int R = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int left = constrain(analogRead(LeftLDR), 80, 450);
  int right = constrain(analogRead(RightLDR), 80, 450);
  L = map (left, 80, 450, 0, 100);
  R = map (right, 80, 450, 0, 100);
  Serial.println(L);
  Serial.println(R);
  delay(1000);
}
