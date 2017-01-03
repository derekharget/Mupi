/*
 * 
 * Created as an inexpensive outdoor lighting control
 * 
 */

#include <SPI.h>
#include <Ethernet.h>
String readString;
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();

    for(int i=2;i<=9;i++){
    relay(i,2);
  }
}


void loop() {

         for (int relayList = 2; relayList <= 9; relayList++) {
            String off = String(relayList) +"/off";
            String on = String(relayList) +"/on";
           if (readString.indexOf("relay/" + off) >0){
               relay(relayList,0);
           }
           if (readString.indexOf("relay/"+on) >0){
               relay(relayList,1);
           }
           } 
           
          readString="";

  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;



    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();



       //read char by char HTTP request
        if (readString.length() < 100) {
          //store characters to string
          readString += c;
          //Serial.print(c);
         }



        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          for (int relaylist = 2; relaylist <= 9; relaylist++) {
            int relaych = digitalRead(relaylist);
            String strRelay = "Relay "+String(relaylist);
            client.print(strRelay+": ");
            if(relaych == 1){
              String Offlist = String(relaylist)+"/on\">Off</a>";
              client.print("<a href=\"/relay/"+Offlist);
            }else{
              String Onlist = String(relaylist) + "/off\">On</a>";
              client.print("<a href=\"/relay/"+Onlist);
            }
            client.println("<br />");
          }
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  
/*           for (int relayList = 2; relayList <= 9; relayList++) {
            String off = String(relayList) +"/off";
            String on = String(relayList) +"/on";
           if (readString.indexOf("relay/" + off) >0){
               relay(relayList,0);
           }
           if (readString.indexOf("relay/"+on) >0){
               relay(relayList,1);
           }
           } */

  }
}


void relay(int pin, int toggle){

  switch(toggle){
    case 0: //Shutoff Relay
      digitalWrite(pin, HIGH);
      break;
  
   case 1: //Open Relay
     digitalWrite(pin, LOW);
     break;
    case 2:
      pinMode(pin, INPUT_PULLUP);
      pinMode(pin, OUTPUT);
      digitalWrite(8, HIGH);
      break;
     case 3:
      if(digitalRead(pin)){
        //return digitalRead(pin);
      }
     break;
  }
}



