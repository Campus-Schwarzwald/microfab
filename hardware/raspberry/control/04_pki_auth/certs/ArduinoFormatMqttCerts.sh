#!/bin/bash

# Transforming already issued MQTT certs (key, cert and CA cert in a format that can be directly loaded to Arduino


echo -e "
#===========================================================================
#       Generating Text Files
# ===========================================================================\n"

openssl x509 -in ./certs_issued/mqtt-client/mqtt-client.cert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > ./certs_issued/mqtt-client/pre-mqtt-client.cert.txt
sed 's/^/"/; s/$/\\n" \\/' ./certs_issued/mqtt-client/pre-mqtt-client.cert.txt > ./certs_issued/mqtt-client/mqtt-client.cert.txt

cat ./certs_issued/mqtt-client/mqtt-client.key.pem > ./certs_issued/mqtt-client/pre-mqtt-client.key.txt
sed 's/^/"/; s/$/\\n" \\/' ./certs_issued/mqtt-client/pre-mqtt-client.key.txt > ./certs_issued/mqtt-client/mqtt-client.key.txt

#openssl x509 -in ./certs_issued/mqtt-client/ca-chain.cert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > ./certs_issued/mqtt-client/pre-ca-chain.cert.txt
#sed 's/^/"/; s/$/\\n" \\/' ./certs_issued/mqtt-client/pre-ca-chain.cert.txt > ./certs_issued/mqtt-client/ca-chain.cert.txt

rm ./certs_issued/mqtt-client/pre-*