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
echo "Defined CN name: "
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

        openssl req -new -x509 -days 3650 -subj "$SUBJECT_CA" -keyout ca.key -out ca.crt
}


function generate_domain_cert() {
        #===========================================================================
        #       Generating Certificate for Local Domain
        #===========================================================================
        echo "Step1.1: Generating Private Key for Local Domain"

        openssl genrsa -out domain.key 2048

        echo "Step 1.2: Generating A Signing Request for Local Domain Cert"

        openssl req -out domain.csr -key domain.key -subj "$SUBJECT_DOMAIN" -new

        echo "Step 1.3: Sending CSR to the CA"

        openssl x509 -req -in domain.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out domain.crt -days 3650
}


function generate_mqtt_server_cert() {
        #===========================================================================
        #       Generating Certificate for MQTT Broker
        #===========================================================================

        echo "STEP2: Generating Private Key for MQTT Broker"
        openssl genrsa -out mqtt-server.pem 2048

        echo "STEP2a: Generating a Signing Request for MQTT Broker Cert"
        openssl req -out mqtt-server.csr -key mqtt-server.pem -subj "$SUBJECT_MQTT_SERVER" -new

        echo "STEP2b: Sending CSR to the CA"
        openssl x509 -req -in mqtt-server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out mqtt-server.crt -days 3650
}

function generate_mqtt_client_cert () {
        #===========================================================================
        #         Generating Certificate for MQTT Clients
        #===========================================================================
        echo "$SUBJECT_MQTT_CLIENT"

        echo "STEP 4: Generating Private Key for MQTT Client Certificate"
        openssl req -new -nodes -sha256 -subj "$SUBJECT_MQTT_CLIENT" -out mqtt-client.csr -keyout mqtt-client.key

        openssl x509 -req -sha256 -in mqtt-client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out mqtt-client.crt -days 3650
}

function generate_opc_server_cert() {
        #===========================================================================
        #       Generating Certificate for OPC Broker
        #===========================================================================

        echo "STEP2: Generating Private Key for OPC Server"
        openssl genrsa -out opc-server.key 2048

        echo "STEP2a: Generating a Signing Request for OPC ServerCert"
        openssl req -out opc-server.csr -key opc-server.key -subj "$SUBJECT_MQTT_SERVER" -new

        echo "STEP2b: Sending CSR to the CA"
        openssl x509 -req -in opc-server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out opc-server.crt -days 3650
}

function generate_opc_client_cert () {
        #===========================================================================
        #         Generating Certificate for OPC Clients
        #===========================================================================
        echo "$SUBJECT_OPC_CLIENT"

        echo "STEP 4: Generating Private Key for OPC Client Certificate"
        openssl req -new -nodes -sha256 -subj "$SUBJECT_OPC_CLIENT" -out opc-client.csr -keyout opc-client.key

        openssl x509 -req -sha256 -in opc-client.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out opc-client.crt -days 3650
}


function generate_influxdb_cert() {
        #===========================================================================
        #       Generating Certificate for InfluxDB
        #===========================================================================

        echo "STEP3: Generating Private Key for INFLUXDB"
        openssl genrsa -out influx-server.key 2048

        echo "STEP3a: Generating a Signing Request for INFLUXDB"
        openssl req -out influx-server.csr -key influx-server.key -subj "$SUBJECT_INFLUXDB" -new

        echo "STEP3b: Sending CSR to the CA"
        openssl x509 -req -in influx-server.csr  -CA ca.crt -CAkey ca.key -CAcreateserial -out influx-server.crt -days 3650
}


function generate_grafana_cert() {
        #===========================================================================
        #         Generating Certificate for Grafana
        #===========================================================================

        echo "STEP5: Generating Private Key for GRAFANA"
        openssl genrsa -out grafana-server.key 2048

        echo "STEP5a: Generating a Signing Request for GRAFANA"
        openssl req -out grafana-server.csr -key grafana-server.key -subj "$SUBJECT_GRAFANA" -new

        openssl x509 -req -in grafana-server.csr -CA ca.crt -CAkey ca.key -CAcreateserial -out grafana-server.crt -days 3650
}


generate_ca
generate_domain_cert
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

cp ca.crt mqtt/
cp ca.crt opc/
cp ca.crt influxdb/
cp ca.crt grafana/

exit 0
