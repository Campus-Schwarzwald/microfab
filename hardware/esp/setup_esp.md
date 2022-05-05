## Step-by-step

- Arduino configuration

    - Download and install the Arduino Software IDE
    
        - https://www.arduino.cc/en/software
    
    - Add Additional Boards Manager URL
    
        - Arduino -> Preferences -> Additional Boards Manager URL
    
            - https://dl.espressif.com/dl/package_esp32_index.json

    - Install ESP32 development board
    
        - Click Tools -> Development Board -> Development Board Management -> Search ESP32 -> Install
    
    - Install PubSub Client
        - Project -> Load library -> Library manager... -> Search PubSubClient -> Install PubSubClient by Nick O’Leary
    
    - Upload files to the ESP
        - To upload files to the ESP follow [these](https://github.com/me-no-dev/arduino-esp32fs-plugin) instructions
        - Use-Case: [Upload certificates to the ESP](../mqtt_self_signed.md)
            - Create a ```data``` folder in your Arduino sketch folder
            - Inside the ```data``` folder create a new folder called ```keys```  
            - Copy the following files in the ```keys``` folder:
                - client.crt
                - client.key
                - ca.crt

Sources:
- https://www.emqx.com/en/blog/esp32-connects-to-the-free-public-mqtt-broker
- https://randomnerdtutorials.com/installing-the-esp32-board-in-arduino-ide-windows-instructions/
- https://github.com/me-no-dev/arduino-esp32fs-plugin