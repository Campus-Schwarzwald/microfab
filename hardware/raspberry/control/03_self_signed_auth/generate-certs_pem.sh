#!/bin/bash
#============================================================================
#       FILE: generate-certs.sh
#       USAGE: ./generate-certs.sh
#   DESCRIPTION: Script to generate self-signed certificates for:
#                   1. Certificate Authority (CA)
#                       1.1 Domain (localhost)
#                   2. InfluxDB
#                   3. Mosquitto MQTT Broker
#
#============================================================================

CERTSDIR="certs"

# NOTE: DO NOT change the `OU` or `CN` values

# Use IP Address for CN
CN=$(hostname -I | awk '{print $1}')
CN='192.168.88.100'

SUBJECT_CA="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=Certificate Authority"
SUBJECT_DOMAIN="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=DOMAIN/CN=${CN}"
SUBJECT_MQTT_SERVER="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=MQTTSERVER/CN=${CN}"
SUBJECT_INFLUXDB="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=INFLUXDB/CN=${CN}"
SUBJECT_MQTT_CLIENT="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=MQTTCLIENT/CN=${CN}"
SUBJECT_GRAFANA="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=GRAFANA/CN=${CN}"
SUBJECT_OPC_CLIENT="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=OPCCLIENT/CN=${CN}"
SUBJECT_OPC_SERVER="/C=DE/ST=BW/L=Freudenstadt/O=CSW/OU=OPCSERVER/CN=${CN}"


mkdir $(pwd)/$CERTSDIR
cd $(pwd)/$CERTSDIR
mkdir mqtt/
mkdir opc/
mkdir influxdb/
mkdir grafana/ # Do not generate Grafana Certificate for time-being

function generate_ca() {
        #===========================================================================
        #       Generating Certificate Authority Key Pair
        #===========================================================================

        echo "STEP1: Generating Certificate Authority"
        echo $SUBJECT_CA

        openssl req -new -x509 -days 3650 -subj "$SUBJECT_CA" -keyout ca_root.key.pem -out ca_root.cert.pem
}

function generate_mqtt_server_cert() {
        #===========================================================================
        #       Generating Certificate for MQTT Broker
        #===========================================================================

        echo "STEP2: Generating Private Key for MQTT Broker"
        openssl genrsa -out mqtt-server.key.pem 2048

        echo "STEP2a: Generating a Signing Request for MQTT Broker Cert"
        openssl req -out mqtt-server.cert.csr -key mqtt-server.key.pem -subj "$SUBJECT_MQTT_SERVER" -new

        echo "STEP2b: Sending CSR to the CA"
        openssl x509 -req -in mqtt-server.cert.csr -CA ca_root.cert.pem -CAkey ca_root.key.pem -CAcreateserial -out mqtt-server.cert.pem -days 3650
}

function generate_mqtt_client_cert () {
        #===========================================================================
        #         Generating Certificate for MQTT Clients
        #===========================================================================
        echo "$SUBJECT_MQTT_CLIENT"

        echo "STEP 3a: Generating Private Key and Certificate Signing Request for MQTT Client Certificate"
        openssl req -new -nodes -sha256 -subj "$SUBJECT_MQTT_CLIENT" -out mqtt-client.cert.csr -keyout mqtt-client.key.pem

        echo "STEP 3b: Generating a MQTT Client Certificate"
        openssl x509 -req -sha256 -in mqtt-client.cert.csr -CA ca_root.cert.pem -CAkey ca_root.key.pem -CAcreateserial -out mqtt-client.cert.pem -days 3650
}

function generate_opc_server_cert() {
        #===========================================================================
        #       Generating Certificate for OPC Broker
        #===========================================================================

        echo "STEP4: Generating Private Key for OPC Server"
        openssl genrsa -out opc-server.key.pem 2048

        echo "STEP4a: Generating a Signing Request for OPC ServerCert"
        openssl req -out opc-server.cert.csr -key opc-server.key.pem -subj "$SUBJECT_MQTT_SERVER" -new

        echo "STEP4b: Sending CSR to the CA"
        openssl x509 -req -in opc-server.cert.csr -CA ca_root.cert.pem -CAkey ca_root.key.pem -CAcreateserial -out opc-server.cert.pem -days 3650
}

function generate_opc_client_cert () {
        #===========================================================================
        #         Generating Certificate for OPC Clients
        #===========================================================================
        echo "$SUBJECT_OPC_CLIENT"

        echo "STEP 5a: Generating Private Key  and Certificate Signing Request for OPC Client Certificate"
        openssl req -new -nodes -sha256 -subj "$SUBJECT_OPC_CLIENT" -out opc-client.cert.csr -keyout opc-client.key.pem

        echo "STEP 5b: Generating a MQTT Client Certificate"
        openssl x509 -req -sha256 -in opc-client.cert.csr -CA ca_root.cert.pem -CAkey ca_root.key.pem -CAcreateserial -out opc-client.cert.pem -days 3650
}


function generate_influxdb_cert() {
        #===========================================================================
        #       Generating Certificate for InfluxDB
        #===========================================================================

        echo "STEP6: Generating Private Key for INFLUXDB"
        openssl genrsa -out influx-server.key.pem 2048

        echo "STEP6a: Generating a Signing Request for INFLUXDB"
        openssl req -out influx-server.cert.csr -key influx-server.key.pem -subj "$SUBJECT_INFLUXDB" -new

        echo "STEP6b: Sending CSR to the CA"
        openssl x509 -req -in influx-server.cert.csr -CA ca_root.cert.pem -CAkey ca_root.key.pem -CAcreateserial -out influx-server.cert.pem -days 3650
}


function generate_grafana_cert() {
        #===========================================================================
        #         Generating Certificate for Grafana
        #===========================================================================

        echo "STEP7: Generating Private Key for GRAFANA"
        openssl genrsa -out grafana-server.key.pem 2048

        echo "STEP7a: Generating a Signing Request for GRAFANA"
        openssl req -out grafana-server.cert.csr -key grafana-server.key.pem -subj "$SUBJECT_GRAFANA" -new

        openssl x509 -req -in grafana-server.cert.csr -CA ca_root.cert.pem -CAkey ca_root.key.pem -CAcreateserial -out grafana-server.cert.pem -days 3650
}


generate_ca
generate_mqtt_server_cert
generate_mqtt_client_cert
generate_opc_server_cert
generate_opc_client_cert
generate_influxdb_cert
generate_grafana_cert # Do Not Generate Grafana certificate for time-being

echo "Moving Certificates in to dedicated directories"

mv mqtt-server.* mqtt/
mv mqtt-client.* mqtt/
mv opc-server.* opc/
mv opc-client.* opc/
mv influx-server.* influxdb/
mv grafana-server.* grafana/

echo "Copying the CA Certificate for Mosquitto, InfluxDB, Grafana"

cp ca_root.cert.pem mqtt/
cp ca_root.cert.pem opc/
cp ca_root.cert.pem influxdb/
cp ca_root.cert.pem grafana/

exit 0
