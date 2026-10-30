#include "AwsIotClient.h"

#include <Arduino.h>
#include <WiFi.h>

#include "AppConfig.h"

AwsIotClient::AwsIotClient() : mqtt_(1024) {}

void AwsIotClient::begin(const char *endpoint, const char *thingName,
                         const char *rootCaPem,
                         const char *deviceCertificatePem,
                         const char *privateKeyPem)
{
  thingName_ = thingName;
  tls_.setCACert(rootCaPem);
  tls_.setCertificate(deviceCertificatePem);
  tls_.setPrivateKey(privateKeyPem);
  tls_.setTimeout(5000);
  mqtt_.begin(endpoint, AppConfig::kMqttPort, tls_);
}

bool AwsIotClient::connect()
{
  if (WiFi.status() != WL_CONNECTED || thingName_ == nullptr)
  {
    return false;
  }

  if (mqtt_.connected())
  {
    return true;
  }

  Serial.println("AWS IoT: connecting over MQTT/TLS...");
  if (!mqtt_.connect(thingName_))
  {
    Serial.println("AWS IoT: connection failed; retry scheduled.");
    return false;
  }

  Serial.println("AWS IoT: connected.");
  return true;
}

bool AwsIotClient::connected() { return mqtt_.connected(); }

void AwsIotClient::disconnect()
{
  if (mqtt_.connected())
  {
    mqtt_.disconnect();
  }
}

void AwsIotClient::loop()
{
  if (mqtt_.connected())
  {
    mqtt_.loop();
  }
}

bool AwsIotClient::publish(const char *topic, const char *payload)
{
  return mqtt_.connected() && mqtt_.publish(topic, payload);
}
