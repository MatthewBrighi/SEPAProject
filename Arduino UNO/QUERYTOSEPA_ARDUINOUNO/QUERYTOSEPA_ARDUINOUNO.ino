/*
 Matteo Brighi 05/12/2020 Scheda: Arduino UNO
 Programma per inviare una query in http post ad un server sepa locale
*/
 
#include <SPI.h>
#include <ArduinoHttpClient.h>
#include <Ethernet.h>

byte mac[]{0xAA, 0x01, 0x02, 0x03, 0xFA, 0x90};
IPAddress ips(192, 168, 1, 200);

char serverAddress[] = "192.168.1.76";  // server address
int port = 8000;

EthernetClient http;
HttpClient client = HttpClient(http, serverAddress, port);

void setup() {
  //Serial monitor Initializzation
  Serial.begin(9600);

  //Ethernet Initializzation
  Serial.println("Ethernet configuration");
  if(Ethernet.begin(mac)==0){
    Serial.println("Error whit the DHCP Configuration.....Initializzation on IP:");
    Ethernet.begin(mac, ips);
  }

  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  //HTTP query
  Serial.println("Start the Sepa query http post");
  
  String contentType = "application/sparql-query";
  String postData = "SELECT * FROM <http://test> WHERE {?s ?p ?o}";

  client.post("/query", contentType, postData);

  Serial.println("Wait for response....");
}

void loop() {
   //numbers of recived byte from SEPA
   int len = http.available();

   //let'z print the received message
    if(len > 0){
      if(len>400) len=400;
      byte buffer[len];
      http.read(buffer,len);
      Serial.write(buffer, len);
      }

   Serial.println();

  //close the connection 
  if(!http.connected()){
    http.stop();
    Serial.println("Disconnected");
    while(1) ;
  }
}
