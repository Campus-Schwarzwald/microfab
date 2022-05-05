This document explains step by step the initial setup and configuration of the RaspberryPi


- Install Raspberry Pi OS Lite
    - goto boot device
        - add new file: ssh
    - insert SD card in raspberry -> Power up
        - login: ssh pi@raspberrypi.local
        - pw: raspberry
    - Unblock WiFi from rfkill (Check with rfkill list)
        ``` bash
        rfkill unblock 0
        ```
    - Change Hostname
        ``` bash
        sudo raspi-config
        ```
        - Hostname -> MikrofabControl -> OK
        ``` bash
        sudo reboot
        ```
    - Login via ssh
        ``` bash
        ssh pi@MikrofabControl.local
        ```
      
- Configure WiFi:
    - Open config file:
        ```bash
        sudo nano /etc/wpa_supplicant/wpa_supplicant.conf
        ```
    - Add following lines:
        ```bash
        ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
        update_config=1
        country=DE
      
        network={
            ssid="Mikrofab_01"
            psk="WLAN-PASSWORD"
        }
        ```
    - Add static IP
        - Open config file
            ``` bash
            sudo nano /etc/dhcpcd.conf 
            ```   
        - Set static IP
            ```bash
            interface wlan0
            static ip_address=192.168.88.100/24
            static routers=192.168.88.1 
            ```
        - Deactivate energy save mode
            ``` bash
            allow-hotplug wlan0
            iface wlan0 inet manual
            wireless-power off
            ```
    - Reboot
        ``` bash
        sudo reboot
        ```
- Update & Upgrade
    ```bash
    sudo apt-get update
    sudo apt-get upgrade
    ```
- Install Git
    ``` bash
    sudo apt install git
    ```
- Install Docker
    ```bash
    sudo curl -fsSL https://get.docker.com | sh
    ```
- Add a Non-Root User to the Docker Group
    - Add users to the Docker group
        ```bash
        sudo usermod -aG docker pi
        ```
    - Check if running
        ```bash
        groups ${USER}
        ```
      
- Install Docker-Compose
    ``` bash
    sudo apt-get install -y python3-pip
    sudo pip3 install docker-compose
    docker-compose version
    ```
- Reboot Raspberry

    ``` bash
    sudo reboot
    ```