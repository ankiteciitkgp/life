// This function changes the relay state according to payload recieved and then also sends the confirmation msg back.

void changeRelayStateThenSend(char switch_num, char state) {   
  String msg = "E-U-U111-";
  
  if (state == '1') {
    digitalWrite(relay_num[(switch_num - '0') - 1], LOW);   //(switch_num - '0')  converts char to int and as the logic of relay is negative I made the pin to low
    relay_state[switch_num - '0'] = true;
    sendToServer(msg + switch_num +"-"+state);  //E-U-PID- Swithno-State
    Serial.flush();
    eatmillis(150);
  }
  else if (state == '0') {
    digitalWrite(relay_num[(switch_num - '0') - 1], HIGH);   //(switch_num - '0')  converts char to int and as the logic of relay is negative I made the pin to low
    relay_state[switch_num - '0'] = false;
    sendToServer(msg + switch_num +"-"+state);                 //E-U_PID- Swithno-State
    Serial.flush();
    eatmillis(150);
  }




}

