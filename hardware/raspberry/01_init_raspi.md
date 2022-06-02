This document explains step by step the initial setup and configuration of the RaspberryPi


- Install Raspberry Pi OS Lite with Raspberry Pi Imager
    - Hostname: MicrofabControl001 ... MicrofabControl006
    - Activate SSH
    - Set User: pi
    - Set password: microfoo123
    - Set WiFi network:
      - SSID: Microfab_001 ...Microfab_006
      - PIN:
- Login via ssh
    ``` bash
    ssh pi@MicrofabControl_001.local
    ```
      
- Configure WiFi:
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
    sudo apt-get upgrade -y
    ```
- Install Git
    ``` bash
    sudo apt install git -y
    ```
- Clone the Repo
    ``` bash
    git clone https://github.com/Campus-Schwarzwald/microfab.git
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