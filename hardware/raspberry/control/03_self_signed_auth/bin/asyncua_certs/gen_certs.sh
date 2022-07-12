#!/bin/bash

openssl genrsa -out key.pem 2048

openssl req -x509 -days 365 -new -out certificate.pem -key key.pem -config ssl.conf

openssl req -x509 -newkey rsa:4096 -sha256 -keyout my_private_key.pem -out my_cert.pem -days 3650 -nodes -addext "subjectAltName = URI:urn:example.org:FreeOpcUa:python-opcua"

openssl x509 -outform der -in my_cert.pem -out my_cert.der