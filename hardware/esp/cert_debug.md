

- Initial mosquitto.conf
    ``` bash
    log_type all
    allow_anonymous true
    
    listener 8883
    keyfile /path/mosquitto.key
    certfile /path/mosquitto.crt
    ```
  
- Restart Docker Compose

- Subscribe Data
    ``` bash
    mosquitto_sub -h 192.168.88.100 -p 8883 --cafile ./ca.crt -t ping/test -d
    ```
  
- Mosquitto log
    ``` bash
    1646405662: New connection from 192.168.88.253:52214 on port 8883.
    1646405662: New client connected from 192.168.88.253:52214 as auto-485ECD5E-5F5B-6A64-2EB1-7D84BF9A8A5A (p2, c1, k60).
    ```
- Terminal feedback
    ``` bash
    Client null sending CONNECT
    Client null received CONNACK (0)
    Client null sending SUBSCRIBE (Mid: 1, Topic: test/ping, QoS: 0, Options: 0x00)
    Client null received SUBACK
    Subscribed (mid: 1): 0
    ```
  
### With authenitication

- Change mosquitto.conf file
    ``` bash
    persistence true
    persistence_location /mosquitto/data/
    log_dest file /mosquitto/log/mosquitto.log
    
    
    listener 8883
    certfile /server.crt
    keyfile /server.key
    
    # We want to require a client certificate
    require_certificate true
    
    # This CA is used to verify the client certificate (it need not be the one used for the above mosquitto.crt)
    cafile /ca.crt
    
    # As we are passing a certificate we can choose to use the certificate CN as out username (removing need for allow_anonymous)
    use_identity_as_username true
    ```
- Restart docker

- Publish data
    ``` bash
    mosquitto_pub -h 192.168.88.100 -t test/florian --cafile ca.crt --cert client.crt --key client.key -m "hello world" -p 8883 -d
    ```
- Mosquitto logs
    ``` bash
    1646406314: New client connected from 192.168.88.253:52286 as auto-59C4F51D-F4F5-7BD4-410D-E57A63D60915 (p2, c1, k60, u'192.168.88.100').
    1646406314: Client auto-59C4F51D-F4F5-7BD4-410D-E57A63D60915 disconnected.
    ```

- Terminal feedback
    ``` bash
    Client null sending CONNECT
    Client null received CONNACK (0)
    Client null sending PUBLISH (d0, q0, r0, m1, 'test/florian', ... (11 bytes))
    Client null sending DISCONNECT
    ```