#include <TimerOne.h>
int regCount = 0;
volatile int V;             // Variable to use as a counter of dimming steps. It is volatile since it is passed between interrupts
volatile boolean zero_cross; // Flag to indicate we have crossed zero
int acPin = 11;                 // Output to Opto Triac PWM wave
int levelIn = A0;                 // Dimming level input from Atmega main board
int dim = 128;                  // Dimming level (0-5)  0 = on, 128 = 0ff
int freqStep = 80;              // This is the delay-per-brightness step in microseconds. It allows for 128 steps
int stepCount = 128;


void setup() {  // Begin setup
  //Serial.begin(115200);
  pinMode(acPin, OUTPUT);
  attachInterrupt(0, zero_cross_detect, RISING);    // Attach an Interupt to Pin 2 (interupt 0) for Zero Cross Detection
  
  Timer1.initialize(freqStep);                      // Initialize TimerOne library for the freq we need
  Timer1.attachInterrupt(dim_check, freqStep);      // Go to dim_check procedure every 75 uS (50Hz)  or 65 uS (60Hz)
}

void zero_cross_detect()
{
  zero_cross = true;               // set flag for dim_check function that a zero cross has occured
  V = 0;                           // stepcounter to 0.... as we start a new cycle
  digitalWrite(acPin, LOW);
}

// Turn on the TRIAC at the appropriate time
// We arrive here every 75 (65) uS
// First check if a flag has been set
// Then check if the counter 'i' has reached the dimming level
// if so.... switch on the TRIAC and reset the counter

void dim_check()
{

  if (zero_cross == true)
  {
    if (V > dim)
    {
      digitalWrite(acPin, HIGH);  // turn on light
      V = 0; // reset time step counter
      zero_cross = false;  // reset zero cross detection flag
    }
    else {
      V = V + 1; // increment time step counter
    }
  }
}

void regulatorLoop()
{
  int sensorValue = analogRead(levelIn);
  int level = sensorValue * (128 / 1023.0);
  dim = level;
  //Serial.println(dim);
}


void loop() 
{
  regulatorLoop();
  delay (50);
}
