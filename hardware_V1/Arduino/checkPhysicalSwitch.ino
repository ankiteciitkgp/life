//Checks for any change in physical switch state and send it so server
void checkPhysicalSwitch()
{
  for(int i=0; i< switches; i++)
  {
    int currentState = digitalRead(switchOut[i]);
    if(booleanToint(switchState[i]) != currentState)
    {
      switchState[i] = !switchState[i];   
      sendToEsp("E-U-"+ productId+"-"+String(i+1)+"-"+String(currentState));
      sendStatus();
    }
  }
}

