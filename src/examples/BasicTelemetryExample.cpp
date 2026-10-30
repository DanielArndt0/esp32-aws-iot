#include "BasicTelemetryExample.h"

#include <Arduino.h>
#include <time.h>

#include "AppConfig.h"
#include "ExampleConfig.h"
#include "TelemetryPublisher.h"
#include "../aws/AwsIotClient.h"
#include "../network/WifiConnection.h"
#include "secrets.h"

namespace
{
  AwsIotClient aws;
  WifiConnection wifi;
  TelemetryPublisher telemetry(aws);
  bool configured = false;
  bool wifiWasReady = false;
  bool clockRequested = false;
  uint32_t lastMqttAttempt = 0;
  uint32_t lastClockLog = 0;
}

void BasicTelemetryExample::setup()
{
  Serial.begin(AppConfig::kSerialBaud);
  delay(100);
  Serial.println("ESP32 AWS IoT Core MQTT example");

  configured = ExampleConfig::valid(
      Secrets::kWifiSsid, Secrets::kWifiPassword,
      Secrets::kAwsEndpoint, Secrets::kThingName,
      Secrets::kRootCaPem, Secrets::kDeviceCertificatePem,
      Secrets::kPrivateKeyPem);
  if (!configured)
  {
    Serial.println("Configuration incomplete: edit include/secrets.h.");
    return;
  }

  aws.begin(Secrets::kAwsEndpoint, Secrets::kThingName,
            Secrets::kRootCaPem, Secrets::kDeviceCertificatePem,
            Secrets::kPrivateKeyPem);
  telemetry.begin(Secrets::kThingName);
  wifi.begin(Secrets::kWifiSsid, Secrets::kWifiPassword);
  lastMqttAttempt = millis() - AppConfig::kMqttRetryMs;
}

void BasicTelemetryExample::loop()
{
  if (!configured)
  {
    delay(1000);
    return;
  }

  const uint32_t now = millis();
  const bool wifiReady = wifi.maintain(now);
  if (!wifiReady)
  {
    if (wifiWasReady)
    {
      aws.disconnect();
      clockRequested = false;
    }
    wifiWasReady = false;
    delay(10);
    return;
  }

  wifiWasReady = true;
  if (!clockRequested)
  {
    configTime(0, 0, "pool.ntp.org", "time.google.com");
    clockRequested = true;
  }

  if (time(nullptr) < AppConfig::kMinimumUnixTime)
  {
    if (now - lastClockLog >= AppConfig::kClockLogEveryMs)
    {
      lastClockLog = now;
      Serial.println("NTP: waiting for clock synchronization...");
    }
    delay(10);
    return;
  }

  if (!aws.connected() &&
      now - lastMqttAttempt >= AppConfig::kMqttRetryMs)
  {
    lastMqttAttempt = now;
    aws.connect();
  }

  aws.loop();
  telemetry.loop(now);
  delay(10);
}
