# Coder Challenge

- Find unencrypted OPC UA and / or MQTT traffic with a tool of your choice (e.g. Wireshark)

- Implement certificate based Authentication / Encryption for the Microfab

- Implement a QoS for the MQTT client higher than 0

## PKI implementation
- Get certificates (OPC, MQTT, Grafana, InfluxDB) from our PKI and transform the .p12 file in the proper format / files:
  - certificate (.pem and .der format)
  - key (.pem)
  - root CA certificate (chain)

- Implement the certificates in the MQTT client, OPC UA Server and Microfab Control


## Additional Questions / Challenges

- What are low-hanging fruits for a cyber attacker if he wants to get access to our system?
  - What can be done to mitigate them?


- Why is it a bad idea to hardcode passwords? What would be an alternative?
  - Can you implement a solution?
  

- Why are stored certificates not secure when located on the file system?
  - What would be a solution to secure them?
  - How can we manipulate the system with a stolen private key?

- Why is copying of private keys on several devices a strong security risk?
  - Have a look at the docker-compose file of the self signed certificate section

- Why is a long validity of a self-signed certificate a problem?
  - If using self-signed certificates. What are proper time spans?

- Is it a good manner to execute bash scripts from an unknown source? 
  - Did you check the script before executing it?

- How can we implement more security to the ESP32


- Where else could we use certificates?


- Why is it a good idea to implement a proxy between a server / client connection?


## Additional technical challenges

- Implement Flash Encryption on the ESP32

- Implement certificates for SSH and WiFi connection

- Use secure (not microfoo123 :D) passwords for authentication

- Implement a proxy in Microfab Control

- Show a Grafana Dashboard on the inbuilt Screen.
