#include <ESPping.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <WakeOnLan.h>

#include "secrets.h"

WiFiUDP UDP;
WakeOnLan WOL(UDP);

const unsigned long WIFI_ATTEMPT_TIMEOUT = 20000;   // 20 s per attempt
const unsigned long WIFI_GIVEUP_RESTART  = 180000;  // restart after 3 min without WiFi
const unsigned long CHECK_INTERVAL       = 30000;   // 30 s between cycles

unsigned long lastCycle = 0;

void wakeUp(const char* macAddress) {
  WOL.sendMagicPacket(macAddress);
}

void startWiFi() {
  WiFi.disconnect(true, true);
  delay(500);
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.setAutoReconnect(true);
  WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS);
  WiFi.begin(ssid, password);
}

// Blocks until connected. Retries, and restarts the ESP if it takes too long.
void connectWiFi() {
  Serial.print("Connecting to WiFi");
  unsigned long totalStart = millis();
  unsigned long attemptStart = millis();
  startWiFi();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");

    if (millis() - totalStart > WIFI_GIVEUP_RESTART) {
      Serial.println("\nWiFi not available, restarting ESP32...");
      delay(200);
      ESP.restart();
    }

    if (millis() - attemptStart > WIFI_ATTEMPT_TIMEOUT) {
      Serial.println("\nAttempt timed out, retrying...");
      startWiFi();
      attemptStart = millis();
    }
  }

  Serial.println("\nWiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  delay(100);

  connectWiFi();
  Serial.println("Starting network monitor...");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi lost, reconnecting...");
    connectWiFi();
  }

  if (lastCycle != 0 && millis() - lastCycle < CHECK_INTERVAL) {
    delay(100);
    return;
  }
  lastCycle = millis();

  for (int i = 0; i < num_targets; i++) {
    if (WiFi.status() != WL_CONNECTED) break;

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
}
