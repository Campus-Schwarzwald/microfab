#!/bin/bash

# Example on how to copy files via SFTP to a remote host
#
# Please define the directory on the remote host according to your file structure

sftp -oPort=22 pi@MicrofabOPCClient002.local:/home/pi/build/certs/pki <<EOF
put ./opc-server/opc-server.cert.der
put ./opc-server/opc-server.key.pem
put ./opc-client/opc-client.cert.der
exit
EOF
