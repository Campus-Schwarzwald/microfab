#!/bin/bash

openssl x509 -in root-ca.cert.pem -out root-ca.cert.der -outform DER
openssl x509 -in playground-ca.cert.pem -out playground-ca.cert.der -outform DER
openssl x509 -in hackthefab-ca.cert.pem -out hackthefab-ca.cert.der -outform DER