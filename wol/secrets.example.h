#pragma once
#include <Arduino.h>

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

struct TargetDevice {
  IPAddress ip;
  const char* mac;
  const char* name;
};

TargetDevice targets[] = {
  { IPAddress(192, 168, 0, 2), "AA:BB:CC:DD:EE:FF", "Desktop PC" },
  { IPAddress(192, 168, 0, 3), "AA:BB:CC:DD:EE:FF", "Living Room HTPC" }
};

const int num_targets = sizeof(targets) / sizeof(targets[0]);