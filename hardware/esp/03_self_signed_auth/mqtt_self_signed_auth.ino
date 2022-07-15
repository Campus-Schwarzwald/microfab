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
"MIIDxDCCAqygAwIBAgIUbey2fOrlPDViYTO+Ecmgyx8L7jIwDQYJKoZIhvcNAQEL\n" \
"BQAwazELMAkGA1UEBhMCREUxCzAJBgNVBAgMAkJXMQwwCgYDVQQHDANGRFMxGjAY\n" \
"BgNVBAoMEUNhbXB1c1NjaHdhcnp3YWxkMRMwEQYDVQQLDApIYWNrVGhlRmFiMRAw\n" \
"DgYDVQQDDAdUZXN0IENBMB4XDTIyMDcxNTE0MjA1NloXDTMyMDUyMzE0MjA1Nlow\n" \
"azELMAkGA1UEBhMCREUxCzAJBgNVBAgMAkJXMQwwCgYDVQQHDANGRFMxGjAYBgNV\n" \
"BAoMEUNhbXB1c1NjaHdhcnp3YWxkMRMwEQYDVQQLDApIYWNrVGhlRmFiMRAwDgYD\n" \
"VQQDDAdUZXN0IENBMIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAvdHi\n" \
"GXMgp1zKOo+4L/D+bQwf088XnuxuqOvrIwIIw2NeU6RyqzXNM+FVazN8lx3u8TGq\n" \
"WPmeQTyuJtGC45Oujo8Sgk0VQne9v7RcZ9bg7GCUiG4aZjygoMqerH+uiqsKDqOd\n" \
"UTupKkGq7vb1sewe3L8em9eNJhbUgLG+Wk0YPddIkXt3luxjvNF+OgKDoyTwtnFg\n" \
"XIWUuZGkoVfaCQfkWhJcjdWkt+q6h26TLnD9K3YUWGDpE5uwr4/BhjRjHzIXobU9\n" \
"fppVw/dY5vVg/n6v7bpLUs93CcwG7lXLHYnFP4O3CWLU3dHVoKJY16CDW6RtLQwP\n" \
"bNshnkvrtOqiSQIYWQIDAQABo2AwXjAdBgNVHQ4EFgQUt4UkqtpmSMco/jTbzyeH\n" \
"kUMQPMYwHwYDVR0jBBgwFoAUt4UkqtpmSMco/jTbzyeHkUMQPMYwDwYDVR0TAQH/\n" \
"BAUwAwEB/zALBgNVHQ8EBAMCAQYwDQYJKoZIhvcNAQELBQADggEBAI1L8vT8O1V4\n" \
"OUv4aqjD1BsOGrbMjsyyitFyjeG+YOq0we25wnciZ3LEK0Ih+xNjih84k1YbJ/kU\n" \
"L8BoA5rrh/XXV8E+NsQmrqw/HDF5roMCmcQKvlljMZzEw8jmJI1ryru8WVYyPCqJ\n" \
"lwmQbDC+dnZz96YNAid9I1meF+IGSoSy3+6JAVvlMLL6x5stCR/Zv2GvrFMLo2sh\n" \
"NlsFR0lMkX/Mgh6S7L/lf99f1B/2/ASuyB7gyAHPvoTmYz2CEruGv6z7fILSPWq7\n" \
"cg1wFR3ozKEdE0W3DWQmUwTGDsgltrlqevTuNmFosCMDC8E/Q4be0HGdj/GTbhI6\n" \
"wLygt2CwRbw=\n" \
"-----END CERTIFICATE-----";


  const char* local_client_crt = \
"-----BEGIN CERTIFICATE-----\n" \
"MIIESTCCAzGgAwIBAgIBAjANBgkqhkiG9w0BAQsFADBrMQswCQYDVQQGEwJERTEL\n" \
"MAkGA1UECAwCQlcxDDAKBgNVBAcMA0ZEUzEaMBgGA1UECgwRQ2FtcHVzU2Nod2Fy\n" \
"endhbGQxEzARBgNVBAsMCkhhY2tUaGVGYWIxEDAOBgNVBAMMB1Rlc3QgQ0EwHhcN\n" \
"MjIwNzE1MTQyMTAwWhcNNDkxMTMwMTQyMTAwWjByMQswCQYDVQQGEwJERTELMAkG\n" \
"A1UECAwCQlcxDDAKBgNVBAcMA0ZEUzEaMBgGA1UECgwRQ2FtcHVzU2Nod2Fyendh\n" \
"bGQxEzARBgNVBAsMCk1RVFRDTElFTlQxFzAVBgNVBAMMDjE5Mi4xNjguODguMTAy\n" \
"MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA219e5HuefikE+jGscD2e\n" \
"79EQOPQzzbNn9FXt4OuASuvOQ+d5S9cbfie9FQQtrqcAGbzAW5xfXCveBQAN3CtL\n" \
"znldjIvTUr759pxd1Bd0Yer1bLvxA6wh4rCcA/iIcBBOkeToixdEHPpl0SQdhjGj\n" \
"TnfRY+QTElth3XWZS8zTfshsH7ltM1oWqargPgBVZf15V49+qdNYiMsjpRWQ10zK\n" \
"u3jpIh0Wf1D9Ocm9yiR71vZ3W3TTvd5deQ7CVivu8lOVj/kYdM25Cn1TLu1Kzl+N\n" \
"BZNdcVYGGcaRiQkDJfEH4TjRjTsYcE0lJeiCIQsjS6ayPevvsxxPetYCN8zNXINl\n" \
"SwIDAQABo4HwMIHtMB0GA1UdDgQWBBQYJbpo8RsPW57tVkFM3c5Kvn58TjAfBgNV\n" \
"HSMEGDAWgBS3hSSq2mZIxyj+NNvPJ4eRQxA8xjAJBgNVHRMEAjAAMAsGA1UdDwQE\n" \
"AwIFoDARBglghkgBhvhCAQEEBAMCBsAwHQYDVR0lBBYwFAYIKwYBBQUHAwEGCCsG\n" \
"AQUFBwMCMCsGCWCGSAGG+EIBDQQeFhxPcGVuU1NMIEdlbmVyYXRlZCBDZXJ0aWZp\n" \
"Y2F0MDQGA1UdEQQtMCuGI2h0dHA6Ly9leGFtcGxlcy5mcmVlb3BjdWEuZ2l0aHVi\n" \
"LmlvhwTAqFhmMA0GCSqGSIb3DQEBCwUAA4IBAQBpMc0jcwMLuY0q0Rh7hIwYBEnL\n" \
"tQeoxn9quiQBQ6eJ/SBDXXifQsBKrFiHeXxb+snYQce07CwcRQHwoE0QYUgO87YK\n" \
"olUGsvUP4nvAoNiGPk9E2NeQDhCt36q6eFFyq+Vj9Ovo8hwwL66ftwmg4e+kQ/ce\n" \
"dZgRF5kNoEC5Z8uqTePdftfHoEW/vINKWSRwcphDP32j/1HCVPkN74nYb9ASrpLx\n" \
"41xZOEp0VbiCU5P+LIGi0WTiV7qqFEm64s35KCBc6wa+WknoHkwRbzT9g8e31i5y\n" \
"dSBeV8Qy//wb4/eWxAoTUU/gyysEfu4CIb3kAWQ3zgEsLc2wQzdT1yFCNA7A\n" \
"-----END CERTIFICATE-----";

  const char* local_client_key = \
"-----BEGIN PRIVATE KEY-----\n" \
"MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQDbX17ke55+KQT6\n" \
"MaxwPZ7v0RA49DPNs2f0Ve3g64BK685D53lL1xt+J70VBC2upwAZvMBbnF9cK94F\n" \
"AA3cK0vOeV2Mi9NSvvn2nF3UF3Rh6vVsu/EDrCHisJwD+IhwEE6R5OiLF0Qc+mXR\n" \
"JB2GMaNOd9Fj5BMSW2HddZlLzNN+yGwfuW0zWhapquA+AFVl/XlXj36p01iIyyOl\n" \
"FZDXTMq7eOkiHRZ/UP05yb3KJHvW9ndbdNO93l15DsJWK+7yU5WP+Rh0zbkKfVMu\n" \
"7UrOX40Fk11xVgYZxpGJCQMl8QfhONGNOxhwTSUl6IIhCyNLprI96++zHE961gI3\n" \
"zM1cg2VLAgMBAAECggEACIYfp0wgX/GTFm+oriHO062xbi0a+DKw5t8R46vy67ZW\n" \
"6O+NGIPOtyuvHqpvZfpqOBXL+aNNV0UDFvmmEDiEdQzlNClYuD3gc5Mm2C9jH8zj\n" \
"5Wo2FrjxBiBrp4HYN1aC3wtgd8sbI+8c1rALD6RNHqbsNW/ux8dSOaswni/ixenM\n" \
"Szl+FxMHdVW0ynvUbb8OrMMrSssiYG6GVp7XqIToDar4f88vXTIlVaRMdqxNHvbt\n" \
"5i/N2prUYFh9WvCKY9OK09oeYMK/A6Z/7jgF+je9NN5A7W4k4RuacvI/yCv/C3jo\n" \
"KbF/AYW9algrO7p0+IKNtkMF2j5hoFVC7Tp3bdQrGQKBgQD7HX1TzaVmlggLih2s\n" \
"XqCthU3T92YiNL/51UXLzWMPMMnhwO0b5a2AMZGU1g6BJ8LZ6FTK0BPf+Z2Ie+uN\n" \
"yox66gbXMWM/2DVEQHdepI7kU7QICqYAsUowlIhdnbsS5H6/j1SUZ3gXhrMQYZ0S\n" \
"ekS6Va3EWT+AhmfYENo+S6BVIwKBgQDfo87kT4bkngytLNx8iVlvtBz3wdCrcHQK\n" \
"FHSccD8GbeEgVU5sD3V+KMcabgCWRz+XgbtZkC3Q3rVDwwocQV6WaVUAr1KR4w/0\n" \
"Ub0g3f5bhvM1dt2hdLO3AP+kvFuiTejnW2CjzNdfJLROOH/W6nZnaeczA8kEXXIC\n" \
"Sv+Pyk4VuQKBgQCUEgt4guZo20bbJxHgNdwg9V2zA0qiGOCqfGuvhOhIaMhh6dTm\n" \
"gkQQNypUzPO4jK2EuE5nWNKfWxzdAdUfwrObNqvZ8DFJ0D94g1rf38CyOfkKVN0V\n" \
"w3SvuKXALeXFeh6S7f09lNVnafHkUuq/noEwLp1MDiQ3sQlCio1l9koQaQKBgCJG\n" \
"PKMI/Sd0gYTY50G+2xIfjID8FzrvFRmmYOjI2x/lHyeRa19sTxlyrnECgtyFNt3F\n" \
"aC0lx5/HQgiMj7Qc4J40rm5YcFMjdYKANHghDaaBXCzJdtYTyKc3Y/afV3vcbOFo\n" \
"Q7tW3XsbaPbD+fO6I0NeG6zZ26jrmVl4nQowex8hAoGAG4gjIjjmaTlNof8xjDiT\n" \
"kEj6uZDfaV/aaYNPv3Tty8rKlDWOWBksx7h+N1IIwpbseu6n9Czcnoqn7Pk05VfW\n" \
"OXkqIfE1UthAE3YtUAv3c3Zz1dLV0MNXtntFnpShntMUdOxvwT65xQVp4dYHJ2Rd\n" \
"Iqvpz0Y+/ekM2SapZ4nrPyo=\n" \
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