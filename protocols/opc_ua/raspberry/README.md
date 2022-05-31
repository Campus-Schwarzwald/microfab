
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
    sudo apt-get upgrade
    ```
- Install Git


    sudo apt install git

- Install cmake and ccmake


    sudo apt-get install build-essential cmake
    sudo apt-get install cmake-curses-gui

- Download the repo


    git clone https://github.com/open62541/open62541.git

- Build the server


    cd open62541 
    mkdir build 
    cd build 
    cmake .. 

- Configure cmake:

    
    ccmake .. 

    enable the UA_BUILD_EXAMPLES option, then configure(c) and generate(g) the makefile

- Make


    make 
    cd bin/examples 
    ./tutorial_server_firststeps

server is now running on the Raspberry Pi and will be listening in the port 4840