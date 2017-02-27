void setupEsp() {
  //  debug.println("Setting Up wiFi Module...");
  Serial.println("ATE0");
  Serial.flush();
  eatmillis(50);
  Serial.println("AT+CWMODE=3");
  Serial.flush();
  eatmillis(50);
  Serial.println("AT+CIPMUX=1");
  Serial.flush();
  eatmillis(50);
  Serial.println("AT+CWJAP=\"STUDENT-1\",\" \"");
  Serial.flush();
  while (!Serial.find("OK")); //wait till it gets connected
  Serial.println("AT+CWSAP=\"AliveHomes\",\"123456789\",5,3");
  Serial.flush();
  while (!Serial.find("OK"));
  Serial.println("AT+CIPSERVER=1,8383");
  Serial.flush();
  while (!Serial.find("OK"));
  eatmillis(100);
  //delay(50);
  eatmillis(100);
}

