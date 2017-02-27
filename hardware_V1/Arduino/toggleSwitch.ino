void toggleSwitch(int switchNumber)
{
  digitalWrite(switchIn[switchNumber], LOW);
  delay(10);
  digitalWrite(switchIn[switchNumber], HIGH);
}

