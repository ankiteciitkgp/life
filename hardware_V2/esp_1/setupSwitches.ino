void setupSwitches() {
  pinMode(relay_num[0], OUTPUT);
  digitalWrite(relay_num[0], HIGH);
  pinMode(relay_num[1], OUTPUT);
  digitalWrite(relay_num[1], HIGH);
  pinMode(relay_num[2], OUTPUT);
  digitalWrite(relay_num[2], HIGH);
  pinMode(relay_num[3], OUTPUT);
  digitalWrite(relay_num[3], HIGH);
  pinMode(physical_sw[0], INPUT);
  pinMode(physical_sw[1], INPUT);
  pinMode(physical_sw[2], INPUT);
  pinMode(physical_sw[3], INPUT);
  pinMode(regOut[0], OUTPUT);
  pinMode(regOut[1], OUTPUT);
  pinMode(regIn[0], INPUT);
  pinMode(regIn[1], INPUT);
  delay(10);
}

