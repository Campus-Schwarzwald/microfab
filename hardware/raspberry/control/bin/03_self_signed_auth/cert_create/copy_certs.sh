#!/bin/bash

# Example on how to copy files via SFTP to a remote host
#
# Please define the directory on the remote host according to your file structure

sftp -oPort=22 pi@MicrofabOPCClient002.local:/home/pi/build/certs/self_signed <<EOF
put ./certs/opc-server.cert.pem
put ./certs/opc-server.cert.der
put ./certs/opc-server.key.pem
put ./certs/opc-client.cert.der
put ./certs/opc-client.cert.pem
exit
EOF
