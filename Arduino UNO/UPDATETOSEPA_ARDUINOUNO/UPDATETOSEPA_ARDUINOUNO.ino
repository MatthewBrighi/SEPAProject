/*
  Matteo Brighi 05/12/2020 Scheda : ArduinoUNO 
  Invio di un UPDATE a server Sepa locale post, inviando un dato numerico contenuto nella variabile i
 */
#include <SPI.h>
#include <Ethernet.h>
#include <ArduinoHttpClient.h>

byte mac[]{0xAA, 0x01, 0x02, 0x03, 0xFA, 0x90};
IPAddress ips(192, 168, 1, 200);

char serverAddress[] = "192.168.1.76";  // server address
int port = 8000;

EthernetClient http;
HttpClient client = HttpClient(http, serverAddress, port);

//Data to send
float i = 200.23;

void setup() {
  Serial.begin(9600);

  Serial.println("Let's start Ethernet Connection");

  //Ethernet Configuration
  if(Ethernet.begin(mac)==0){
    Serial.println("DHCP Failed....Begin manualy on ");
    Ethernet.begin(mac,ips);
  }

  // Print the Ip Address of the client
  IPAddress ip = Ethernet.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println("Make the UPDATE");
  String contentType = "application/sparql-update";
  String postData = "INSERT DATA { GRAPH <http://test> { <http://a> <http://b> " + String(i) + " } }";

  client.post("/update", contentType, postData);
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
