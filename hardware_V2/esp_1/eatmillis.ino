// Well at times we recieve incoming serial data which we doesnot need, but that can cuase communication problems so this methods gets rid of that incoming serial data. 
//just give some number for time in millis you want to get rid of in arguments

void eatmillis(byte count) {   
  

  unsigned long time = millis();
  while ((millis() - time) < count)
  {
    if(Serial.available())
   Serial.read();


  }


}
