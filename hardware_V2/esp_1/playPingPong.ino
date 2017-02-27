
void playPingPong(int mode) {
  /************************
  * mode=1 --> online
  * mode=0 --> offline
  *************************/
  if(mode==1)
  {
    if(counter >= 200 )
    { 
      if(wait==0||wait>=30)
        sendToServer("E-P-U111");
      
    }
    if(wait>200)
      setup();
  }
  else if(mode==0)
  {
    //Do nothing for now.
  }
}



