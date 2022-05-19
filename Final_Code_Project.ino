#include <PID_v1.h>

const int photores = A0; // Photo resistor input
const int pot = A1; // Potentiometer input
const int led = 9; // LED output
double lightLevel; // variable that stores the incoming light level

// For Fan
#define FAN 5
#define DIRA 3
#define DIRB 4

// Tuning parameters
float Kp=0; //Initial Proportional Gain 
float Ki=10; //Initial Integral Gain 
float Kd=0; //Initial Differential Gain


double Setpoint, Input, Output; //These are just variables for storingvalues
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, REVERSE); // This sets up our PDID Loop


const int sampleRate = 1;      // Variable that determines how fast our PID loop runs
const long serialPing = 500;  // This determines how often we ping our loop 
unsigned long now = 0;       // This variable is used to keep track of time


unsigned long lastMessage = 0; //This keeps track of when our loop last 

//**********************************************
void setup(){
  
  pinMode(FAN, OUTPUT);
  pinMode(DIRA, OUTPUT);
  pinMode(DIRB, OUTPUT);

  digitalWrite(DIRA,HIGH); //one way
  digitalWrite(DIRB,LOW);
  digitalWrite(FAN, LOW); // enable off

  lightLevel = analogRead(photores);        //Read in light level
  Input = map(lightLevel, 0, 1024, 0, 255); //Change read scale to analog 
  Setpoint = map(analogRead(pot), 0, 1024, 0, 255); 
  
  Serial.begin(9600);                 //Start a serial session 
  myPID.SetMode(AUTOMATIC);          //Turn on the PID loop 
  myPID.SetSampleTime(sampleRate);  //Sets the sample rate 


 
  lastMessage = millis(); // timestamp 
}

//**********************************************
void loop(){

  Setpoint = map(analogRead(pot), 0, 1024, 0, 255);    // Read our setpoint 
  lightLevel = analogRead(photores);                  // Get the light level
  Input = map(lightLevel, 0, 900, 0, 255);            // Map it to the right scale 
  myPID.Compute();                                   // Run the PID loop
  analogWrite(led, Output);                         // Write out the output from the PID loop
 
  now = millis();                   //Keep track of time
  if(now - lastMessage > serialPing) { 
    Serial.print("Setpoint = "); 
    Serial.print(Setpoint); 
    Serial.print(" Input = "); 
    Serial.print(Input); 
    Serial.print(" Output = "); 
    Serial.print(Output); 
    Serial.print("\n");


  lastMessage = now; //update the time stamp.
  } 

  // Turn Fan on if Low brightness is read from Input
  if(Input < 70){
    digitalWrite(FAN, HIGH);
  }else{
    digitalWrite(FAN, LOW);
  }

}