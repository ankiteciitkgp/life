#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WebSocketsClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <Hash.h>

//defining serial for communication between esp and uC
#define uc_SERIAL Serial

//Initialize WiFiMulti Object
ESP8266WiFiMulti WiFiMulti;
//Initializing Websocket Object
WebSocketsClient webSocket;
//Initializing WebServer Object on port 80
ESP8266WebServer server(80);

//Global Variables
const char* dalivessid = "Alive";                        //Default SSID and Password of the access point
const char* dalivepass = "alivehomes";
String productId = "Alive_AH2";                               //Product Id
char *ipServer = "54.169.236.206";//"10.124.195.116";    //Server IP Adress
uint16_t portServer = 9000;                              //Server Port
boolean hardwareReset = true;                            //Flag for hardware reset is true when hardware is reset and once connects to server will be changed to false 
String currentStatus = "";                               //Variable to store current state of things in ESP gets updated from uC
int waitCount  = 0;                                      //Main wait count which goes up to 400  
int scount = 0;                                          //Small Counter 
boolean tryConnect = true;                               //Flag for trying to connect to server 
String wifissid;                                         // Variable to store WiFi ssid and password read by eeprom
String wifipass;


//Functional Prototypes
void webSocketEvent(WStype_t type, uint8_t *payload, size_t lenght);
void arduinoSend(char *payload);
String arduinoReceive();
void processarduinoReceive(char *payload);
void processPayload(char *payload);
void writeAliveCredentials(String argalivename, String argalivepass);
void writeWifiCredentials(String argwifiname, String argwifipass);
void handleRoot();
void handleNotFound();
void createAliveAp();
void connectToServer();

//Setup Function
void setup() {
  uc_SERIAL.begin(115200);
  EEPROM.begin(512);
  for (uint8_t t = 4; t > 0; t--) {
    delay(1000);
  }
  createAliveAp();
  connectToServer();
}

void loop()
{
  //try to connect to server when scount is greater than equal to waitcount and trycnnect flag is set
  if (scount >= waitCount && tryConnect)
  {
    webSocket.loop();         // Takes around 6 seconds when not connected to Server
    scount = 0;
  }
  else if (scount <= waitCount)
    scount++;
  //If Wifissid is not null try connecting to wifi since it doesn't use much time
  if(wifissid != NULL)
    WiFiMulti.run();
  //Check if any message is received from arduino
  arduinoReceive();
  //handle HHTP P2P client
  server.handleClient();
}
//Creating access pointof the alive system
void createAliveAp()
{
  //Reading EEPROM for alivessid(0-32) and alivepass(32-64)
  String alivessid = "";
  for (int i = 0; i < 32; ++i)
  {
    alivessid += char(EEPROM.read(i));
  }
  String alivepass = "";
  for (int i = 32; i < 64; ++i)
  {
    alivepass += char(EEPROM.read(i));
  }
  // Creating access point with the stored name of EEPROM is not empty else creating AP with default name
  if ( alivessid != NULL)
  {
    WiFi.softAP(alivessid.c_str(), alivepass.c_str());
  }
  else
  {
    WiFi.softAP(dalivessid, dalivepass);
  }
  //Starting HTTP server with handleRoot function as callback function
  server.on("/index", handleRoot);
  server.begin();
  server.onNotFound ( handleNotFound );
}
// Connecting hardware to server
void connectToServer()
{
  //Deconstructor for WiFiMulti to remove stored settings
  WiFiMulti.~ESP8266WiFiMulti();
  //Reading wifi credentials from EEPROM ssid(64-96) and pass(96-128)
  wifissid = "";
  for (int i = 64; i < 96; ++i)
  {
    wifissid += char(EEPROM.read(i));
  }
  wifipass = "";
  for (int i = 96; i < 128; ++i)
  {
    wifipass += char(EEPROM.read(i));
  }
  //If EEPROM is not empty connect to wifi and establish websocket connection 
  if ( wifissid != NULL)
  {
    WiFiMulti.addAP(wifissid.c_str(), wifipass.c_str());
   
    if (WiFiMulti.run() != WL_CONNECTED) {
      delay(1000);
    }
    //Starting websocket connection with callback function websocketEvent
    webSocket.begin(ipServer, portServer);
    //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
    webSocket.onEvent(webSocketEvent);
  }
}

//Websocket Event Call Back Function
void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght)
{
  switch (type) {
    case WStype_DISCONNECTED:
    // Turn the tryconnect flag to false after waitcount approaches 400
      if (waitCount < 400)
        waitCount += 100;
      else
        tryConnect = false;
      break;
   // One connected when hardwareReset flag is set register to server and get status from server and turn the flag false
   //Else register and send to server the current status
    case WStype_CONNECTED:
      waitCount = 0;
      if (hardwareReset == true)
      {
        webSocket.sendTXT("E-R-" + productId);
        webSocket.sendTXT("E-S-" + productId);
        hardwareReset = false;
      }
      else
      {
        webSocket.sendTXT("E-R-" + productId);
        webSocket.sendTXT(currentStatus);
      }
      break;
    //Send message as it is to arduino from server
    case WStype_TEXT:
      arduinoSend((char *)payload);
      break;
    case WStype_BIN:
      hexdump(payload, lenght);
      break;
  }
}

//Send Message to Arduino
void arduinoSend(char *payload)
{
  uc_SERIAL.println(payload);
  uc_SERIAL.flush();
}

//Function to receive messages from arduino
String arduinoReceive()
{
  String str = "";
  while (Serial.available() > 0)
  {
    char temp =  (char)Serial.read();
    if (temp == '~')
    {
      str += temp;
      break;
    }
    else
    {
      str += temp;
    }
    delay(2);
  }
  //Send all messages as its to server and store the status message in the current status variable
  if (str != "")
  {
    char *cstr = new char[str.length() + 1];
    strcpy(cstr, str.c_str());
    if (cstr[2] == 'E')
    {
      currentStatus = cstr;
    }
    else
    {
      webSocket.sendTXT(cstr);
    }
    return str;
  }
  return "";
}
// Calback function for HTTP client
void handleRoot() {
  if (server.hasArg("TYPE"))
  {
    hardwareReset = false;
    if (server.arg("TYPE") == "SETUP")
    {
      String argalivename = server.arg("ALIVENAME");
      String argalivepass = server.arg("ALIVEPASS");
      String argwifiname = server.arg("WIFINAME");
      String argwifipass = server.arg("WIFIPASS");
      if (argalivename.length() > 0 && argalivepass.length() >= 8)
      {
        writeAliveCredentials(argalivename, argalivepass);
      }
      else
      {
        server.send(200, "text/plain", "Incorrect Alive System Credentials");
      }
      if (argwifiname.length() > 0 && (argwifipass.length() >= 8 || argwifipass.length() == 0))
      {
        writeWifiCredentials(argwifiname, argwifipass);
      }
      else
      {
        server.send(200, "text/plain", "Incorrect Credentials of Wi-Fi System");
      }
      //Serial.println("Reset Credentials EEPROM" + String(argalivename.length()) + " " + String(argwifiname.length()));
      if (argalivename.length() == 0 && argwifiname.length() == 0)
      {
        for (int i = 0; i < 128; i++)
          EEPROM.write(i, 0);
        EEPROM.commit();
      }
      server.send(200, "text/plain", "SETUP SUCCESS");
      ESP.reset();
    }
    else if (server.arg("TYPE") == "CONTROL")
    {
      //Send Message to arduino
      arduinoSend((char *)server.arg("MESSAGE").c_str());
      server.send(200, "text/plain", "CONTROL SUCCESS");
    }
    else if (server.arg("TYPE") == "STATUS")
    {
      String str = "S" + currentStatus.substring(4 + productId.length()) + "-~";
      char *cstr = new char[str.length() + 1];
      strcpy(cstr, str.c_str());
      server.send(200, "text/plain", cstr);
    }
    else if (server.arg("TYPE") == "RETRY")
    {
      tryConnect = true;
      scount = 0;
      waitCount = 0;
    }
    else
    {
      server.send(200, "text/plain", "Wrong Arguments");
    }
  }
}

//Callback function if handle not found
void handleNotFound() {
  server.send ( 404, "text/plain", "File Not Found" );
}
//Functon to write alive system credentials on EEPROM while setting up using P2P
void writeAliveCredentials(String argname, String argpass)
{
  for (int i = 0; i < 64; ++i)
  {
    EEPROM.write(i, 0);
  }
  for (int i = 0; i < argname.length(); ++i)
  {
    EEPROM.write(i, argname[i]);
  }
  for (int i = 0; i < argpass.length(); ++i)
  {
    EEPROM.write(32 + i, argpass[i]);
  }
  EEPROM.commit();   
}
//Function to write WiFi credentials on EEPROM while setting up through P2P
void writeWifiCredentials(String argname, String argpass)
{
  for (int i = 64; i < 128; ++i)
  {
    EEPROM.write(i, 0);
  }
  for (int i = 0; i < argname.length(); ++i)
  {
    EEPROM.write(64 + i, argname[i]);
  }
  for (int i = 0; i < argpass.length(); ++i)
  {
    EEPROM.write(96 + i, argpass[i]);
  }
  EEPROM.commit();
}

