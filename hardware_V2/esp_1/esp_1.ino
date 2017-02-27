// Global Variables
char payload_str[20];     //to store recieved messages
String str;               //USed 
char * ch;
boolean relay_state[4] = {false, false, false, false};
byte  relay_num[4] = {2, 3, 4, 5};
byte physical_sw[4] = {6, 7, 8, 9};
int counter = 0;

byte regOut[2] = {10, 11}; // Analogoutput to regulator
byte regIn[2] = {A0,A1}; // Physical buttons reader 
int dim[2] = {5,5}; //Default State of Regulator is off
int regCount = 1;
int wait;

void setup() {
  Serial.begin(115200);
  //debug.begin(115200);
  //delay(10);
  setupEsp(); // setup the esp 
  setupSwitches(); // setup the switches
  //connect to server
  ConnectToServer("10.124.195.116", "9000");
  eatmillis(200);
  randomSeed(analogRead(0));  //seed for random number generation
  //delay(100);                 
  sendToServer("E-R-U111");    //E-R-PID #for one time registration when powered up
  delay(100);
  sendToServer("E-S-U111");   //E-S-111 and reply is S-4-XXXX
}
void loop() {
  delay(100); // Loop frequency controlling delay
  //checkForPhysicalSw();
  playPingPong(1);
  regulatorLoop(regCount-1);
  counter++;
  if(counter>200)
    wait++;
}

void serialEvent() {
  checkForRcv();
  //delay(10);
  processPayload();
}

// problems in this code
//1. In function setupEsp() if the esp doesnot work the code will stuckk there for infinite time.
//2. In function setupSwitches() input mod eis not clearly used
//3. in connect to server, the communication between esp and atmega is not analysed, also the response from server is not analysed.
//4. eatmillis() function is a work around for incoming serial data from overflowing the buffer. Rather than just eating up all data we should analyse the data.
//5. randomSeed(analogRead(0)) should have a bettre seed
//6. similar to point 3 sendToServer must have response checking mechanism.
//7. delay function may posses some problems 
//8. I think pingPong can be implmented better.


