//Defining pin modes of all the used pins
//For now Considering all switches will start with state off
void setupSwitches()
{
  for (int i = 0 ; i < switches; i++)
  {
    pinMode(switchIn[i], OUTPUT);
    digitalWrite(switchIn[i], HIGH);
    pinMode(switchOut[i], INPUT);
  }

  for (int i = 0; i < regulators; i++)
  {
    pinMode(regulatorIn[i], OUTPUT);
    pinMode(regulatorOut[i], INPUT);
  }
  delay(10);
}

