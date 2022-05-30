# microfab
Microfab abstracts the data traffic of a production onto a board. With two ESP32 and a Raspberry Pi OPC UA and MQTT are explained clearly


```mermaid
  classDiagram;
      id1(MQTT Client 2)
      id2(MQTT Client 2)
      id3(MQTT Server & OPC UA Client)
      id4(OPC UA Server)
      
      id1 --> id3
      id2 --> id3
      id4 --> id4
      
      id1: ESP32
      id2: ESP32
      id3: Raspberry Pi 4b
      id4: Raspberry Pi 4b
      
      
```

```mermaid
  erDiagram;
      Grafana
      
      
```