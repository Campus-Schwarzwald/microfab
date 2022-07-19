#!/bin/bash


openssl x509 -in root-ca.cert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > pre-root-ca.cert.txt
sed 's/^/"/; s/$/\\n" \\/' pre-root-ca.cert.txt > ca-chain.cert.txt

openssl x509 -in playground-ca.cert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > pre-playground-ca.cert.txt
sed 's/^/"/; s/$/\\n" \\/' pre-playground-ca.cert.txt >> ca-chain.cert.txt

openssl x509 -in hackthefab-ca.cert.pem -text -certopt no_header,no_pubkey,no_subject,no_issuer,no_signame,no_version,no_serial,no_validity,no_extensions,no_sigdump,no_aux,no_extensions > pre-hackthefab-ca.cert.txt
sed 's/^/"/; s/$/\\n" \\/' pre-hackthefab-ca.cert.txt >> ca-chain.cert.txt

rm pre-*