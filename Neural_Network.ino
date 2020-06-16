#include <math.h>
#include "SR04.h" //include library for sr04 ultrasonic sensor
#define triggerpin 13 //define pins for ultrasonic sensor
#define echopin 12 
#define enablemotorL 9 //define motor 1 pinouts
#define dirLF 5
#define dirLB 7
#define enablemotorR 10 //define motor 2 pinouts
#define dirRF 6
#define dirRB 8
SR04 sr04 = SR04(echopin,triggerpin);
long distance; //define variable for distance
/******************************************************************
 * Network Configuration - customized per network 
 ******************************************************************/

const int PatternCount = 7;
const int InputNodes = 3;
const int HiddenNodes = 8;
const int OutputNodes = 2;
const float LearningRate = 0.25;
const float Momentum = 0.9;
const float InitialWeightMax = 0.5;
const float Success = 0.0004;

const byte Input[PatternCount][InputNodes] = {
  { 1, 1, 1 }, // Distance High, Light Left and Right       0
  { 1, 1, 0 }, // Distance High, Light Left                 1
  { 1, 0, 1 }, // Distance High, Light Right                2
  { 1, 0, 0 }, // Distance High, No Light                   3
  { 0, 1, 1 }, // Distance Low, Light Left and Right        4
  { 0, 1, 0 }, // Distance Low, Light Left                  5
  { 0, 0, 1 }, // Distance Low, Light Right                 6 // 00 was removed
}; 

const byte Target[PatternCount][OutputNodes] = {
  { 1, 1 }, // Forward
  { 0.75, 1}, // Bear Left
  { 1, 0.75}, // Bear Right
  { 1, 1 }, // Forward
  { 0, 1 }, // Spin Anticlockwise
  { 0, 1 }, // Spin Anticlockwise
  { 1, 0 }, // Spin Clockwise
};

/******************************************************************
 * End Network Configuration
 ******************************************************************/


int i, j, p, q, r;
int ReportEvery1000;
int RandomizedIndex[PatternCount];
long  TrainingCycle;
float Rando;
float Error;
float Accum;


float Hidden[HiddenNodes];
float Output[OutputNodes];
float HiddenWeights[InputNodes+1][HiddenNodes];
float OutputWeights[HiddenNodes+1][OutputNodes];
float HiddenDelta[HiddenNodes];
float OutputDelta[OutputNodes];
float ChangeHiddenWeights[InputNodes+1][HiddenNodes];
float ChangeOutputWeights[HiddenNodes+1][OutputNodes];

void setup(){
  pinMode(enablemotorL, OUTPUT); // define output pins motor 1
  pinMode(dirLF,OUTPUT);
  pinMode(dirLB,OUTPUT);
  pinMode(enablemotorR,OUTPUT); // define output pins motor 2
  pinMode(dirRF,OUTPUT);
  pinMode(dirRB,OUTPUT);
  pinMode(A4,INPUT);
  pinMode(A5, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(3));
  ReportEvery1000 = 1;
  for( p = 0 ; p < PatternCount ; p++ ) {    
    RandomizedIndex[p] = p ;
  }
}  

void loop (){

/******************************************************************
* Initialize HiddenWeights and ChangeHiddenWeights 
******************************************************************/

  for( i = 0 ; i < HiddenNodes ; i++ ) {    
    for( j = 0 ; j <= InputNodes ; j++ ) { 
      ChangeHiddenWeights[j][i] = 0.0 ;
      Rando = float(random(100))/100;
      HiddenWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
/******************************************************************
* Initialize OutputWeights and ChangeOutputWeights
******************************************************************/

  for( i = 0 ; i < OutputNodes ; i ++ ) {    
    for( j = 0 ; j <= HiddenNodes ; j++ ) {
      ChangeOutputWeights[j][i] = 0.0 ;  
      Rando = float(random(100))/100;        
      OutputWeights[j][i] = 2.0 * ( Rando - 0.5 ) * InitialWeightMax ;
    }
  }
  Serial.println("Initial/Untrained Outputs: ");
  toTerminal();
/******************************************************************
* Begin training 
******************************************************************/

  for( TrainingCycle = 1 ; TrainingCycle < 2147483647 ; TrainingCycle++) {    

/******************************************************************
* Randomize order of training patterns
******************************************************************/

    for( p = 0 ; p < PatternCount ; p++) {
      q = random(PatternCount);
      r = RandomizedIndex[p] ; 
      RandomizedIndex[p] = RandomizedIndex[q] ; 
      RandomizedIndex[q] = r ;
    }
    Error = 0.0 ;
/******************************************************************
* Cycle through each training pattern in the randomized order
******************************************************************/
    for( q = 0 ; q < PatternCount ; q++ ) {    
      p = RandomizedIndex[q];

/******************************************************************
* Compute hidden layer activations
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = HiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) {
          Accum += Input[p][j] * HiddenWeights[j][i] ;
        }
        Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
      }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i++ ) {    
        Accum = OutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          Accum += Hidden[j] * OutputWeights[j][i] ;
        }
        Output[i] = 1.0/(1.0 + exp(-Accum)) ;   
        OutputDelta[i] = (Target[p][i] - Output[i]) * Output[i] * (1.0 - Output[i]) ;   
        Error += 0.5 * (Target[p][i] - Output[i]) * (Target[p][i] - Output[i]) ;
      }

/******************************************************************
* Backpropagate errors to hidden layer
******************************************************************/

      for( i = 0 ; i < HiddenNodes ; i++ ) {    
        Accum = 0.0 ;
        for( j = 0 ; j < OutputNodes ; j++ ) {
          Accum += OutputWeights[i][j] * OutputDelta[j] ;
        }
        HiddenDelta[i] = Accum * Hidden[i] * (1.0 - Hidden[i]) ;
      }


/******************************************************************
* Update Inner-->Hidden Weights
******************************************************************/


      for( i = 0 ; i < HiddenNodes ; i++ ) {     
        ChangeHiddenWeights[InputNodes][i] = LearningRate * HiddenDelta[i] + Momentum * ChangeHiddenWeights[InputNodes][i] ;
        HiddenWeights[InputNodes][i] += ChangeHiddenWeights[InputNodes][i] ;
        for( j = 0 ; j < InputNodes ; j++ ) { 
          ChangeHiddenWeights[j][i] = LearningRate * Input[p][j] * HiddenDelta[i] + Momentum * ChangeHiddenWeights[j][i];
          HiddenWeights[j][i] += ChangeHiddenWeights[j][i] ;
        }
      }

/******************************************************************
* Update Hidden-->Output Weights
******************************************************************/

      for( i = 0 ; i < OutputNodes ; i ++ ) {    
        ChangeOutputWeights[HiddenNodes][i] = LearningRate * OutputDelta[i] + Momentum * ChangeOutputWeights[HiddenNodes][i] ;
        OutputWeights[HiddenNodes][i] += ChangeOutputWeights[HiddenNodes][i] ;
        for( j = 0 ; j < HiddenNodes ; j++ ) {
          ChangeOutputWeights[j][i] = LearningRate * Hidden[j] * OutputDelta[i] + Momentum * ChangeOutputWeights[j][i] ;
          OutputWeights[j][i] += ChangeOutputWeights[j][i] ;
        }
      }
    }

/******************************************************************
* Every 1000 cycles send data to terminal for display
******************************************************************/
    ReportEvery1000 = ReportEvery1000 - 1;
    if (ReportEvery1000 == 0)
    {
      Serial.println(); 
      Serial.println(); 
      Serial.print ("TrainingCycle: ");
      Serial.print (TrainingCycle);
      Serial.print ("  Error = ");
      Serial.println (Error, 5);

      toTerminal();

      if (TrainingCycle==1)
      {
        ReportEvery1000 = 999;
      }
      else
      {
        ReportEvery1000 = 1000;
      }
    }    


/******************************************************************
* If error rate is less than pre-determined threshold then end
******************************************************************/

    if( Error < Success ) break ;  
  }
  Serial.println ();
  Serial.println(); 
  Serial.print ("TrainingCycle: ");
  Serial.print (TrainingCycle);
  Serial.print ("  Error = ");
  Serial.println (Error, 5);

  toTerminal();

  Serial.println ();  
  Serial.println ();
  Serial.println ("Training Set Solved! ");
  Serial.println ("--------"); 
  Serial.println ();
  Serial.println ();  
  ReportEvery1000 = 1;
  while (Error < Success) {
    int LeftLDR = analogRead(A4);
    int RightLDR = analogRead(A5);
    distance=sr04.Distance();
    LeftLDR = constrain(LeftLDR, 50, 400);
    RightLDR = constrain(RightLDR, 50, 400);
    distance = constrain(distance, 5, 100);
    //Serial.print(LeftLDR); all debugging code
    //Serial.println(" Left");
    //Serial.print(RightLDR);
    //Serial.println(" Right");
    Serial.print(distance);
    //Serial.println("cm");
    float LightLeft = map(LeftLDR, 40, 400, 0, 100);
    float LightRight = map(RightLDR, 40, 400, 0, 100);
    float DistanceOutput = map(distance, 5, 100, 0, 100); // used to be 0-1 but couldnt get it to work
    float Inputs[] = {0, 0, 0};                           // takes longer to train with 0-100, still hadn't trained by 8000
    Inputs[0] = DistanceOutput/100; // so instead i just divided by 100 to get my desired output to feed into the neural network
    Inputs[1] = LightLeft/100; // now trains in 1367 cycles approx. takes under a min
    Inputs[2] = LightRight/100; // outputs finally work
    //Serial.print("Input 1 Distance:"); 
    //Serial.println(Inputs[0]);
    //Serial.print("Input 2 Left: ");
    //Serial.println(Inputs[1]);
    //Serial.print("Input 3 Right: "); 
    //Serial.println(Inputs[2]);
    InputToOutput(Inputs[0], Inputs[1], Inputs[2]);
    int speedL = Output[0] * 100;
    int speedR = Output[1] * 100;
    //Serial.println(speedL);
    //Serial.print("Speed Left");
    //Serial.println(speedR);
    //Serial.print("Speed Right");
    if (speedL < 50)
    {
      speedL = map(speedL, 50, 0, 0, 255);
      digitalWrite(dirLF, LOW);
      digitalWrite(dirLB, HIGH);
      analogWrite(enablemotorL, speedL);
      //Serial.print("Left dir = ");
      //Serial.println(leftdir);
      //Serial.print("Left speed = ");
      //Serial.println(speedL);
    }
    else
    {
      speedL = map (speedL, 50, 100, 0, 255);
      digitalWrite(dirLF, HIGH);
      digitalWrite(dirLB, LOW);
      analogWrite(enablemotorL, speedL);
      //Serial.print("Left dir = ");
      //Serial.println(leftdir);
      //Serial.print("Left speed = ");
      //Serial.println(speedL);
    }
    if (speedR < 50)
    {
      speedR = map(speedR, 50, 0, 0, 255);
      digitalWrite(dirRF, LOW);
      digitalWrite(dirRB, HIGH);
      analogWrite(enablemotorR, speedR);
      //Serial.print("Left dir = ");
      //Serial.println(leftdir);
      //Serial.print("Left speed = ");
      //Serial.println(speedL);
    }
    else
    {
      speedR = map (speedR, 50, 100, 0, 255);
      digitalWrite(dirRF, HIGH);
      digitalWrite(dirRB, LOW);
      analogWrite(enablemotorR, speedR);
      //Serial.print("Left dir = ");
      //Serial.println(leftdir);
      //Serial.print("Left speed = ");
      //Serial.println(speedL);
    }
   delay(500); 
  }
}

void toTerminal()
{

  for( p = 0 ; p < PatternCount ; p++ ) { 
    Serial.println(); 
    Serial.print ("  Training Pattern: ");
    Serial.println (p);      
    Serial.print ("  Input ");
    for( i = 0 ; i < InputNodes ; i++ ) {
      Serial.print (Input[p][i], DEC);
      Serial.print (" ");
    }
    Serial.print ("  Target ");
    for( i = 0 ; i < OutputNodes ; i++ ) {
      Serial.print (Target[p][i], DEC);
      Serial.print (" ");
    }
/******************************************************************
* Compute hidden layer activations
******************************************************************/

    for( i = 0 ; i < HiddenNodes ; i++ ) {    
      Accum = HiddenWeights[InputNodes][i] ;
      for( j = 0 ; j < InputNodes ; j++ ) {
        Accum += Input[p][j] * HiddenWeights[j][i] ;
      }
      Hidden[i] = 1.0/(1.0 + exp(-Accum)) ;
    }

/******************************************************************
* Compute output layer activations and calculate errors
******************************************************************/

    for( i = 0 ; i < OutputNodes ; i++ ) {    
      Accum = OutputWeights[HiddenNodes][i] ;
      for( j = 0 ; j < HiddenNodes ; j++ ) {
        Accum += Hidden[j] * OutputWeights[j][i] ;
      }
      Output[i] = 1.0/(1.0 + exp(-Accum)) ; 
    }
    Serial.print ("  Output ");
    for( i = 0 ; i < OutputNodes ; i++ ) {       
      Serial.print (Output[i], 5);
      Serial.print (" ");
    }
  }

}

void InputToOutput(float In1, float In2, float In3)
{
  float Inputs[] = {0, 0, 0};
  Inputs[0] = In1;
  Inputs[1] = In2;
  Inputs[2] = In3;

  /******************************************************************
    Compute hidden layer activations
  ******************************************************************/

  for ( i = 0 ; i < HiddenNodes ; i++ ) {
    Accum = HiddenWeights[InputNodes][i] ;
    for ( j = 0 ; j < InputNodes ; j++ ) {
      Accum += Inputs[j] * HiddenWeights[j][i] ;
    }
    Hidden[i] = 1.0 / (1.0 + exp(-Accum)) ;
  }

  /******************************************************************
    Compute output layer activations and calculate errors
  ******************************************************************/

  for ( i = 0 ; i < OutputNodes ; i++ ) {
    Accum = OutputWeights[HiddenNodes][i] ;
    for ( j = 0 ; j < HiddenNodes ; j++ ) {
      Accum += Hidden[j] * OutputWeights[j][i] ;
    }
    Output[i] = 1.0 / (1.0 + exp(-Accum)) ;
  }
#ifdef DEBUG
  SerialUSB.print ("  Output ");
  for ( i = 0 ; i < OutputNodes ; i++ ) {
    SerialUSB.print (Output[i], 5);
    SerialUSB.print (" ");
  }
#endif
}
