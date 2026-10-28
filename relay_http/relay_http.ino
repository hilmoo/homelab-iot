#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "secrets.h"

ESP8266WebServer server(80);

const int pin = 0;

void TurnOn(){
  digitalWrite(pin, HIGH);
  server.send(200, "text/plain", "Timmy is now wake up");
}

void TurnOff(){
  digitalWrite(pin, LOW);
  server.send(200, "text/plain", "Timmy is now sleep");
}

void NotFound(){
  server.send(404, "text/plain", "404 Error: Endpoint not found.");
}

void setup() {
  Serial.begin(115200);

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);

  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);
  
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");

  server.on("/on", HTTP_GET, TurnOn); 
  server.on("/off", HTTP_GET, TurnOff); 
  server.onNotFound(NotFound);

  server.begin();
}

void loop() {
  server.handleClient();
}
