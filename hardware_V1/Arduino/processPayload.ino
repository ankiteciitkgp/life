//To process messages received from ESP8266
void processPayload(String payload)
{

  if(payload[0] == 'S' || payload[0] == 'P')
  {
    if(charToint(payload[2]) == switches + regulators)
    {
      for(int i=0;i<switches;i++)
      {
        int serverState = charToint(payload[4+2*i]);
        int currentState = booleanToint(switchState[i]);
        if(currentState != serverState)
        {
          toggleSwitch(i);
        }
      }
      for(int i=0;i<regulators;i++)
      {
        dimLevel[i] = 5 - charToint(payload[4+2*switches+2*i]);  
        // 5- is to take care of inverting logic of Switch state on Server 0 is off 
      } 
    }
    else if(payload[2] == 'C')
    {
      if(charToint(payload[4])<=switches)
      {
        int switchnum = charToint(payload[4])-1;
        int currentState = booleanToint(switchState[switchnum]);
        if(currentState != charToint(payload[6]))
        {
          toggleSwitch(switchnum);
          switchState[switchnum] = !switchState[switchnum];
        }
        sendToEsp("E-U-"+ productId+"-"+String(switchnum+1)+"-"+String(switchState[switchnum]));
        sendStatus();
        
      }
      else
      {
        int level = 5- charToint(payload[6]);               //Logic Inversion of Level
        
        dimLevel[charToint(payload[4])-switches-1] = level;   
        //Sending Message of form E-U-U111-5-4;
        sendToEsp("E-U-" + productId + "-" + String(payload[4]) + "-" + String(5-level));
        sendStatus();
      }
    }
  }
}

