//To handle regulator state and check whether physical regulator button is pressed
void regulatorLoop()
{
  for (int i = 0; i < regulators; i++)
  {
    if (i == 1)
    {
      int sensorValue = analogRead(regulatorOut[i]);
      float voltage = sensorValue * (5.0 / 1023.0);
      if (voltage > 1 && voltage < 3)
      {
        if (dimLevel[i] < 5)
        {
          dimLevel[i] = dimLevel[i] + 1;
          sendToEsp("E-U-" + productId + "-" + String(i + 5) + "-" + String(5 - dimLevel[i]));
          sendStatus();
        }
      }
      else if (voltage < 1)
      {
        if (dimLevel[i] > 0)
        {
          dimLevel[i] = dimLevel[i] - 1;
          sendToEsp("E-U-" + productId + "-" + String(i + 5) + "-" + String(5 - dimLevel[i]));
          sendStatus();
        }
      }
    }
    else
    {
      int sensorValue = analogRead(regulatorOut[i]);
      float voltage = sensorValue * (5.0 / 1023.0);
      if (voltage > 1 && voltage < 3)
      {
        if (dimLevel[i] > 0)
        {
          dimLevel[i] = dimLevel[i] - 1;
          sendToEsp("E-U-" + productId + "-" + String(i + 5) + "-" + String(5 - dimLevel[i]));
          sendStatus();
        }
      }
      else if (voltage < 1)
      {
        if (dimLevel[i] < 5)
        {
          dimLevel[i] = dimLevel[i] + 1;
          sendToEsp("E-U-" + productId + "-" + String(i + 5) + "-" + String(5 - dimLevel[i]));
          sendStatus();
        }
      }
    }

    //Caliberation for smooth dimming
    if (dimLevel[i] == 0)
      analogWrite(regulatorIn[i], 0);
    else if (dimLevel[i] == 1)
      analogWrite(regulatorIn[i], 64);
    else if (dimLevel[i] == 2)
      analogWrite(regulatorIn[i], 96);
    else if (dimLevel[i] == 3)
      analogWrite(regulatorIn[i], 128);
    else if (dimLevel[i] == 4)
      analogWrite(regulatorIn[i], 160);
    else if (dimLevel[i] == 5)
      analogWrite(regulatorIn[i], 255);

  }
}

