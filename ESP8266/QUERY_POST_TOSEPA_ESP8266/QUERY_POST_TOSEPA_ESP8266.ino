/*
  Matteo Brighi 30/11/2020 Query POST SEPA  Scheda : ESP8266
  Programma per effettuare una QUERY di tipo POST al server SEPA. Viene poi mostrata a schermo la risposta del server
*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define SERVER_IP "192.168.1.74:8000"  //local Sepa Server IP, whit his localPort for Query

#ifndef STASSID
#define STASSID "dlink-F5E9"  //your access point SSID
#define STAPSK  "familiabrighi"  //your accesspoint password key
#endif

void setup() {

  //inizializzazione della seriale
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  //WiFi initializzation
  WiFi.begin(STASSID, STAPSK);

  //attend the connection beginning
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  //when the connession is start, print de client IP Address
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    //create a WiFi HTTP client
    WiFiClient client;
    HTTPClient http;

    //connection to the local server, whit an UPDATE Request
    http.begin(client, "http://" SERVER_IP "/query"); //HTTP
    http.addHeader("Content-Type", "application/sparql-query");

    //Http Body whit the Sparql code for query request
    int httpCode = http.POST("SELECT * FROM <http://test> WHERE {?s ?p ?o}");

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // server response
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {  //in case of error, print the error code
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end(); //ending the HTTP Client
  }
  delay(10000); //wait for 10 second before do another update
}
