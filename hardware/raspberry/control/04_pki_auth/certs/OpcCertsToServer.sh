#!/bin/bash

# Copy certs for the OPC Server to a remote host

sftp -oPort=22 pi@MicrofabOPCServer002.local:/home/pi/build/certs/pki <<EOF
put -r ./certs_issued/opc-server/*
exit
EOF

