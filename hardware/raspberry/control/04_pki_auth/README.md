# Required certificates

## Naming convention

- A root, self-signed certificate and corresponding private keys in encrypted PEM format
  - ca_root.cert.pem
  - ca_root.key.pem

- A server certificate and corresponding private keys in encrypted PEM format issued by ca_root.cert.pem and used by the server (e.g. Mosquitto Broker)
  - <NAME OF THE SERVER>-server.cert.pem
  - <NAME OF THE SERVER>-server.key.pem

A client certificate and corresponding private key in encrypted PEM format issued by ca_root.cert.pem and used by the client (e.g. Mosquitto Client)
  - <NAME OF THE CLIENT>-client.cert.pem
  - <NAME OF THE CLIENT>-client.key.pem

## Structure

    certs/
    ├── ca_root.cert.pem
    ├── ca_root.key.pem
    ├── ca_root.cert.srl
    ├── grafana
    │   ├── ca_root.cert.pem
    │   ├── grafana-server.cert.pem
    │   ├── grafana-server.cert.csr
    │   └── grafana-server.key.pem
    ├── influxdb
    │   ├── ca_root.cert.pem
    │   ├── influx-server.cert.pem
    │   ├── influx-server.cert.csr
    │   └── influx-server.key.pem
    └── mqtt
    │   ├── ca_root.cert.pem
    │   ├── mqtt-client.cert.pem
    │   ├── mqtt-client.cert.csr
    │   ├── mqtt-client.key.pem
    │   ├── mqtt-server.cert.pem
    │   ├── mqtt-server.cert.csr
    │   ├── mqtt-server.key.pem
    └── opc
        ├── ca_root.cert.pem
        ├── opc-client.cert.pem
        ├── opc-client.cert.csr
        ├── opc-client.key.pem
        ├── opc-server.cert.pem
        ├── opc-server.cert.csr
        ├── opc-server.key.pem

## OPC & MQTT Profiles

### Certificate Profiles

**Key Algorithm**: RSA
**Bit Length**: 2048
**Key Usage**: Digital Signature, Key encipherment
**Extended Key Usage**: Client Authentication, Server Authentication

### End Entity Profiles

#### Server

**CN**: 192.168.88.100
**OU**: HackTheFab
**O**: Campus Schwarzwald
**C**: DE

**Token**: P12 file


## Additional

- Public Key is a subset of the Private Key (for RSA) = often called key pair
- A certificate is a signed object (X.509)
- Most certificates contain the public key and an identity (owner of the key)
- Certificate is often used in terms of PKCS#12 / PFX files which contains the certificate and the private key
- There are no private or public certificates
- 