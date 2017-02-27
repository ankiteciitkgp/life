// here is most of the payload processing goes on here we extarcts exact chars from payload and do the action.

void processPayload() {

  if ((payload_str[2] == 'S') && (payload_str[4] == '4')) {
    counter = 0;
    wait = 0;
    for (int i = 0; i < 8; i += 2) {
      if (payload_str[i + 6] == '1') {
        digitalWrite(relay_num[i / 2], LOW);
        relay_state[i] = true;
      }
      if (payload_str[i + 6] == '0') {
        digitalWrite(relay_num[i / 2], HIGH);
        relay_state[i] = false;
      }
    }
  }

  if ((payload_str[2] == 'S') && (payload_str[4] == 'P')) {
    counter = 0;
    wait = 0;

  }
  if ((payload_str[2] == 'S') && (payload_str[4] == 'C')) {
    counter = 0;
    wait = 0;
    if ((payload_str[6] - '0') <= 4)
    {
      changeRelayStateThenSend(payload_str[6], payload_str[8]);
    }
    else
    {
      dim[payload_str[6] - '0' - 5] = 5 - payload_str[8] + '0';
      sendToServer("E-U-U111-" + String(payload_str[6] - '0') + "-" + String(5 - dim[payload_str[6] - '0' - 5]));
      //Serial.println(String(payload_str[6]-'0'-5)+" " +String(5-payload_str[8]+'0') );
    }
  }



  for (int i = 0; i < 20; i++)
    payload_str[i] = '0';
}

