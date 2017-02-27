//This function just sends anything to server  for more on how this method work visit: http://stackoverflow.com/questions/8125507/how-can-i-send-and-receive-websocket-messages-on-the-server-side

void sendToServer(String toSend) {
//  debug.println("sending to server --> "+toSend);
//  debug.flush();
  int len=toSend.length();
  String final_cipsend = "AT+CIPSEND=0," + String(len + 6);
  byte headers[6];
  byte data[len+1];
  headers[0] = 129;
  headers[1] = toSend.length() | 128;
  for (int i = 0; i < 4; i++) {
    headers[i + 2] = random(10, 80);
  }
  Serial.println(final_cipsend);
  eatmillis(20);
   toSend.getBytes(data,len+1);

   for(int i=0;i<6;i++){
    Serial.write(headers[i]);
    
   }
    for(int i=0;i<len;i++){
      byte encrypted= data[i]^headers[(i%4)+2];
      Serial.write(encrypted);
      
    }
   // eatmillis(50);
   delay(10);
}

