#include <ESPping.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>

#include "secrets.h" 

WiFiUDP UDP;
WakeOnLan WOL(UDP);

void wakeUp(const char* macAddress) {
  WOL.sendMagicPacket(macAddress);
}

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Starting network monitor...");
}

void loop() {
  for (int i = 0; i < num_targets; i++) {
    Serial.print("Checking ");
    Serial.print(targets[i].name);
    Serial.print(" (");
    Serial.print(targets[i].ip);
    Serial.print(")... ");

    bool isOn = Ping.ping(targets[i].ip, 5);
    
    if (!isOn) {
      Serial.println("Status: OFF. Sending WoL magic packet...");
      wakeUp(targets[i].mac);
    } else {
      Serial.println("Status: ON.");
    }

    delay(1000); 
  }

  Serial.println("Cycle complete. Waiting before next check...");

  delay(30000); // Wait 30 seconds before the next check
}