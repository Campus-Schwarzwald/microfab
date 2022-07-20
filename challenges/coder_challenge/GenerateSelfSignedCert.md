# Generating a self signed CA with certificates

The issued certificates are created using a Config (.cnf) file. The respective config files are stored under 
/hardware/raspberry/control/03_self_signed_auth/certs/cnf. For the creation of the respective 
files the corresponding config file must be sent in the openssl command to

## Generating Certificate Authority Key Pair

### Create Private Key & Self Signed Certificate
```bash
openssl req -x509 -config ./cnf/openssl-ca.cnf -newkey rsa:2048 -nodes -out cacert.pem -outform PEM -days 3600
```

## Example: Generating OPC client certificate

### Generate Private Key & Certificate Signing Request (CSR)
```bash
openssl req -config ./cnf/opc-client.cnf -newkey rsa:2048 -sha256 -nodes -out opc-client.cert.csr -outform PEM
```

### Sign the CSR
```bash
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-client.cert.pem -infiles opc-client.cert.csr
```

```bash
cat opc-client.cert.pem cacert.pem > opc-client-full.cert.pem
```

> All further certificates can be created according to the same scheme

## Final Structure
The final structure should look like this:

    certs/
    ├── cacert.pem
    ├── cakey.pem
    ├── cacert.srl
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
    │   ├── cacert.pem
    │   ├── grafana-server.cert.pem
    │   └── grafana-server.key.pem
    ├── InfluxDB
    │   ├── cacert.pem
    │   ├── influx-server.cert.pem
    │   └── influx-server.key.pem
    ├── Telegraf
    │   ├── cacert.pem
    │   ├── mqtt-client.cert.pem
    │   ├── mqtt-client.key.pem
    │   ├── opc-client.cert.pem
    │   ├── opc-client.key.pem
    └── Mosquitto Broker
    │   ├── cacert.pem
    │   ├── mqtt-client.cert.pem
    │   ├── mqtt-client.key.pem
    └── OPC Client
        ├── cacert.pem
        ├── opc-client.cert.pem
        ├── opc-client.key.pem
        ├── opc-server.cert.pem


### Microfab OPC Server

The certificates should be placed in /home/pi/build/certs/self_signed

    home
    ├── pi
        ├── build
           ├── certs
                ├── self_signed
                    ├── opc-server.cert.pem
                    ├── opc-server.key.pem
                    ├── opc-client.cert.pem
