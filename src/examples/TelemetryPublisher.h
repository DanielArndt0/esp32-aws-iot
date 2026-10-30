#pragma once

#include <Arduino.h>
#include <stdint.h>

#include "../aws/AwsIotClient.h"

class TelemetryPublisher
{
public:
  explicit TelemetryPublisher(AwsIotClient &mqtt);
  void begin(const char *thingName);
  void loop(uint32_t now);

private:
  AwsIotClient &mqtt_;
  String topic_;
  const char *thingName_ = nullptr;
  uint32_t lastPublished_ = 0;
  uint32_t sequence_ = 0;
};
