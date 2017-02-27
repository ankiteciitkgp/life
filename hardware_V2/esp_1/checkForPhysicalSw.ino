//still to be implemted

void checkForPhysicalSw() {
  for (int i = 0; i < 4; i++) {
    if ((digitalRead(physical_sw[i]) == HIGH) && (relay_state[i] == false)) {
      digitalWrite(relay_num[i], LOW);
 //     debug.println(i+" is ON manually");
    }
    else if ((digitalRead(physical_sw[i]) == HIGH) &&  (relay_state[i] == true)) {
      digitalWrite(relay_num[i], HIGH);
 //     debug.println(i+" is OFF manually");
    }
  }

}

