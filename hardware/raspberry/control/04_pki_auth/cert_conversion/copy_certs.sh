#!/bin/bash
#============================================================================
#       FILE: copy_cert.sh
#       USAGE: bash copy_certs.sh -n YOUR_FILE.pem -h YOUR_HOST
#   DESCRIPTION: Script to copy cert files or any other files to a remote host
#
#============================================================================

usage="$(basename "$0") [-h] [-n cert_file] [-c hostname]
Create a wpa_supplicant.conf:
    -h  show this help text
    -n  any cert or other file
    -c remote host"

options=':h:n:c:'
while getopts $options option; do
  case "$option" in
    h) echo "$usage"; exit;;
    n) cert_file=$OPTARG;;
    c) hostname=$OPTARG;;
    :) printf "missing argument for -%s\n" "$OPTARG" >&2; echo "$usage" >&2; exit 1;;
   \?) printf "illegal option: -%s\n" "$OPTARG" >&2; echo "$usage" >&2; exit 1;;
  esac
done

shift "$(( OPTIND - 1 ))"

if [ ! "$cert_file" ]
then
    echo "-n is a mandatory argument. Please provide a cert file"
    exit 1
fi

if [ ! "$hostname" ]
then
    echo "-c is a mandatory argument. Please provide a hostname"
    exit 1
fi

sftp -oPort=22 pi@${hostname}:/home/pi/build/certs/pki <<EOF
put ${cert_file}
exit
EOF
