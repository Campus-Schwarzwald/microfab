
> 07-07-2022 - Florian Handke: There seems to be an issue with open62541 and the Raspberry. 
I opened an issue on Github: https://github.com/open62541/open62541/issues/5245
> 
> If there is progress on this, I will come back to open62541

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
            static ip_address=192.168.88.101/24
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


    sudo apt install git -y

- Install cmake and ccmake


    sudo apt-get install build-essential cmake -y
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

    enable the UA_EXAMPLE option, then configure(c) and generate(g) the makefile

- Make


    make 
    cd bin/examples 

- Test the Server


    ./server_inheritance

server is now running on the Raspberry Pi and will be listening in the port 4840

# Create service on startup

- Create new service


    sudo nano /lib/systemd/system/opc.service

- Insert configuration


     [Unit]
     Description=OPC UA Server
     After=multi-user.target
    
     [Service]
     Type=idle
     ExecStart=/home/pi/open62541/build/bin/examples/server_inheritance
    
     [Install]
     WantedBy=multi-user.target

- Set permissions


    sudo chmod 644 /lib/systemd/system/opc.service

- Configure systemd


    sudo systemctl daemon-reload
    sudo systemctl enable opc.service

- Reboot the device


    sudo reboot