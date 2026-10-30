# AWS IoT Core setup

This guide covers the AWS configuration required by the basic ESP32 MQTT example.

## 1. Create a Thing

In **AWS IoT Core → All devices → Things**, create a Thing such as `esp32-demo-01`.

## 2. Create a certificate

Create an X.509 device certificate and download its certificate and private key. Attach the certificate to the Thing and keep it active. Also download [Amazon Root CA 1](https://www.amazontrust.com/repository/AmazonRootCA1.pem) for TLS server validation.

Store the complete PEM contents in your local `include/secrets.h` file. The private key must never be committed.

## 3. Configure the IoT policy

Attach an AWS IoT policy to the device certificate with only these permissions:

| Action | Allowed resource |
| --- | --- |
| `iot:Connect` | `arn:aws:iot:REGION:ACCOUNT_ID:client/THING_NAME` |
| `iot:Publish` | `arn:aws:iot:REGION:ACCOUNT_ID:topic/devices/THING_NAME/telemetry` |

Replace `REGION`, `ACCOUNT_ID`, and `THING_NAME` with your AWS account and Thing values. No subscribe or receive permissions are needed for this example.

The MQTT client ID used by the firmware must match the Thing name. You can write a policy for one specific Thing or use AWS IoT [Thing policy variables](https://docs.aws.amazon.com/iot/latest/developerguide/thing-policy-variables.html) to reuse the same policy structure across devices.

## 4. Configure the endpoint

In AWS IoT Core, copy your **Device data endpoint (ATS)** into `kAwsEndpoint`. Use the hostname only, without `https://` or a port number.

## 5. Verify messages

Upload the firmware, open the serial monitor, and wait for MQTT connectivity. In the AWS IoT MQTT test client, subscribe to:

```text
devices/esp32-demo-01/telemetry
```

Use your actual Thing name. Telemetry is published every 15 seconds.

## References

- [Create AWS IoT resources](https://docs.aws.amazon.com/iot/latest/developerguide/create-iot-resources.html)
- [Attach a Thing or policy to a certificate](https://docs.aws.amazon.com/iot/latest/developerguide/attach-to-cert.html)
- [Connect and publish policy examples](https://docs.aws.amazon.com/iot/latest/developerguide/connect-and-pub.html)

