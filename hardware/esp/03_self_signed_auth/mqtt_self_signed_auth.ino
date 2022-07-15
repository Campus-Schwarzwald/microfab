#include <WiFi.h>
//#include <WiFiClientSecure.h>  //included WiFiClientSecure does not work!
#include "src/dependencies/WiFiClientSecure/WiFiClientSecure.h" //using older WiFiClientSecure
#include <time.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>


const char ssid[] = "Microfab_002";
const char pass[] = "8UcXxXCF";

#define HOSTNAME "192.168.88.100"

IPAddress local_IP(192, 168, 88, 102);
IPAddress gateway(192, 168, 88, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);

const char *MQTT_HOST = "192.168.88.100";
const int MQTT_PORT = 8883;
const char *MQTT_USER = "pubclient";
const char *MQTT_PASS = "microfoo123";

const char* local_root_ca = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIDxDCCAqygAwIBAgIUUYNYRGz0zH1j7LaCQguxI76RuucwDQYJKoZIhvcNAQEL\n" \
"BQAwazELMAkGA1UEBhMCREUxCzAJBgNVBAgMAkJXMQwwCgYDVQQHDANGRFMxGjAY\n" \
"BgNVBAoMEUNhbXB1c1NjaHdhcnp3YWxkMRMwEQYDVQQLDApIYWNrVGhlRmFiMRAw\n" \
"DgYDVQQDDAdUZXN0IENBMB4XDTIyMDcxNTE0NDUzN1oXDTMyMDUyMzE0NDUzN1ow\n" \
"azELMAkGA1UEBhMCREUxCzAJBgNVBAgMAkJXMQwwCgYDVQQHDANGRFMxGjAYBgNV\n" \
"BAoMEUNhbXB1c1NjaHdhcnp3YWxkMRMwEQYDVQQLDApIYWNrVGhlRmFiMRAwDgYD\n" \
"VQQDDAdUZXN0IENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEArsVy\n" \
"Ulz0XHOtdnTm00HIVVD4glVXGQIzIMT4nE4TO8jNzodtE/2PBvCKepRwlETIds52\n" \
"CvYia0lkdvrLzDJgjBUIJrXMXyuyWcTPocsfUFc3SehMpo0Lsdv89yJndb3NiM50\n" \
"vLM77rExUumJU5EM8BaMS3QyBaQesXNzBkDLnApIR8pv9HDp/KAozEpLFCUnyFIe\n" \
"snQ1xSgSnOTr2+hZVyEKUNSz1AXfG4wbfZRIo3l53UmnQdFYH+mBSEx5Ytk0ECex\n" \
"+3lKeatP1BI8ii+v0nxoB780TxIRP7WrM2CK02tdVMsQdmDeO9vCYfCoZphcAjoW\n" \
"WzLp/WoF7lmmtmE6IwIDAQABo2AwXjAdBgNVHQ4EFgQUIEn3NTyaJN+RwXJyg2rW\n" \
"szOPEJIwHwYDVR0jBBgwFoAUIEn3NTyaJN+RwXJyg2rWszOPEJIwDwYDVR0TAQH/\n" \
"BAUwAwEB/zALBgNVHQ8EBAMCAQYwDQYJKoZIhvcNAQELBQADggEBACusnWR/uj1j\n" \
"ZN95BFTUsZcqdehWHI/ML61V94xR6Nz0MY7xI5mHkgqBeu34Rd1KpaASrYDXGRk/\n" \
"XHi7A6lLNN0zlS0uZ1BYNEUQ6JBXbey1WVVnieX0nYG8Tziofl+tz/XVZD+t7+oi\n" \
"NPXkZuLbxE4ZxS0biHsX230c0N+n2YuFhtvgBMEbmVVwMkwzE8BSoUrAFGTdicRY\n" \
"SBhZ930cGKL502EN0LuKxUl/blWMB06pvB6I9ZkNaTexXzJ/iymqBOY/uxmPiAFC\n" \
"q4ElAfeO0f+Kmzj13Up20U2+KZj3Tw86K/uy8xV286/zSW5UBszj3nVYjc2DqaRd\n" \
"vPeP5CB6RW8=\n" \
"-----END CERTIFICATE-----";


  const char* local_client_crt = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIESTCCAzGgAwIBAgIBAjANBgkqhkiG9w0BAQsFADBrMQswCQYDVQQGEwJERTEL\n" \
"MAkGA1UECAwCQlcxDDAKBgNVBAcMA0ZEUzEaMBgGA1UECgwRQ2FtcHVzU2Nod2Fy\n" \
"endhbGQxEzARBgNVBAsMCkhhY2tUaGVGYWIxEDAOBgNVBAMMB1Rlc3QgQ0EwHhcN\n" \
"MjIwNzE1MTQ0NTQyWhcNNDkxMTMwMTQ0NTQyWjByMQswCQYDVQQGEwJERTELMAkG\n" \
"A1UECAwCQlcxDDAKBgNVBAcMA0ZEUzEaMBgGA1UECgwRQ2FtcHVzU2Nod2Fyendh\n" \
"bGQxEzARBgNVBAsMCk1RVFRDTElFTlQxFzAVBgNVBAMMDjE5Mi4xNjguODguMTAy\n" \
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAmzRTGmpcuGVyepI0n1mA\n" \
"7YHhsPTusikPBGUmhO7b6TMYUm4ZrsfEzsvwWtSUyWZyoOfb1FpHhmYNCtSWKJzl\n" \
"deSiBaWGVFS0T4K7fIrvw52O/bgfZhlmDFPOCCVcpiJl7mgDqYSQZw9wjSbuUDTq\n" \
"akFWVAStsU6jxVqdVqoDnWCzE4WmsHoJyYTGmywDAoDQE5Vaermwc+ncRLr9Rjq4\n" \
"83Ntz9DM4NAxQ2m7JR1UPTT38cbpEaudhO/MxVZLLBIKBaRPXT3X/otJF3fmMp4w\n" \
"L09unGoPHhxXoT1h+AGTcVfWJYIn1xx0kWdNGxdkWm3VYuXwYTE300yVX1EJ2BxO\n" \
"uwIDAQABo4HwMIHtMB0GA1UdDgQWBBReUsxGXRKegmyS6KENmuWEkA20AzAfBgNV\n" \
"HSMEGDAWgBQgSfc1PJok35HBcnKDatazM48QkjAJBgNVHRMEAjAAMAsGA1UdDwQE\n" \
"AwIFoDARBglghkgBhvhCAQEEBAMCBsAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsG\n" \
"AQUFBwMCMCsGCWCGSAGG+EIBDQQeFhxPcGVuU1NMIEdlbmVyYXRlZCBDZXJ0aWZp\n" \
"Y2F0MDQGA1UdEQQtMCuGI2h0dHA6Ly9leGFtcGxlcy5mcmVlb3BjdWEuZ2l0aHVi\n" \
"LmlvhwTAqFhmMA0GCSqGSIb3DQEBCwUAA4IBAQAojKqN/520LLw3cc6YFz8p9BrU\n" \
"jzz8Z6GOGxdnI8QXQE+WQqCTElnIfFI8+UO2uaWs3hH6Zgcq+Xnx6KaDxFSHTkmp\n" \
"nm2pRmiK1vpA7Sgm2oA/c5XOjPhZxt+E79OXxHYGTGr8qdwT+7wVXD5liH56hEg0\n" \
"7uXHdyaaMgQAp78yVUWGdQwqH40RIUOivinASPfkH6LEGdT19RPju+o8tmgAywmD\n" \
"B8p8YV/8HnSoyLJ2+yurS1YXYD3MCa8LowBT42mkSwSuBGmtzL0VH1TIP3mUuxR2\n" \
"GNVAeuQVRAmjOgghVyjz7XL8mDp54fgaBNzr8s/7tEUJOH0HkCwQZ+p15Tpw\n" \
"-----END CERTIFICATE-----";

  const char* local_client_key = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQCbNFMaaly4ZXJ6\n" \
"kjSfWYDtgeGw9O6yKQ8EZSaE7tvpMxhSbhmux8TOy/Ba1JTJZnKg59vUWkeGZg0K\n" \
"1JYonOV15KIFpYZUVLRPgrt8iu/DnY79uB9mGWYMU84IJVymImXuaAOphJBnD3CN\n" \
"Ju5QNOpqQVZUBK2xTqPFWp1WqgOdYLMThaawegnJhMabLAMCgNATlVp6ubBz6dxE\n" \
"uv1GOrjzc23P0Mzg0DFDabslHVQ9NPfxxukRq52E78zFVkssEgoFpE9dPdf+i0kX\n" \
"d+YynjAvT26cag8eHFehPWH4AZNxV9YlgifXHHSRZ00bF2RabdVi5fBhMTfTTJVf\n" \
"UQnYHE67AgMBAAECggEAMxEK3PCtNGDww2Ky3NGKydsH4v0cQtOepNARVgDvq7HL\n" \
"qWefnLIPUAmrOn+iuwex5L6VRrAzw4V1eiBLje5d06YYJ2p4oevC7jxyKv7d+Aux\n" \
"/kpkTmhB+wFiE60TaHh9+G0R82dySe67Fj5+gDaBANghqwBlFCdvXGK9zUOgb9nb\n" \
"k1C6+LL4lKhhUC1kbN8UwddBnc1oS08vcSTIHxEYCqBRf9uT3BmaUblAYfplgjKe\n" \
"8E8jgw6EleFwiWAnXxl6a5+ZeOS8wQOQWOVKJL8W03eg8WfhYpaDpUE75kK0gTnf\n" \
"jcnIp04cfRaBmEGFnHAgN95nTz6sh6lA9s/1m8CinQKBgQDQnVf+umI2bHmfbU5L\n" \
"/gprAQAKHv+UJ1YyrYCctMhbs117vkK0DPisTo8pLg7d/ihDSlRuEybl4HvShV3u\n" \
"mFAsyFZ6cbakdCV9syz2iYaNDHuLxK2DV81Yx2GcjNRXJ6QAV034iotfY3yf0B1S\n" \
"E20y6Y8qlfPO9jwdP4Jb4L8ERwKBgQC+dT+rFyOkC+G25+J36L3SFl1C2/eVCK57\n" \
"6+hhddkRIADoNPTJVMDkd9Lq3wAsmQZ7sYgNyDvXBWSRm2rQ2QIIG5TlVsbJGZlL\n" \
"59yQAhHOqesxFYs1dE96v8DqQdx3vMEwENM0K+RfYeRaW8e334q4kVMgNu4htIrI\n" \
"uM01WFlf7QKBgDow+b/Da+v0fESdb+7AsVVB1Sn9lZtVEbVXMn4D67Owp84Bt4eT\n" \
"6rXdwA9jmqAP6llC3aRK0VnkMYX6z8XjVNH5HC5Rk/nxsBeNKGio2hB0P+4v8Zqm\n" \
"nlA6ZZ8OpZieJSA3OjevJct46+WEXvlrE9XU0qu1YmoDw6cA9TL90Sq3AoGAQcMw\n" \
"Yb4So2Eq5cH5k5e8e18Xgq97NG2T+dwCGCzd76jSR1uJrj7if8DUBh9L6xpHf8xU\n" \
"AxZBe0Q0boZwgn570hrb9bhZ2TRKl0gPA3aDeY14VTFwKNvpWaP+U6S8IGf/eL5G\n" \
"fvws1bZyZ7TLKDU6q/IZ9sSgwuiZByewS5we+B0CgYA/kopq0a5EKL/75DIdiAWr\n" \
"BoyaYQMgNbGTNxjrTfGA6w3fut1IYSPFTpaenXzTR/eOjZSF2/XTvvgaVw23dFKL\n" \
"V1rFI+O7oSwubqKd1QO3cehBHetVpcidOE85zXG4Dl1n0ucGzZxZLozjMdp4Em51\n" \
"1TIpVPNHmw0SO/YqKF3PbQ==\n" \
"-----END PRIVATE KEY-----";

const char* topic = "test/"; // CHANGE SensorID here!
char output[128];
time_t now;

WiFiClientSecure net;
PubSubClient client(net);

/*
 * JSON Data Format
*/
const size_t capacity = JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(3);
DynamicJsonDocument doc(capacity);
JsonObject data = doc.createNestedObject("data");

void mqtt_connect()
{
    while (!client.connected()) {
    Serial.print("Time:");
    Serial.print(ctime(&now));
    Serial.print("MQTT connecting");
    if (client.connect(HOSTNAME, MQTT_USER, MQTT_PASS)) {
      Serial.println("connected");
      client.subscribe(topic);
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }

}

void receivedCallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received [");
  Serial.print(topic);
  Serial.print("]: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
}

void wifi_connect()
{
  if (WiFi.status() != WL_CONNECTED) {
    // WIFI
      Serial.println();
      Serial.print("===> WIFI ---> Connecting to ");
      Serial.println(ssid);

      delay(10);
      WiFi.setHostname(HOSTNAME);
      WiFi.mode(WIFI_STA);
      // Configures static IP address
      if (!WiFi.config(local_IP, gateway, subnet, primaryDNS)) {
        Serial.println("STA Failed to configure");
      }
      WiFi.begin(ssid, pass);

      int Attempt = 0;
      while (WiFi.status() != WL_CONNECTED) {
        Serial.print(". ");
        Serial.print(Attempt);

        delay(100);
        Attempt++;
      if (Attempt == 150)
      {
        Serial.println();
        Serial.println("-----> Could not connect to WIFI");

        ESP.restart();
        delay(200);
      }

    }
      Serial.println();
      Serial.print("===> WiFi connected");
      Serial.print(" ------> IP address: ");
      Serial.println(WiFi.localIP());
    }
  }

void setup()
{
  Serial.begin(115200);

  wifi_connect();

  Serial.print("Setting time using SNTP");
  configTime(-5 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  now = time(nullptr);
  while (now < 1510592825) {
    delay(500);
    Serial.print(".");
    now = time(nullptr);
  }
  Serial.println("");
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  Serial.print("Current time: ");
  Serial.print(asctime(&timeinfo));

  net.setCACert(local_root_ca);
  net.setCertificate(local_client_crt);
  net.setPrivateKey(local_client_key);
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(receivedCallback);
  mqtt_connect();
}

void loop()
{
  now = time(nullptr);
  if (WiFi.status() != WL_CONNECTED)
  {
    wifi_connect();
  }
  else
  {
    if (!client.connected())
    {
      mqtt_connect();
    }
    else
    {
        data["temp"].set(random(20, 25));
        data["hall"].set(hallRead());

        // Output Format
        serializeJson(doc, output);
        Serial.println(output);
        client.publish(topic, output);
        delay(5000); // Publish data every 5 seconds to the Broker
        client.loop();
    }
  }
}