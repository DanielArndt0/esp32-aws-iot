# ESP32 AWS IoT Core Example

A basic ESP32 example for publishing device telemetry to AWS IoT Core over MQTT with mutual TLS authentication. Built with the Arduino framework and PlatformIO.

## Features

- MQTT over TLS on port 8883 with X.509 device authentication.
- Server certificate verification using Amazon Root CA 1.
- Wi-Fi and MQTT reconnection with bounded retry intervals.
- NTP synchronization before establishing a TLS connection.
- JSON telemetry containing uptime, available heap, and a message counter.
- Local configuration through an ignored `secrets.h` file.
- Small, responsibility-focused C++ modules.

## Requirements

- ESP32 development board compatible with `esp32dev`.
- 2.4 GHz Wi-Fi with internet access.
- Visual Studio Code with PlatformIO, or the PlatformIO CLI.
- An AWS IoT Core Thing, active X.509 certificate, and attached IoT policy.

## Getting started

1. Configure your Thing, certificate, endpoint, and permissions using the [AWS IoT setup guide](docs/aws-iot-setup.md).
2. Copy `include/secrets.h.example` to `include/secrets.h`.
3. Set the Wi-Fi credentials, AWS IoT endpoint, Thing name, and PEM contents in `include/secrets.h`.
4. Connect your ESP32 and build/upload the firmware:

```bash
pio run
pio run --target upload
pio device monitor -b 115200
```

For PowerShell, copy the configuration file with:

```powershell
Copy-Item include/secrets.h.example include/secrets.h
```

The application waits for Wi-Fi, valid system time, and MQTT connectivity before publishing messages.

## Configuration

| Setting | Description |
| --- | --- |
| `kWifiSsid`, `kWifiPassword` | Wi-Fi network credentials |
| `kAwsEndpoint` | AWS IoT Core device data (ATS) endpoint, without a protocol prefix |
| `kThingName` | AWS IoT Thing name, also used as the MQTT client ID |
| `kRootCaPem` | Complete Amazon Root CA 1 certificate in PEM format |
| `kDeviceCertificatePem` | Complete device certificate in PEM format |
| `kPrivateKeyPem` | Complete device private key in PEM format |

Paste the full PEM blocks, including their BEGIN and END lines, into the existing raw string literals.

## Example

The default example is implemented in `src/examples/BasicTelemetryExample.cpp`. It publishes every 15 seconds to:

```text
devices/THING_NAME/telemetry
```

Example payload:

```json
{
  "thing": "esp32-demo-01",
  "uptime_seconds": 30,
  "free_heap_bytes": 203000,
  "sequence": 1
}
```

Messages use MQTT QoS 0. The values are ESP32 runtime metrics, not simulated sensor readings.

## Security

Real credentials belong only in `include/secrets.h`, which is ignored by Git. Never commit device certificates or private keys. Each device should have its own certificate and a narrowly scoped AWS IoT policy.

This is a basic demonstration. The private key is embedded in the firmware image; production hardware should use an appropriate protected key-storage and device-provisioning strategy.

## License

MIT. Copyright (c) 2026 Daniel Gier Arndt.
