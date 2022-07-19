//===========================================================================
//       MQTT client with PKI certificates
//===========================================================================
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
"MIIBlDCCATugAwIBAgIUIx23uKnAbT27Fzyi7GOyfHNdur4wCgYIKoZIzj0EAwQw\n" \
"GDEWMBQGA1UEAwwNS0YtQ1MtUm9vdC1DQTAeFw0yMjA0MDYwNjA1MjBaFw0zMjA0\n" \
"MDMwNjA1MTlaMBgxFjAUBgNVBAMMDUtGLUNTLVJvb3QtQ0EwWTATBgcqhkjOPQIB\n" \
"BggqhkjOPQMBBwNCAAQDxdjwz1Z7mDXsQH+pmG0ukgAplpknPkMYrVDmDJjPO6oI\n" \
"V5T/IJbEuaNiCWLhmIyWJiOXop9tIqNCkN3m/Eioo2MwYTAPBgNVHRMBAf8EBTAD\n" \
"AQH/MB8GA1UdIwQYMBaAFNfyHV37KQV2IxmJOOrmb4IxnmasMB0GA1UdDgQWBBTX\n" \
"8h1d+ykFdiMZiTjq5m+CMZ5mrDAOBgNVHQ8BAf8EBAMCAYYwCgYIKoZIzj0EAwQD\n" \
"RwAwRAIgAhFuOvifYASZYAPtYcCsV5o2jmlqMxqjyyrLQg+YNsACIBALRFE0Sse8\n" \
"LL+hxgrTDDTdLZdbRL7wEdK4i8uSAn1v\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"MIIBoDCCAUWgAwIBAgIUZ6H3BlrDiKJ6Uir4OttFX8KU++owCgYIKoZIzj0EAwQw\n" \
"GDEWMBQGA1UEAwwNS0YtQ1MtUm9vdC1DQTAeFw0yMjA0MDYwNjM3MzhaFw0zMjA0\n" \
"MDMwNjA1MTlaMCIxIDAeBgNVBAMMF0tGLUNTLUludC1DQS1QbGF5R3JvdW5kMFkw\n" \
"EwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEnFNONsdycSXj4l98D8SzNdgFVWV6EQa5\n" \
"sNTo+OoXlxjZ8LWvlqGdYjKerHh+4XWWmewK2kMUj9wsKP5a3AMdbKNjMGEwDwYD\n" \
"VR0TAQH/BAUwAwEB/zAfBgNVHSMEGDAWgBTX8h1d+ykFdiMZiTjq5m+CMZ5mrDAd\n" \
"BgNVHQ4EFgQUMwcqUiau+434Blrq7ymvNUEqOVcwDgYDVR0PAQH/BAQDAgGGMAoG\n" \
"CCqGSM49BAMEA0kAMEYCIQCQsMpoUL1EToDbOPydaBVZLzH1VtKOMLYiorXdZCS+\n" \
"TgIhAPg05WN/IlV9cAv2jE/jsiG66Ei1fQtiRyWYT5f8h83/\n" \
"-----END CERTIFICATE-----\n" \
"-----BEGIN CERTIFICATE-----\n" \
"MIIBpDCCAUugAwIBAgIUXMljPxOgbaMjNZNPZVkREZGrFXwwCgYIKoZIzj0EAwQw\n" \
"IjEgMB4GA1UEAwwXS0YtQ1MtSW50LUNBLVBsYXlHcm91bmQwHhcNMjIwNzEyMDYx\n" \
"NDIwWhcNMjMwNzEyMDYxNDE5WjAeMRwwGgYDVQQDDBNLRi1DUy1IQUNLVEhFRkFC\n" \
"LUNBMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEmSEp9U8tv+uhn+4SKyXEYoml\n" \
"3GLWmOp10oxNMMRjJFIZLEVDUBEYw5J8e1NhmPofizz1hyhHX5fO6klIljJDM6Nj\n" \
"MGEwDwYDVR0TAQH/BAUwAwEB/zAfBgNVHSMEGDAWgBQzBypSJq77jfgGWurvKa81\n" \
"QSo5VzAdBgNVHQ4EFgQUYEM8tBJPLlsrjrFdb9ijuHxcELQwDgYDVR0PAQH/BAQD\n" \
"AgGGMAoGCCqGSM49BAMEA0cAMEQCIBAj/MdRhaZYUWrTc8FPJ3iMNhrJsKVa0ZHo\n" \
"hBbAeGJlAiB3yIjeH5WbJ0a4WNkj27XB0/oySB9s4W/IUiMw/Mxy5w==\n" \
"-----END CERTIFICATE-----";


  const char* local_client_crt = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIC0zCCAnmgAwIBAgIUf0goDJhTaEMrnccLFUNZLFR/I98wCgYIKoZIzj0EAwQw\n" \
"HjEcMBoGA1UEAwwTS0YtQ1MtSEFDS1RIRUZBQi1DQTAeFw0yMjA3MTgxNDMyMzha\n" \
"Fw0yMzA3MDMxNDMyMzdaMGkxEDAOBgNVBA0MB0dyb3VwXzExFzAVBgNVBAMMDjE5\n" \
"Mi4xNjguODguMTAyMRMwEQYDVQQLDApIYWNrVGhlRmFiMRowGAYDVQQKDBFDYW1w\n" \
"dXNTY2h3YXJ6d2FsZDELMAkGA1UEBhMCREUwggEiMA0GCSqGSIb3DQEBAQUAA4IB\n" \
"DwAwggEKAoIBAQCnn3H1mRY/wIbtcxlmr0n5A1MUAAjuaSSrx12TCIqb2R76ugCA\n" \
"LPwZ1p2URaJVjWvDS6i7qo4MRw0KkTcOkCMCP8cLz8s99QXWbGDx3UG1KxTZnkDW\n" \
"b7OxemBLV9vwPXfiIvf7PRQbRYLVt3CyyKxMQbPDzBKN+WY0WXMhrNZctWLaPr32\n" \
"gBaKzpsfizLlMNl087iV+PV9hahHqk/bBbZyj2icmwq37NBn/B/5B/ghyfLlJBd7\n" \
"luA5jzB3qYRubjN8yGGIWeiixnputAs44ZxJMSBfL04HJyFuSjtanr2PVSoFw08K\n" \
"NVuf7WIDlrGLRwpowVGnPvqEQipVG+1/o7/TAgMBAAGjfzB9MAwGA1UdEwEB/wQC\n" \
"MAAwHwYDVR0jBBgwFoAUYEM8tBJPLlsrjrFdb9ijuHxcELQwHQYDVR0lBBYwFAYI\n" \
"KwYBBQUHAwIGCCsGAQUFBwMBMB0GA1UdDgQWBBQQorvJpxAT7pWkjuNVAlavjQJI\n" \
"yTAOBgNVHQ8BAf8EBAMCBaAwCgYIKoZIzj0EAwQDSAAwRQIgEyvix3WLz4igPZSU\n" \
"I82D11EXbQ3s+24/a2HsM93fQDoCIQD/O72EC5YmTsf4AcSGpciSrBiiuzASDaMj\n" \
"BYK5VvUtNg==\n" \
"-----END CERTIFICATE-----";

  const char* local_client_key = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQCnn3H1mRY/wIbt\n" \
"cxlmr0n5A1MUAAjuaSSrx12TCIqb2R76ugCALPwZ1p2URaJVjWvDS6i7qo4MRw0K\n" \
"kTcOkCMCP8cLz8s99QXWbGDx3UG1KxTZnkDWb7OxemBLV9vwPXfiIvf7PRQbRYLV\n" \
"t3CyyKxMQbPDzBKN+WY0WXMhrNZctWLaPr32gBaKzpsfizLlMNl087iV+PV9hahH\n" \
"qk/bBbZyj2icmwq37NBn/B/5B/ghyfLlJBd7luA5jzB3qYRubjN8yGGIWeiixnpu\n" \
"tAs44ZxJMSBfL04HJyFuSjtanr2PVSoFw08KNVuf7WIDlrGLRwpowVGnPvqEQipV\n" \
"G+1/o7/TAgMBAAECggEAB81/LDveXysmCY4zjv+dSM2nHUW5JLWMdib9vwPrq9oj\n" \
"ook5AvGh7jP606jM9M2N+1VqYXEzICgu8VOvXx3fiB6SGrjA0PDQckBLE2DFl8Da\n" \
"VunFqK9V2CIGf7HANILJmCNUYGoVJWw+Of0wux0AEQneUkCnVnUj4ej0cb6OOSd9\n" \
"dW90HHWJWi9UjAI8G2LFDkH8TEhfjrfDZn6UP54LIinNXB/pXdtK+XiBlxc3yfuR\n" \
"es91UuXA6qIPNUdcvgO4qEk/SIzLHXHzeWHr5FmUCGC5eSbxnuMbe79qpNs8UDcp\n" \
"XVhei6oyltL9y5OCuJBJOXMpzE/kgKt37RpRLJwYCQKBgQDdeEVSuylK2ENtHhce\n" \
"3y1+Y1xjSxCcwXyDlcUNJPwuF5HI0jLUGhNhz6tsZgkw4XxgkCZvByN9dL0IjtQo\n" \
"8XUwseS8py6JPzGgPcF3fNRJOH1dpl3ZppOawwLfTg/z93sJkLUfrGjBhnt01sLQ\n" \
"/3388FG3Re6n1XhfWckOEcsbywKBgQDBwe4lKVxEGNzm6vleJN7kIANAdYn2v12L\n" \
"Lx2QZmkqnyKe1O60dV0Zxs5M8jrYtJrI/Nwl2DpOOBjicPrZUidigWvOr7QQoaGU\n" \
"o7XXZ/rdVgC/nnnMkcpIfd9MDHeYilxQf4RL4L7GzryrBwpHkiMcg3Ax387qie9l\n" \
"R2oTxer7GQKBgCF3+4H1JthYtlfXJEdWEtn2yg93WJtX4ilAiPbjj/inNzDSEzxa\n" \
"dHNjntRS1zC74r/1CNneik57RdX1P7qwQrWJLLgVy8KR+j3tFz+nba1jDA8LBUPP\n" \
"GJ9auSRl96M20zEu3TBcLbiRSjbt/aL3+WIwLykzOE3ExNwkBW5K4hbhAoGADwuh\n" \
"E2+nw33uwVujq4TPbgVyWzIFWYy4ldM9x3W8Wtvqk0zTkF3Asfy3DlCM80Dq+Apy\n" \
"idnFhUdj30mIoRF5D3QfS/hm0hfK9UDVIPnpCADio/6JVLJJc+GaUkxxiP3Nd06/\n" \
"lijIh6WMDvfH+5DEezVVo/nOFnZJiaLHAOwQeBkCgYBJZ/ix6LWJ+Herv5CHmr+N\n" \
"7NJ+z9hFeUh6BxEPxOC3Am7DekZReKZ1yFTCH4V8qVv2eVU8S6rg9PAxDwAQn7ZK\n" \
"qaByx1MV7UAKb116BVdpiMwXblWO9FDTEdv27BYAbcW/VzmGsMYLdh7NixCL05TR\n" \
"S9EaBBl1RpLi7joNOG5Fwg==\n" \
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