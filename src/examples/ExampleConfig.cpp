#include "ExampleConfig.h"

#include <cstring>

namespace
{
       bool missing(const char *value)
       {
              return value == nullptr || value[0] == '\0' ||
                     std::strstr(value, "CHANGE_ME") != nullptr ||
                     std::strstr(value, "PASTE_") != nullptr;
       }

       bool validPem(const char *pem, const char *ending)
       {
              return !missing(pem) && std::strstr(pem, "-----BEGIN ") != nullptr &&
                     std::strstr(pem, ending) != nullptr;
       }
}

bool ExampleConfig::valid(const char *ssid, const char *password,
                          const char *endpoint, const char *thingName,
                          const char *rootCa, const char *certificate,
                          const char *privateKey)
{
       return !missing(ssid) &&
              !missing(password) &&
              !missing(endpoint) &&
              !missing(thingName) &&
              validPem(rootCa, "CERTIFICATE-----") &&
              validPem(certificate, "CERTIFICATE-----") &&
              validPem(privateKey, "PRIVATE KEY-----");
}
