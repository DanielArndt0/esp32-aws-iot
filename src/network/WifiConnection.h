#pragma once

#include <stdint.h>

class WifiConnection
{
public:
  void begin(const char *ssid, const char *password);
  bool maintain(uint32_t now);

private:
  bool wasConnected_ = false;
  uint32_t lastAttempt_ = 0;
};
