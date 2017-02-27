//Global Variables
String productId = "Alive_1";                               //Product Id                            //To check connection with Server 0-initial 1-connected 2-disconnected
//Two regulator and four Switch module
const int switches = 4;
const int regulators = 2;
//Setting up Switch variables
byte switchIn[switches]  = {2,4,6,8};                          //Input to control Physical Switch
byte switchOut[switches] = {3,5,7,9};                          //Reading State of Physical Switch
boolean switchState[switches] = {false, false, false, false};  //Storing Current State of Physical Switch
//Setting up Regulator variables
byte regulatorIn[regulators] = {10,11};                          //Analog Output to Regulator
byte regulatorOut[regulators] = {A0,A1};                         //To read Physical Button press of Regulator
int dimLevel[regulators] = {5,5};                                //Dimming Level of Regulator Default Off = 5

void setup() {
  Serial.begin(115200);                                 //To start Serial
  setupSwitches();                                      //Setup Switches                
}

void loop() {
  checkPhysicalSwitch();
  regulatorLoop();
  delay(100);
}

void serialEvent()
{
  checkReceive();
}

