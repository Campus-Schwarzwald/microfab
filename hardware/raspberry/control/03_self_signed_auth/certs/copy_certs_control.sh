#!/bin/bash

# Example on how to copy files via SFTP to a remote host
#
# Please define the directory on the remote host according to your file structure

mkdir remote_certs
mkdir remote_certs/opc
cp -r ./certs_issued/opc/* ./remote_certs/opc/
rm ./remote_certs/opc/opc-server.key*

mkdir remote_certs/mqtt
cp -r ./certs_issued/mqtt/* ./remote_certs/mqtt/
rm ./remote_certs/mqtt/mqtt-client.key*

mkdir remote_certs/grafana
cp -r ./certs_issued/grafana/* ./remote_certs/grafana/

mkdir remote_certs/influxdb
cp -r ./certs_issued/influxdb/* ./remote_certs/influxdb/

mkdir remote_certs/telegraf
cp -r ./certs_issued/mqtt/* ./remote_certs/telegraf/
cp -r ./certs_issued/opc/* ./remote_certs/telegraf/
#rm ./remote_certs/telegraf/mqtt-client.key*
#rm ./remote_certs/telegraf/opc-server.key*

sftp -oPort=22 pi@MicrofabControl002.local:/home/pi/microfab/hardware/raspberry/control/03_self_signed_auth/certs <<EOF
put -r ./remote_certs/*
exit
EOF

rm -rf ./remote_certs/
