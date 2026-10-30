#include "WifiConnection.h"

#include <Arduino.h>
#include <WiFi.h>

#include "AppConfig.h"

void WifiConnection::begin(const char *ssid, const char *password)
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  lastAttempt_ = millis();
}

bool WifiConnection::maintain(uint32_t now)
{
  const bool connected = WiFi.status() == WL_CONNECTED;

  if (connected && !wasConnected_)
  {
    Serial.println("Wi-Fi: connected.");
  }

  if (!connected && wasConnected_)
  {
    Serial.println("Wi-Fi: connection lost.");
  }

  if (!connected && now - lastAttempt_ >= AppConfig::kWifiRetryMs)
  {
    lastAttempt_ = now;
    Serial.println("Wi-Fi: reconnecting...");
    WiFi.reconnect();
  }

  wasConnected_ = connected;
  return connected;
}
