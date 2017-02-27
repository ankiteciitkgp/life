void ConnectToServer(String url, String port) {   //working good
//  debug.println("Connecting to "+url+":"+port);
//  debug.flush();
  String cmd_cipstart = "AT+CIPSTART=0,\"TCP\"";      // here we are making the cipstart command "AT+CIPSTART=0,"TCP","url",port"
  String cipsend = "AT+CIPSEND=0,";                    // this is for cipsend command "AT+CIPSEND=0,len(msgToBeSent)" 

  //AT+CIPSTART command formation with arguments provided
  String final_cipstart = cmd_cipstart + String(",") + String("\"") + url + String("\"") + String(",") + port;
  Serial.println(final_cipstart);
  Serial.flush();
  eatmillis(200);
  
  delay(500); //  VERY FUCKING!!!! important delay of 500ms #magicNumber


  //AT+CIPSEND command formation with arguments provided
  String final_cipsend = cipsend + String(url.length() + 144);   // in this like 144 is the number of bytes in the msg to be sent i.e. headers
  Serial.println(final_cipsend);
  Serial.flush();
  eatmillis(50);

// headers for websocket connection

  Serial.println("GET / HTTP/1.1");
  delay(2);
  Serial.println("Host: " + url + String(":") + port);
   delay(2);
  Serial.println("Upgrade: websocket");
   delay(2);
  Serial.println("Connection: Upgrade");
   delay(2);
  Serial.println("Sec-WebSocket-Key: dGhlIHNhbXBsZSBub25jZQ==");
   delay(2);
  Serial.println("Sec-WebSocket-Version: 13");
  Serial.println();
//  eatmillis(30);
}

