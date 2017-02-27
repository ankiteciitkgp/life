//To check and receive messages from RX and calls processPayload
void checkReceive()
{
  String str = "";
  while(Serial.available())
  {
    str += (char)Serial.read();
    delay(2);
  }
  if(str != "")
  {
    processPayload(str);
  }
}

