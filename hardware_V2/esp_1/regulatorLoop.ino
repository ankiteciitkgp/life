void regulatorLoop(int reg)
{
  String msg = "E-U-U111-";
  int sensorValue = analogRead(regIn[reg]);
  float voltage = sensorValue * (5.0 / 1023.0);
  if (voltage > 1 && voltage < 3)
  {
    if (dim[reg] < 5)
    {
      dim[reg] = dim[reg] + 1;
      sendToServer(msg + String(reg + 5) + "-" + String(5 - dim[reg]));
    }
  }
  else if (voltage < 1)
  {
    if (dim[reg] > 0)
    {
      dim[reg] = dim[reg] - 1;
      sendToServer(msg + String(reg + 5) + "-" + String(5 - dim[reg]));
    }
  }
  //Serial.println(dim[reg] );
  if (dim[reg] == 0)
    analogWrite(regOut[reg], 0);
  else if (dim[reg] == 1)
    analogWrite(regOut[reg], 64);
  else if (dim[reg] == 2)
    analogWrite(regOut[reg], 96);
  else if (dim[reg] == 3)
    analogWrite(regOut[reg], 128);
  else if (dim[reg] == 4)
    analogWrite(regOut[reg], 160);
  else if (dim[reg] == 5)
    analogWrite(regOut[reg], 255);
}
