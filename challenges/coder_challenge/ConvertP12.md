# Conversion of a .p12 file into the appropriate certificates and keys

## Generating the certificate out of the .p12 file
```bash
openssl pkcs12 -in YourP12File.p12 -out CertificateName.cert.pem -nokeys -chain -info
```

## Generating the private key out of the .p12 file
```bash
openssl pkcs12 -in YourP12File.p12 -out PrivateKeyName.key.pem -nocerts -nodes
```

## Final Structure
The final structure should look like this:

    certs/
    ├── ca-chain.cert.pem
    ├── ca-chain.key.pem
    ├── grafana
    │   ├── grafana-server.cert.pem
    │   ├── grafana-server.cert.csr
    │   └── grafana-server.key.pem
    ├── influxdb
    │   ├── influx-server.cert.pem
    │   ├── influx-server.cert.csr
    │   └── influx-server.key.pem
    └── mqtt
    │   ├── mqtt-client.cert.pem
    │   ├── mqtt-client.cert.csr
    │   ├── mqtt-client.key.pem
    │   ├── mqtt-server.cert.pem
    │   ├── mqtt-server.cert.csr
    │   ├── mqtt-server.key.pem
    └── opc
        ├── opc-client.cert.pem
        ├── opc-client.cert.csr
        ├── opc-client.key.pem
        ├── opc-server.cert.pem
        ├── opc-server.cert.csr
        ├── opc-server.key.pem

## Transfer to remote host

After generating all certificates and keys und need to transfer them to the remote host.

### Microfab Control

The respective containers must be able to consume the following certificates to function properly:

    certs/
    ├── Grafana
    │   ├── ca-chain.cert.pem
    │   ├── grafana-server.cert.pem
    │   └── grafana-server.key.pem
    ├── InfluxDB
    │   ├── ca-chain.cert.pem
    │   ├── influx-server.cert.pem
    │   └── influx-server.key.pem
    ├── Telegraf
    │   ├── ca-chain.cert.pem
    │   ├── mqtt-client.cert.pem
    │   ├── mqtt-client.key.pem
    │   ├── opc-client.cert.pem
    │   ├── opc-client.key.pem
    └── Mosquitto Broker
    │   ├── ca-chain.cert.pem
    │   ├── mqtt-client.cert.pem
    │   ├── mqtt-client.key.pem
    └── OPC Client
        ├── ca-chain.cert.pem
        ├── opc-client.cert.pem
        ├── opc-client.key.pem
        ├── opc-server.cert.pem

### Microfab OPC Server

The certificates should be placed in /home/pi/build/certs/pki

    home
    ├── pi
        ├── build
           ├── certs
                ├── pki
                    ├── opc-server.cert.pem
                    ├── opc-server.key.pem
                    ├── opc-client.cert.pem