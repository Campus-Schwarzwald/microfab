#!/bin/bash

# Example on how to copy files via SFTP to a remote host
#
# Please define the directory on the remote host according to your file structure

mkdir remote_certs
mkdir remote_certs/opc
cp -r ./certs_issued/opc-client/* ./remote_certs/opc/

cp -r ./certs_issued/opc-server/* ./remote_certs/opc/
rm ./remote_certs/opc/opc-server.key*

mkdir remote_certs/telegraf
cp -r ./remote_certs/opc/* ./remote_certs/telegraf/
#rm ./remote_certs/telegraf/mqtt-client.key*
#rm ./remote_certs/telegraf/opc-server.key*

sftp -oPort=22 pi@MicrofabControl002.local:/home/pi/microfab/hardware/raspberry/control/04_pki_auth/certs <<EOF
put -r ./remote_certs/*
exit
EOF

rm -rf ./remote_certs/
