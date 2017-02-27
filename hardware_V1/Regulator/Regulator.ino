#include <TimerOne.h>
const int regCount = 2;
volatile int V[regCount];             // Variable to use as a counter of dimming steps. It is volatile since it is passed between interrupts
volatile boolean zero_cross[regCount]; // Flag to indicate we have crossed zero
int acPin[regCount] = {10, 11};                // Output to Opto Triac PWM wave
int levelIn[regCount] = {A0, A1};                // Dimming level input from Atmega main board
int dim[regCount] = {128, 128};                  // Dimming level (0-128)  0 = on, 128 = 0ff
int freqStep = 80;              // This is the delay-per-brightness step in microseconds. It allows for 128 steps
int stepCount = 128;
int regRelay[regCount] = {3, 4};

void setup() 
{  
  for (int i = 0; i < regCount; i++)
  {
    pinMode(acPin[i], OUTPUT);
    pinMode(regRelay[i], OUTPUT);
    digitalWrite(regRelay[i],LOW);
  }
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);      // Go to dim_check procedure every 75 uS (50Hz)  or 65 uS (60Hz)
}

void zero_cross_detect()
{
  for (int i = 0; i < regCount; i++)
  {
    zero_cross[i] = true;               // set flag for dim_check function that a zero cross has occured
    V[i] = 0;                           // stepcounter to 0.... as we start a new cycle
    digitalWrite(acPin[i], LOW);
  }
}

// Turn on the TRIAC at the appropriate time
// We arrive here every 75 (65) uS
// First check if a flag has been set
// Then check if the counter 'i' has reached the dimming level
// if so.... switch on the TRIAC and reset the counter

void dim_check()
{
  for (int i = 0; i < regCount; i++)
  {
    if (zero_cross[i] == true)
    {
      if (V[i] > dim[i])
      {
        digitalWrite(acPin[i], HIGH);  // turn on light
        V[i] = 0; // reset time step counter
        zero_cross[i] = false;  // reset zero cross detection flag
      }
      else
      {
        V[i] = V[i] + 1; // increment time step counter
      }
    }
  }
}

void regulatorLoop()
{
  for (int i = 0; i < regCount; i++)
  {
    int sensorValue = analogRead(levelIn[i]);
    int level = sensorValue * (128 / 1023.0);
    dim[i] = level;
    //Serial.println(String(dim[i]) + " " + String(i));
    //delay(100);
    if(dim[i]>=120)
    {
      digitalWrite(regRelay[i], HIGH);
      digitalWrite(acPin[i], LOW);
    }
    else 
    {
      digitalWrite(regRelay[i], LOW);
    }
  }
 
}


void loop() 
{
  regulatorLoop();
}
