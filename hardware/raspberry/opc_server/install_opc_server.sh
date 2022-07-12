#!/bin/bash

# --------------------------------------------------------------------------------------------
# Installs an OPC UA server on the Raspberry Pi (Raspbian)
#
# The server is Python based (asyncua)
#
# Run from the web:
#   curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/opc_server/install_opc_server.sh >install.sh
#   bash ./install.sh
# --------------------------------------------------------------------------------------------

# Welcome message
echo -e "
This will install an OPC Server using https://github.com/FreeOpcUa/opcua-asyncio.
It will take about 10 minutes to compile on the original Raspberry Pi.\n"


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

echo -e "\ninstalling git, python ..."
sudo apt-get install git python3 python-is-python3 pip -y

echo -e "\ninstall requirements ..."
sudo python pip install asyncua

echo -e "\nused python:"
python -c "import sys; print(sys.path)"

echo -e "\nsetting a static ip..."
sudo bash -c 'echo "
interface wlan0
static ip_address=192.168.88.101/24
static routers=192.168.88.1

allow-hotplug wlan0
iface wlan0 inet manual
wireless-power off
" >> /etc/dhcpcd.conf'

mkdir build
cd build

curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/opc_client/01_no_auth/01_microfab_opcua_server_no_auth.py >01_microfab_opcua_server_no_auth.py

sudo touch /lib/systemd/system/opc.service

sudo bash -c 'echo "[Unit]
Description=OPC UA Server
After=multi-user.target

[Service]
Type=idle
ExecStart=/usr/bin/python3 /home/pi/build/01_microfab_opcua_server_no_auth.py

[Install]
WantedBy=multi-user.target" > /lib/systemd/system/opc.service'

echo -e "\nCreated 'opc.service' Unit file at '/lib/systemd/system/'"

echo -e "\nSetting permissions for opc.service"
sudo chmod 644 /lib/systemd/system/opc.service
chmod +x /home/pi/build/01_microfab_opcua_server_no_auth.py

echo -e "\nEnabling 'opc.service'...\n"
sudo systemctl daemon-reload
sudo systemctl enable opc.service
sudo systemctl start opc.service

echo -e "
---Changes on /lib/systemd/system/opc.service---
sudo systemctl daemon-reload

---Check status---
sudo systemctl status hello.service

---Start service---
sudo systemctl start hello.service

---Stop service---
sudo systemctl stop hello.service

---Check service log---
sudo journalctl -f -u hello.service\n"

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