#!/bin/bash
#============================================================================
#       FILE: p12_to_pem.sh
#       USAGE: ./p12_to_pem.sh
#   DESCRIPTION: Script to generate a private key and certificate from a .p12 file
#
#============================================================================

usage="$(basename "$0") [-h] [-p file_path]
Create a wpa_supplicant.conf:
    -h  show this help text
    -p  file path of the p12 file"

options=':h:p:'
while getopts $options option; do
  case "$option" in
    h) echo "$usage"; exit;;
    p) file_path=$OPTARG;;
    :) printf "missing argument for -%s\n" "$OPTARG" >&2; echo "$usage" >&2; exit 1;;
   \?) printf "illegal option: -%s\n" "$OPTARG" >&2; echo "$usage" >&2; exit 1;;
  esac
done

shift "$(( OPTIND - 1 ))"

if [ ! "$file_path" ]
then
    echo "-p is a mandatory argument. Please provide a .p12 file"
    exit 1
fi

if [ ! -r "$file_path" ]; then
  echo "file \"${file_path}\" not found or couldn't read"
  exit 1;
fi

fullname="${file_path##*/}"
dirname="${file_path%/*}"
basename="${fullname%.*}"
extension="${fullname##*.}"

# If the file is in the same directory with the script,
# path likely will not include any directory seperator.
if [ "$dirname" == "$file_path" ]; then
  dirname="."
fi

# If the file has no extension, exit with error
if [ "$extension" == "$basename" ]; then
  echo "file \"${file_path}\" is not a .p12 file"
fi

echo -e "\nGenerating the certificate out of the .p12 file"
openssl pkcs12 -in ${fullname} -out ${basename}.cert.pem -clcerts -nokeys

echo -e "\nGenerating the private key out of the .p12 file"
openssl pkcs12 -in ${fullname} -out ${basename}.key.pem -nocerts -nodes