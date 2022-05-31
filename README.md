# microfab
Microfab abstracts the data traffic of a production onto a board. With two ESP32 and a Raspberry Pi OPC UA and MQTT are explained clearly


```mermaid
  classDiagram;
      MicrofabControl -- WiFi_Router
      MQTT_Client -- WiFi_Router
      OPC_Server -- WiFi_Router

      class MicrofabControl{
          Raspberry Pi
          Docker
      }
      class WiFi_Router{
          TP-Link
          SSID: Microfab_00x
      }
      class MQTT_Client{
          Espressif ESP32
      }
      class OPC_Server{
          Raspberry Pi
          Open62541 OPC Server      
      }
      
      
```
