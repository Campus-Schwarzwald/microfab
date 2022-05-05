#include <WiFi.h>
//#include <WiFiClientSecure.h>  //included WiFiClientSecure does not work!
#include "src/dependencies/WiFiClientSecure/WiFiClientSecure.h" //using older WiFiClientSecure
#include <time.h>
#include <PubSubClient.h>
#include<ArduinoJson.h>

#ifndef SECRET
  const char ssid[] = "Mikrofab_01";
  const char pass[] = "mikrofoo123";

  #define HOSTNAME "192.168.88.100"

  const char *MQTT_HOST = "192.168.88.100";
  const int MQTT_PORT = 8883;
  const char *MQTT_USER = "pubclient"; // leave blank if no credentials used
  const char *MQTT_PASS = "tiguitto"; // leave blank if no credentials used

  const char* local_root_ca = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDpTCCAo2gAwIBAgIUDL/Vkt8GUhTDsjXS9rcJQ7g94KIwDQYJKoZIhvcNAQEL\n" \
    "BQAwYjELMAkGA1UEBhMCREUxDzANBgNVBAgMBkJyZW1lbjEPMA0GA1UEBwwGQnJl\n" \
    "bWVuMREwDwYDVQQKDAhUSUdVSVRUTzEeMBwGA1UECwwVQ2VydGlmaWNhdGUgQXV0\n" \
    "aG9yaXR5MB4XDTIyMDMwNzEzMjk1NloXDTMyMDMwNDEzMjk1NlowYjELMAkGA1UE\n" \
    "BhMCREUxDzANBgNVBAgMBkJyZW1lbjEPMA0GA1UEBwwGQnJlbWVuMREwDwYDVQQK\n" \
    "DAhUSUdVSVRUTzEeMBwGA1UECwwVQ2VydGlmaWNhdGUgQXV0aG9yaXR5MIIBIjAN\n" \
    "BgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEApEdgV0uLxACf7bfqypcYE09fwvhX\n" \
    "n0elHkIkGCK3QczbFsEVTyZVvjS5SJPkkUWt26nf95474gCL6WJNhRBlVj4O1/A0\n" \
    "Na6rzIt/55xETrhYFrHvwminTF88WgYSX4EYTrHmqj9tpjEDudk30g84ivtVEnRx\n" \
    "2eE6Rr5IDlt4JRJRgtZsriRvmmtRSro1ZgB4RHF0DmQgB6eO/TGCfR1x/bb7g2BU\n" \
    "wdxz7Tps7sSk4lZdnxb6LTTv+Mr2IdvTaa4kQQnsLiEiOJUPxpIgLWNUrRuj/rA5\n" \
    "FJ21YfVq8G/5poAzpCRV8SbadMHt08mPGd24YnnGmxdqna7VLvKS7umGXQIDAQAB\n" \
    "o1MwUTAdBgNVHQ4EFgQU5Hk3bbtHqLArrVAA2Av2SAx8GQswHwYDVR0jBBgwFoAU\n" \
    "5Hk3bbtHqLArrVAA2Av2SAx8GQswDwYDVR0TAQH/BAUwAwEB/zANBgkqhkiG9w0B\n" \
    "AQsFAAOCAQEANTWSepMv/FZdT8qeebTiPF8BfOiDtC5nF82iLWQJ6/rS8wzZe+f0\n" \
    "r3w8GJjsURwNxNHryikGEjEQqRWk3yhJzm4Q2NPSyxJP/ZrfwNh8OQyH4xxExq92\n" \
    "9kiGeSTLSgt0ntWCEkLpYpeJ1L/6kH+uJ/lNsJqm8TPX7tY+53MnTO+dB+dQPnu7\n" \
    "umByx8KJJ1PRLGJX/FZ72NsEViZOoKMXSUQHLSPfxm/5oRd8ynRYaW+IZ4KdLznJ\n" \
    "xe+3FWzmc4V18kf4JCzl1MADwMHsQGmpmUx2ftJxqYEdXO585ZtV8YmTYp/Imoru\n" \
    "earH650FUxB12ROO0kA1SrP9sBMxHFQjQQ==\n" \
    "-----END CERTIFICATE-----";


  const char* local_client_crt = \
    "-----BEGIN CERTIFICATE-----\n" \
    "MIIDVTCCAj0CFFaLLY2WBAxePlWLG3bThsMteFzqMA0GCSqGSIb3DQEBCwUAMGIx\n" \
    "CzAJBgNVBAYTAkRFMQ8wDQYDVQQIDAZCcmVtZW4xDzANBgNVBAcMBkJyZW1lbjER\n" \
    "MA8GA1UECgwIVElHVUlUVE8xHjAcBgNVBAsMFUNlcnRpZmljYXRlIEF1dGhvcml0\n" \
    "eTAeFw0yMjAzMDcxMzMwMTFaFw0zMjAzMDQxMzMwMTFaMGwxCzAJBgNVBAYTAkRF\n" \
    "MQ8wDQYDVQQIDAZCcmVtZW4xDzANBgNVBAcMBkJyZW1lbjENMAsGA1UECgwEQklC\n" \
    "QTETMBEGA1UECwwKTVFUVENMSUVOVDEXMBUGA1UEAwwOMTY5LjI1NC4zMS4yNDgw\n" \
    "ggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQC+Q7ZYBPDgxbdQzxnw4JZm\n" \
    "DS0FUDSPDNotw6yZhfmqrWwJxVeG20FWU61p2Tbhnz6Ya6/cSAUQvmPW0Ik0kkzM\n" \
    "txvHTgtqI3kNalBYVJOuddN5PF+Ks8kCd6KTogCjjFRJaVyCSm+1PSs5pAQ5EXaQ\n" \
    "ymzNMvsgvGkaNY0mzZxHshnZkuCChvRMmTDNxz+h5z74GQsF/9b01FcWxNLyA5O4\n" \
    "/fXsFHVcgqchtoNNrcBWYHV3HEWX3CijN16jWOKbSLwY+LYQQ6hCnyXlO9r7TwrB\n" \
    "ujaB2zZTRwO4BgdFphrYRFHZFNgl92On1/RCcphy0bTGyZS0gaMACJ9mfkO0hdwh\n" \
    "AgMBAAEwDQYJKoZIhvcNAQELBQADggEBAIVAdVfRaJryVoz8MOA7oeV7A0jgjlCW\n" \
    "7WFylB0u3ZE1KV+UhlRH+6tfKzTPkfNpEvfNjbzwc+dDle34MvdYPJE4+iTlJKVF\n" \
    "pVTOe7MehX38sairEar232uMb1UVmeBSruAjCCMoLm5l4VgJj/JBr/PUY1uEBfux\n" \
    "WkTLG7ZMHU2NmsyXJyPspu7+jQRzx4VW1CYmczlBzhCwlTCa1DNm0L9kuG+Ao0dJ\n" \
    "65X4dXgf4OqlGIyO9e9SIvQ0YJJ2iZ387D6a9TOSsZflLMaswEmYUex7jws17R8X\n" \
    "8AYwzZHihC5dKeDDxm6uj2PBpcwUd8DbpaOGrgGMLMJ2Munz92tbM2U=\n" \
    "-----END CERTIFICATE-----";

  const char* local_client_key = \
    "-----BEGIN PRIVATE KEY-----\n" \
    "MIIEvQIBADANBgkqhkiG9w0BAQEFAASCBKcwggSjAgEAAoIBAQC+Q7ZYBPDgxbdQ\n" \
    "zxnw4JZmDS0FUDSPDNotw6yZhfmqrWwJxVeG20FWU61p2Tbhnz6Ya6/cSAUQvmPW\n" \
    "0Ik0kkzMtxvHTgtqI3kNalBYVJOuddN5PF+Ks8kCd6KTogCjjFRJaVyCSm+1PSs5\n" \
    "pAQ5EXaQymzNMvsgvGkaNY0mzZxHshnZkuCChvRMmTDNxz+h5z74GQsF/9b01FcW\n" \
    "xNLyA5O4/fXsFHVcgqchtoNNrcBWYHV3HEWX3CijN16jWOKbSLwY+LYQQ6hCnyXl\n" \
    "O9r7TwrBujaB2zZTRwO4BgdFphrYRFHZFNgl92On1/RCcphy0bTGyZS0gaMACJ9m\n" \
    "fkO0hdwhAgMBAAECggEAKwcbJJqxctOj2lWLs0rHGHsgVFRIHpW0facD0JBY3v1r\n" \
    "MmZ7ndbV1tUDoa1q2OlveX7Y/YMYCipUX883SDlRvvYmBROkRzOQe7UwSFmQMGMb\n" \
    "GQ7LOI3iBnnRvXtkk5f3s8ZtElYNHzqfHOD3x4QogSQWKO2/ly6kwjWcH0tebWP4\n" \
    "EF6eTsJmMjX8YW5RgNXBQkdakawNV0NWUioHFc8aYGQk+h7HkNrm945sgapeRy82\n" \
    "Q+oRAsdNk+V8XDvgYHe1AT/9MBk+9ei8Q+h7XlJWgDLtrHb8QG9LTI0zCQmG/ARC\n" \
    "PQXJ9QYrdRf/40dAO9xM0YTXsf/0DVOU3jF0tjplFQKBgQD6YsLN8K3G9TRHeZuz\n" \
    "H/3ite4ab7RQMdR/fEmEWRgeyrRmqpTqrxA0NGZWgI2a1xtQjOPXb6qthrbkZT9F\n" \
    "GXVwK1xpw4r6dCEoOy9LNelILy+DlSr8lNeYRGzfm4BvEwL1Pk/En3fnpFo6CjlK\n" \
    "lS5g4xNtyFbwavtv3cXOlTTTVwKBgQDCh9lnyYGfQxzfqoWgqYN73fmYFLQumJDR\n" \
    "5Q8+gH4mvuG+Z+8yzMWWAAe27iLTIYWvUdEff9jmXzVRtUuk3AFSBrU/6eEG8uQj\n" \
    "nmTxEnTcCl98DzSOqrwdkjtI0wWqBD6277eI5a+mYRWf6IrFZDxRfeCEu9f1xL99\n" \
    "Q/ggbApZRwKBgQDzthmDNpcPJzDBuV6R1bvbZdXiqYPK4xXBGQq95bKSlh9OJw9X\n" \
    "9xpAlhhebjZm1qVhnCFRWwG4cX2nhd01tvgWPD5xO8IU0eBMNn9sJ4dvmW4YF6OF\n" \
    "wqKvNTr703IJq2mfqA0gh0/Airq/75+gNuCzsZMBspTc4mTml+inkZefdQKBgC/L\n" \
    "fOj5I3Xj4WY62VYeEAYRBdAN1i9UoNBRPSuYGmaeo4xKsQSEN0+hdkPFwpypMdVf\n" \
    "fsgEe2AuqEUJDCrXOyPSLhoj9nvklZUCx75LUJb3nXo3qiwbWtjRIuvDnTKmlFoS\n" \
    "D7/83uk8FToSwnW5KPDEwrMaC/eAfaPjXUVqBijlAoGAZUne9HIKas99t5nbw1kl\n" \
    "6dpC+L6Uwo9VckwPRIh7JBZP78upRS2O8G3z8QEREFHlM9EORzFhKCNUUd5bML+a\n" \
    "dncKT9PxY30WwamJ/gxX0O14ysRuL9GzbwjgdnDck1eky2Qv9BpG4WNSuwm8eZQX\n" \
    "OYVXIN85Rboz4StAt/feuIU=\n" \
    "-----END PRIVATE KEY-----";

#endif

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
      WiFi.begin(ssid, pass);
      //WiFi.config(IPAddress(ip_static), IPAddress(ip_gateway), IPAddress(ip_subnet), IPAddress(ip_dns));

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