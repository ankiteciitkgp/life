// This method gets fired up when a serial event happens. It stores all the Recieved in a variable name = str. 
// Then we tokenize the string with delimeter as ":" and points to it by pointer named 'ch'.
// finally that string is transfered to payload_str global variable.

void checkForRcv() {
  // Recieving if any msg
  str = "";
  while (Serial.available()) {

    str += (char)Serial.read();  // now str contains our complete msg (from server+esp) (SEND OK...+IPD...)
  }
///  debug.println("str?-->" + str);
///debug.flush();

  //Extracting useful data from msg recieved
  ch = strtok(&str[0], ":");
  ch = strtok(NULL, ":"); // ch is the pointer to an String with first element as size of data string

  if (ch != NULL) {
    strcpy(payload_str, ch); // now payload_str contains the latest msg String in it
    str = "";
// We should make ch also null in here.

    
//    debug.print("Msg Revcd-->");
//    debug.print(payload_str);
//    debug.println("<--");
//    debug.flush();
  }
}

