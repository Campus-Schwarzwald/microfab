#!/bin/bash

CERTSDIR="certs"
mkdir $(pwd)/$CERTSDIR
mkdir $(pwd)/$CERTSDIR/crl


# https://www.paulligocki.com/create-openssl-ca-certificate-authority-dev-test-purposes/

touch index.txt
echo 00 > serial
echo 1000 > crlnumber

# CA: Create Private Key & Self Signed Certificate
openssl req -x509 -config openssl-ca.cnf -newkey rsa:2048 -nodes -out cacert.pem -outform PEM -days 3600
# There should now to two more files
#  cacert.pem (Self Signed Certificate)
#  cakey.pem (CA Private Key) (WITHOUT A PASSWORD)

# Inspect the CA Self Signed Certificate
openssl x509 -in cacert.pem -text -noout

# Inspect the CA Self Signed Certificate for its Purpose/Ability
openssl x509 -purpose -in cacert.pem -inform PEM
# There should now to two more files

openssl x509 -in cacert.pem -out cacert.der -outform DER

## Generate Client Certificates

# Client: Generate Private Key & Certificate Signing Request (CSR)
openssl req -config openssl-client.cnf -newkey rsa:2048 -sha256 -nodes -out opc-client.cert.csr -outform PEM
# There should be two more files
#   opc-client.key.pem (Client Private Key)
#   opc-client.cert.scr (Certificate Signing Request)

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in opc-client.cert.csr

# CA: Sign the CSR
openssl ca -config openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-client.cert.pem -infiles opc-client.cert.csr
# There should be two more files
# opc-client.cert.pem (CA Signed Client Certificate)
# opc-client.key.pem (Client Private Key)

# Merging the client with the root certificate
cat opc-client.cert.pem cacert.pem > opc-client-full.cert.pem

# Inspect the new Client Certificate
openssl x509 -in opc-client-full.cert.pem -text -noout

# Client: Create DER out of PEM
openssl x509 -in opc-client.cert.pem -out opc-client.cert.der -outform DER



## Generate Server Certificates

# Server: Generate Private Key & Certificate Signing Request (CSR)
openssl req -config openssl-server.cnf -newkey rsa:2048 -sha256 -nodes -out opc-server.cert.csr -outform PEM
# There should be two more files
#   opc-server.key.pem (Server Private Key)
#   opc-server.cert.scr (Certificate Signing Request)

# Inspect the CSR (Certificate Signing Request)
openssl req -text -noout -verify -in opc-server.cert.csr

# CA: Sign the CSR
openssl ca -config openssl-ca.cnf -policy signing_policy -extensions signing_req -out opc-server.cert.pem -infiles opc-server.cert.csr
# There should be two more files
# opc-server.cert.pem (CA Signed Server Certificate)
# opc-server.key.pem (Server Private Key)

# Merging the server with the root certificate
cat opc-server.cert.pem cacert.pem > opc-server-full.cert.pem

# Inspect the new Server Certificate
openssl x509 -in opc-server.cert.pem -text -noout

# Server: Create DER out of PEM
openssl x509 -in opc-server-full.cert.pem -out opc-server.cert.der -outform DER

# Generate CRL List

# Generate Certificate Revocation List (CRL)
openssl ca -config openssl-ca.cnf -gencrl -out ./certs/crl/rootca.crl.pem

openssl crl -in ./certs/crl/rootca.crl.pem -text -noout

openssl crl -in ./certs/crl/rootca.crl.pem -outform DER -out ./certs/crl/rootca.crl

mv opc-server.* certs/
mv opc-client.* certs/
mv index.* certs/
mv *.pem certs/
mv *.der certs/
mv serial certs/
mv serial.* certs/
mv crlnumber* certs/


