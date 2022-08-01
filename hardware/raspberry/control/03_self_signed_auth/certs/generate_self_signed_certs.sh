#!/bin/bash

mkdir certs_issued/
mkdir certs_issued/root/
mkdir certs_issued/mqtt/
mkdir certs_issued/opc/
mkdir certs_issued/influxdb/
mkdir certs_issued/grafana/
mkdir certs_issued/crl/


# https://www.paulligocki.com/create-openssl-ca-certificate-authority-dev-test-purposes/

touch index.txt
echo 00 > serial
echo 1000 > crlnumber

echo -e "
#===========================================================================
#       Generating Certificate Authority Key Pair
#===========================================================================\n"

echo -e " CA: Create Private Key & Self Signed Certificate\n"
openssl req -x509 -config ./cnf/openssl-ca.cnf -newkey rsa:2048 -nodes -out cacert.pem -outform PEM -days 3600
echo -e "  There should now to two more files\n
  cacert.pem (Self Signed Certificate)\n
  cakey.pem (CA Private Key) (WITHOUT A PASSWORD)\n"

echo -e "Inspect the CA Self Signed Certificate\n"
openssl x509 -in cacert.pem -text -noout

echo -e "Inspect the CA Self Signed Certificate for its Purpose/Ability\n"
openssl x509 -purpose -in cacert.pem -inform PEM
# There should now to two more files

openssl x509 -in cacert.pem -out cacert.der -outform DER

echo -e "
#===========================================================================
#       Generating OPC client certificate
#===========================================================================\n"

echo -e "Client: Generate Private Key & Certificate Signing Request (CSR)\n"
openssl req -config ./cnf/opc-client.cnf -newkey rsa:2048 -sha256 -nodes -out opc-client.cert.csr -outform PEM
echo -e "There should be two more files\n
   opc-client.key.pem (Client Private Key)\n
   opc-client.cert.scr (Certificate Signing Request)\n"

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in opc-client.cert.csr

echo -e "CA: Sign the CSR\n"
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-client.cert.pem -infiles opc-client.cert.csr
echo -e "There should be two more files\n
 opc-client.cert.pem (CA Signed Client Certificate)\n
 opc-client.key.pem (Client Private Key)\n"

# Merging the client with the root certificate
cat opc-client.cert.pem cacert.pem > opc-client-full.cert.pem

# Inspect the new Client Certificate
openssl x509 -in opc-client-full.cert.pem -text -noout

# Client: Create DER out of PEM
openssl x509 -in opc-client.cert.pem -out opc-client.cert.der -outform DER

echo -e "
#===========================================================================
#       Generating OPC server certificate
#===========================================================================\n"

echo -e "Server: Generate Private Key & Certificate Signing Request (CSR)\n"
openssl req -config ./cnf/opc-server.cnf -newkey rsa:2048 -sha256 -nodes -out opc-server.cert.csr -outform PEM
echo -e "There should be two more files\n
#   opc-server.key.pem (Server Private Key)\n
#   opc-server.cert.scr (Certificate Signing Request)\n"

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in opc-server.cert.csr

echo -e "CA: Sign the CSR\n"
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-server.cert.pem -infiles opc-server.cert.csr
echo -e "There should be two more files\n
 opc-server.cert.pem (CA Signed Server Certificate)\n
 opc-server.key.pem (Server Private Key)\n"

# Merging the server with the root certificate
cat opc-server.cert.pem cacert.pem > opc-server-full.cert.pem

# Inspect the new Server Certificate
openssl x509 -in opc-server.cert.pem -text -noout

# Server: Create DER out of PEM
openssl x509 -in opc-server-full.cert.pem -out opc-server.cert.der -outform DER

echo -e "
#===========================================================================
#       Generating MQTT client certificate
#===========================================================================\n"

echo -e "Client: Generate Private Key & Certificate Signing Request (CSR)\n"
openssl req -config ./cnf/mqtt-client.cnf -newkey rsa:2048 -sha256 -nodes -out mqtt-client.cert.csr -outform PEM
echo -e "There should be two more files\n
   mqtt-client.key.pem (Client Private Key)\n
   mqtt-client.cert.scr (Certificate Signing Request)\n"

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in mqtt-client.cert.csr

echo -e "CA: Sign the CSR\n"
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out mqtt-client.cert.pem -infiles mqtt-client.cert.csr
echo -e "There should be two more files\n
  mqtt-client.cert.pem (CA Signed Client Certificate)\n
  mqtt-client.key.pem (Client Private Key)\n"

# Merging the client with the root certificate
cat mqtt-client.cert.pem cacert.pem > mqtt-client-full.cert.pem

# Inspect the new Client Certificate
openssl x509 -in mqtt-client-full.cert.pem -text -noout

# Client: Create DER out of PEM
openssl x509 -in mqtt-client.cert.pem -out mqtt-client.cert.der -outform DER

echo -e "
#===========================================================================
#       Generating MQTT server certificate
#===========================================================================\n"

echo -e "Server: Generate Private Key & Certificate Signing Request (CSR)\n"
openssl req -config ./cnf/mqtt-server.cnf -newkey rsa:2048 -sha256 -nodes -out mqtt-server.cert.csr -outform PEM
echo -e "There should be two more files\n
  mqtt-server.key.pem (Server Private Key)\n
  mqtt-server.cert.scr (Certificate Signing Request)\n"

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in mqtt-server.cert.csr

echo -e "CA: Sign the CSR\n"
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out mqtt-server.cert.pem -infiles mqtt-server.cert.csr
echo -e "There should be two more files\n
  mqtt-server.cert.pem (CA Signed Server Certificate)\n
  mqtt-server.key.pem (Server Private Key)\n"

# Merging the server with the root certificate
cat mqtt-server.cert.pem cacert.pem > mqtt-server-full.cert.pem

# Inspect the new Server Certificate
openssl x509 -in mqtt-server.cert.pem -text -noout

# Server: Create DER out of PEM
openssl x509 -in mqtt-server-full.cert.pem -out mqtt-server.cert.der -outform DER

echo -e "
#===========================================================================
#       Generating Grafana certificates
#===========================================================================\n"

echo -e "Generate Private Key & Certificate Signing Request (CSR)\n"
openssl req -config ./cnf/grafana.cnf -newkey rsa:2048 -sha256 -nodes -out grafana.cert.csr -outform PEM
echo -e "There should be two more files\n
  grafana.key.pem (Server Private Key)\n
  grafana.cert.scr (Certificate Signing Request)\n"

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in grafana.cert.csr

echo -e "CA: Sign the CSR\n"
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out grafana.cert.pem -infiles grafana.cert.csr
echo -e "There should be two more files\n
  grafana.cert.pem (CA Signed Server Certificate)\n
  grafana.key.pem (Server Private Key)\n"

# Merging the server with the root certificate
cat grafana.cert.pem cacert.pem > grafana-full.cert.pem

# Inspect the new Server Certificate
openssl x509 -in grafana.cert.pem -text -noout

# Create DER out of PEM
openssl x509 -in grafana-full.cert.pem -out grafana.cert.der -outform DER

echo -e "
#===========================================================================
#       Generating InfluxDB certificates
#===========================================================================\n"

echo -e "Generate Private Key & Certificate Signing Request (CSR)\n"
openssl req -config ./cnf/influxdb.cnf -newkey rsa:2048 -sha256 -nodes -out influxdb.cert.csr -outform PEM
echo -e "There should be two more files\n
  influxdb.key.pem (Server Private Key)\n
  influxdb.cert.scr (Certificate Signing Request)\n"

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in influxdb.cert.csr

echo -e "CA: Sign the CSR\n"
openssl ca -config ./cnf/openssl-ca.cnf -policy signing_policy -extensions signing_req -out influxdb.cert.pem -infiles influxdb.cert.csr
echo -e "There should be two more files\n
  influxdb.cert.pem (CA Signed Server Certificate)\n
  influxdb.key.pem (Server Private Key)\n"

# Merging the server with the root certificate
cat influxdb.cert.pem cacert.pem > influxdb-full.cert.pem

# Inspect the new Server Certificate
openssl x509 -in influxdb.cert.pem -text -noout

# Create DER out of PEM
openssl x509 -in influxdb-full.cert.pem -out influxdb.cert.der -outform DER

echo -e "
#===========================================================================
#       Generating CRL
# ===========================================================================\n"

echo -e "Generate Certificate Revocation List (CRL)\n"
openssl ca -config ./cnf/openssl-ca.cnf -gencrl -out ./certs_issued/crl/rootca.crl.pem

openssl crl -in ./certs_issued/crl/rootca.crl.pem -text -noout

openssl crl -in ./certs_issued/crl/rootca.crl.pem -outform DER -out ./certs_issued/crl/rootca.crl

echo -e "
#===========================================================================
#       Generating Text Files
# ===========================================================================\n"

openssl x509 -in mqtt-client.cert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > pre-mqtt-client.cert.txt
sed 's/^/"/; s/$/\\n" \\/' pre-mqtt-client.cert.txt > mqtt-client.cert.txt

cat mqtt-client.key.pem > pre-mqtt-client.key.txt
sed 's/^/"/; s/$/\\n" \\/' pre-mqtt-client.key.txt > mqtt-client.key.txt

openssl x509 -in cacert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > pre-cacert.txt
sed 's/^/"/; s/$/\\n" \\/' pre-cacert.txt > cacert.txt

rm pre-*

cp cacert.pem certs_issued/mqtt/
cp cacert.pem certs_issued/opc/
cp cacert.pem certs_issued/influxdb/
cp cacert.pem certs_issued/grafana/

mv mqtt* certs_issued/mqtt/
mv cacert.txt certs_issued/mqtt/
mv opc* certs_issued/opc/
mv influx* certs_issued/influxdb/
mv grafana* certs_issued/grafana/

mv cacert.* certs_issued/root/
mv cakey.* certs_issued/root/
mv serial.* certs_issued/root/
mv serial certs_issued/root/
mv crlnumber certs_issued/root/
mv crlnumber.* certs_issued/root/
mv index.* certs_issued/root/

mv *.pem certs_issued/root/