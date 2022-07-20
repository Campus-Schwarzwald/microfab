#!/bin/bash

# Example on how to copy files via SFTP to a remote host
#
# Please define the directory on the remote host according to your file structure

sftp -oPort=22 pi@MicrofabOPCServer002.local:/home/pi/build/certs/self_signed <<EOF
put ./certs_issued/opc/opc-server.cert.pem
put ./certs_issued/opc/opc-server.cert.der
put ./certs_issued/opc/opc-server.key.pem
put ./certs_issued/opc/opc-client.cert.der
put ./certs_issued/opc/opc-client.cert.pem
exit
EOF
