#!/bin/bash

# --------------------------------------------------------------------------------------------
# Installs an OPC UA server on the Raspberry Pi (Raspbian)
#
# Basic setup according to http://www.open62541.org/doc/0.3/building.html#building
#
# Run from the web:
#   curl -s https://gist.githubusercontent.com/FHatCSW/59b6364b74795fd900b153654b0f0807/raw/bfd28dc2896b0d4d20b28937795be1681d0a9118/install_opc_server.sh >install.sh
#   bash ./install.sh
# --------------------------------------------------------------------------------------------

# Welcome message
echo -e "
This will install an OPC Server using https://github.com/open62541/open62541.
It will take about 20 minutes to compile on the original Raspberry Pi.\n"

echo -e "
\n*****************************
\n*****************************
\nThere is a manual step in this script (ccmake). You need to enable the UA_EXAMPLE and UA_ENABLE_AMALGAMATION press (c) and afterwards press (g)'
\n*****************************
\n*****************************\n"

# Prompt to continue
read -p "  Continue? (y/n) " ans
if [[ $ans != "y" ]]; then
  echo -e "\nQuitting...\n"
  exit
fi
echo

# Time the install process
START_TIME=$SECONDS

echo -e "\nupdating ..."
sudo apt-get update

echo -e "\nupgrading ..."
sudo apt-get upgrade -y

echo -e "\ninstalling cmake, git, gcc, python ..."
sudo apt-get install git build-essential gcc pkg-config cmake python python-six

echo -e "\ninstalling cmake-curses-gui ..."
sudo apt-get install cmake-curses-gui

echo -e "\ninstalling libmbedtls-dev ..."
sudo apt-get install libmbedtls-dev

echo -e "\ncloning the repository ..."
git clone https://github.com/open62541/open62541.git

echo -e "\nbuilding the server ..."
cd open62541
mkdir build
cd build
cmake ..
echo -e "\n...server build"

echo -e "\nconfigure ccmake"
ccmake .. # cmake ../ tells cmake to prepare the build process in the current subdirectory.

echo -e "\nmake ..."
make # make  executes the generated Makefiles which build the stack

sudo touch /lib/systemd/system/opc.service

sudo bash -c 'echo "[Unit]
Description=OPC UA Server
After=multi-user.target

[Service]
Type=idle
ExecStart=/home/pi/open62541/build/bin/examples/server_inheritance

[Install]
WantedBy=multi-user.target" > /lib/systemd/system/opc.service'

echo -e "\nCreated 'opc.service' Unit file at '/lib/systemd/system/'"

echo -e "\nSetting permissions for opc.service"
sudo chmod 644 /lib/systemd/system/opc.service

echo -e "\nEnabling 'opc.service'...\n"
sudo systemctl daemon-reload
sudo systemctl enable opc.service

# Print the time elapsed
ELAPSED_TIME=$(($SECONDS - $START_TIME))
echo -e "\nFinished in $(($ELAPSED_TIME/60/60)) hr, $(($ELAPSED_TIME/60%60)) min, and $(($ELAPSED_TIME%60)) sec\n"

echo -e "\nAfter reboot check the service via 'systemctl status opc.service'\n"

read -p "  The Pi needs to reboot. Do you want to reboot? (y/n) " ans
if [[ $ans != "n" ]]; then
  echo -e "\nRebooting...\n"
  sudo reboot
  exit
fi
echo