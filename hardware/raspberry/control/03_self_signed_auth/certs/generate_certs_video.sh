echo -e "
#===========================================================================
#       Generating Certificate Authority Key Pair
#===========================================================================\n"

openssl req -x509 -config ./cnf/openssl-ca.cnf -newkey rsa:2048 -nodes -out cacert.pem -outform PEM -days 3600

touch index.txt
echo 00 > serial

echo -e "
#===========================================================================
#       Generating OPC client certificate
#===========================================================================\n"

openssl req -config ./cnf/opc-client.cnf -newkey rsa:2048 -sha256 -nodes -out opc-client.cert.csr -outform PEM

openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-client.cert.pem -infiles opc-client.cert.csr

echo -e "
#===========================================================================
#       Generating OPC server certificate
#===========================================================================\n"

openssl req -config ./cnf/opc-server.cnf -newkey rsa:2048 -sha256 -nodes -out opc-server.cert.csr -outform PEM

openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-server.cert.pem -infiles opc-server.cert.csr

echo -e "
#===========================================================================
#       Generating MQTT client certificate
#===========================================================================\n"

openssl req -config ./cnf/mqtt-client.cnf -newkey rsa:2048 -sha256 -nodes -out mqtt-client.cert.csr -outform PEM

openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out mqtt-client.cert.pem -infiles mqtt-client.cert.csr

echo -e "
#===========================================================================
#       Generating MQTT server certificate
#===========================================================================\n"

openssl req -config ./cnf/mqtt-server.cnf -newkey rsa:2048 -sha256 -nodes -out mqtt-server.cert.csr -outform PEM

openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out mqtt-server.cert.pem -infiles mqtt-server.cert.csr

echo -e "
#===========================================================================
#       Generating Grafana certificates
#===========================================================================\n"

openssl req -config ./cnf/grafana.cnf -newkey rsa:2048 -sha256 -nodes -out grafana.cert.csr -outform PEM

openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out grafana.cert.pem -infiles grafana.cert.csr

echo -e "
#===========================================================================
#       Generating InfluxDB certificates
#===========================================================================\n"

openssl req -config ./cnf/influxdb.cnf -newkey rsa:2048 -sha256 -nodes -out influxdb.cert.csr -outform PEM

openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out influxdb.cert.pem -infiles influxdb.cert.csr

