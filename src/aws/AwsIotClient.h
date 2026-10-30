#pragma once

#include <MQTTClient.h>
#include <WiFiClientSecure.h>

class AwsIotClient
{
public:
  AwsIotClient();

  void begin(const char *endpoint, const char *thingName,
             const char *rootCaPem, const char *deviceCertificatePem,
             const char *privateKeyPem);
  bool connect();
  bool connected();
  void disconnect();
  void loop();
  bool publish(const char *topic, const char *payload);

private:
  WiFiClientSecure tls_;
  MQTTClient mqtt_;
  const char *thingName_ = nullptr;
};
