#pragma once

#include <stdint.h>
#include <time.h>

namespace AppConfig
{
  constexpr uint32_t kSerialBaud = 115200;
  constexpr uint16_t kMqttPort = 8883;
  constexpr uint32_t kWifiRetryMs = 10000;
  constexpr uint32_t kMqttRetryMs = 10000;
  constexpr uint32_t kTelemetryEveryMs = 15000;
  constexpr uint32_t kClockLogEveryMs = 15000;
  constexpr time_t kMinimumUnixTime = 1700000000;
}
