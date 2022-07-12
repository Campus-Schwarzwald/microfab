#!/bin/bash

# --------------------------------------------------------------------------------------------
# Prepares the Raspberry Pi for further setup
#
# Run from the web:
#   curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/initial_setup_control.sh >install.sh
#   bash ./install.sh
# --------------------------------------------------------------------------------------------

# Welcome message
echo -e "
This will prepare the Raspberry Pi for further installations.
It will take about 20 minutes to compile on the Raspberry Pi.\n"

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

echo -e "\ninstalling git ..."
sudo apt install git -y

echo -e "\ncloning the repository ..."
git clone https://github.com/Campus-Schwarzwald/microfab.git

echo -e "\ninstalling docker ..."
sudo curl -fsSL https://get.docker.com | sh

echo -e "\nadding the pi user to docker ..."
sudo usermod -aG docker pi

echo -e "\nOutput pi user permissions:"
groups ${USER}

echo -e "\ninstalling python 3 ..."
sudo apt-get install -y python3-pip

echo -e "\ninstalling docker-compose..."
sudo pip3 install docker-compose
docker-compose version

echo -e "\nsetting a static ip..."
sudo bash -c 'echo "
interface wlan0
static ip_address=192.168.88.100/24
static routers=192.168.88.1

allow-hotplug wlan0
iface wlan0 inet manual
wireless-power off
" >> /etc/dhcpcd.conf'

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