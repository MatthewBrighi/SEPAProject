/*Matteo Brighi 30/11/2020 SUBRCIBE TO SEPA Scheda:ESP8266
 * Programma per effettuare una sottoscizione ad un certo dato del server SEPA. Riceve le notifiche ogni volta che viene aggiunto un nuovo 
 * valore, diverso da quelli inviati in precedenza*/


#include <ArduinoWebsockets.h>
#include <ESP8266WiFi.h>

const char* ssid = "dlink-F5E9"; //Enter SSID
const char* password = "familiabrighi"; //Enter Password
const char* websockets_server_host = "192.168.1.76"; //Enter server adress
const uint16_t websockets_server_port = 9000; // Enter server port

using namespace websockets;

WebsocketsClient client;
void setup() {
    Serial.begin(115200);
    
    // Connect to wifi
    WiFi.begin(ssid, password);

    // Wait some time to connect to wifi
    for(int i = 0; i < 10 && WiFi.status() != WL_CONNECTED; i++) {
        Serial.print(".");
        delay(1000);
    }

    // Check if connected to wifi
    if(WiFi.status() != WL_CONNECTED) {
        Serial.println("No Wifi!");
        return;
    }

    Serial.println("Connected to Wifi, Connecting to server.");
    
    // try to connect to Websockets server
    bool connected = client.connect(websockets_server_host, websockets_server_port, "/subscribe"); //connetcion to the serve whit subribe mode
    if(connected) {
        Serial.println("Connecetd!");
        client.send("{'subscribe' : {'sparql' : 'select * from <http://test> where { ?s ?p ?o }'}}"); //subsription request
    } else {
        Serial.println("Not Connected!");
    }
    
    // run callback when messages are received
    client.onMessage([&](WebsocketsMessage message) {
        Serial.print("Got Message: ");
        Serial.println(message.data());
    });
}

void loop() {
    // let the websockets client check for incoming messages
    if(client.available()) {
        client.poll();
    }
    delay(500);
}
