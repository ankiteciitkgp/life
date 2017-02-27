//Send Status to Server
void sendStatus()
{
  String str = "E-E-" + productId +"-" +String(switches + regulators);
  for (int i = 0 ; i < switches; i++)
  {
      str+= "-" + String(booleanToint(switchState[i]));
  }
  for (int i = 0 ; i < regulators; i++)
  {
    int level = 5- dimLevel[i];
      str+= "-" + String(level);
  }
  sendToEsp(str);
}

