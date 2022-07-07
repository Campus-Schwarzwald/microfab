#!/bin/bash

# --------------------------------------------------------------------------------------------
# Installs an OPC UA server on the Raspberry Pi (Raspbian)
#
# Run from the web:
#   bash <(curl -s https://gist.githubusercontent.com/blacktm/8302741/raw/install_ruby_rpi.sh)
# --------------------------------------------------------------------------------------------

# Welcome message
echo -e "
This will install an OPC Server using https://github.com/open62541/open62541.
It will take about 20 minutes to compile on the original Raspberry Pi.\n"

# Prompt to continue
read -p "  Continue? (y/n) " ans
if [[ $ans != "y" ]]; then
  echo -e "\nQuitting...\n"
  exit
fi
echo

echo -e "\nupdating ..."
sudo apt-get update

echo -e "\nupgrading ..."
sudo apt-get upgrade -y

echo -e "\ninstalling git ..."
sudo apt install git -y

echo -e "\ninstalling cmake ..."
sudo apt-get install build-essential cmake -y

echo -e "\ninstalling cmake-curses-gui ..."
sudo apt-get install cmake-curses-gui

echo -e "\ncloning the repository ..."
git clone https://github.com/open62541/open62541.git

echo -e "\nbuilding the server ..."
cd open62541
mkdir build
cd build
cmake ..
echo -e "\n...server build"

echo -e "\nconfigure ccmake"
ccmake ..

echo -e "\nmake ..."
make

sudo touch /lib/systemd/system/opc.service

echo -e "[Unit]
Description=OPC UA Server
After=multi-user.target

[Service]
Type=idle
ExecStart=/home/pi/open62541/build/bin/examples/server_inheritance

[Install]
WantedBy=multi-user.target" > /lib/systemd/system/opc.service

echo -e "\nCreated 'opc.service' Unit file at '/lib/systemd/system/'"

echo -e "\nSetting permissions for opc.service"
sudo chmod 644 /lib/systemd/system/opc.service

echo -e "\nEnabling 'opc.service'...\n"
sudo systemctl daemon-reload
sudo systemctl enable opc.service

# Print the time elapsed
ELAPSED_TIME=$(($SECONDS - $START_TIME))
echo -e "\nFinished in $(($ELAPSED_TIME/60/60)) hr, $(($ELAPSED_TIME/60%60)) min, and $(($ELAPSED_TIME%60)) sec\n"

read -p "  The Pi needs to reboot. Do you want to reboot? (y/n) " ans
if [[ $ans != "n" ]]; then
  echo -e "\nRebooting...\n"
  sudo reboot
  exit
fi
echo