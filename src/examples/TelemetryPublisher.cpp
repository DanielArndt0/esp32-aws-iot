#include "TelemetryPublisher.h"

#include <ArduinoJson.h>

#include "AppConfig.h"

TelemetryPublisher::TelemetryPublisher(AwsIotClient &mqtt) : mqtt_(mqtt) {}

void TelemetryPublisher::begin(const char *thingName)
{
  thingName_ = thingName;
  topic_ = String("devices/") + thingName + "/telemetry";
  lastPublished_ = millis() - AppConfig::kTelemetryEveryMs;
}

void TelemetryPublisher::loop(uint32_t now)
{
  if (!mqtt_.connected() ||
      now - lastPublished_ < AppConfig::kTelemetryEveryMs)
  {
    return;
  }

  lastPublished_ = now;
  StaticJsonDocument<256> document;
  document["thing"] = thingName_;
  document["uptime_seconds"] = now / 1000;
  document["free_heap_bytes"] = ESP.getFreeHeap();
  document["sequence"] = ++sequence_;

  char payload[256];
  const size_t length = serializeJson(document, payload, sizeof(payload));
  if (length == 0 || length >= sizeof(payload) - 1)
  {
    Serial.println("MQTT: payload too large.");
    return;
  }

  Serial.println(mqtt_.publish(topic_.c_str(), payload)
                     ? "MQTT: telemetry published."
                     : "MQTT: publish failed.");
}
