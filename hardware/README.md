# How to get started

To get started all hardware has a bais level of setup. 
Follow the following instructions to establish this basic setup

## Setting up the hardware

### Microfab Control

Microfab Control is the central unit of the Microfab. It uses a dockerized techstack with Grafana, InfluxDB, Telegraf, Mosquitto and an OPC UA Client and can be extended by any other compatible Docker container. For this setup we are using Docker Compose

**Hardware**: Raspberry Pi 4B

**OS**: Raspian headless

- Install Raspberry Pi OS Lite with Raspberry Pi Imager
    - Hostname: MicrofabControl001 ... MicrofabControl006
    - Activate SSH
    - Set User: pi
    - Set password: microfoo123
    - Set WiFi network:
      - SSID: Microfab_001 ...Microfab_006
      - PIN: find in microfab/hardware/router/setup_router.md
      

- Put the MicroSD card in the Pi


- Login via ssh
    ``` bash
    ssh pi@MicrofabControl_001.local
    ```

- Download the script for the setup
    ``` bash
    curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/initial_setup_control.sh >install.sh
    ```
  
- Execute the script
    ``` bash
    bash ./install.sh
    ```
  
- The Pi has now the basic setup and you can go with the individual setup which can be one of the following:

  - **No Authenitcation**: microfab/hardware/raspberry/01_no_auth/README.md

  - **Password Authenitcation**: microfab/hardware/raspberry/02_basic_auth/README.md

  - **Authenitcation with self signed certificates**: microfab/hardware/raspberry/03_self_signed_auth/README.md

  - **Authenitcation with PKI issued certificates**: microfab/hardware/raspberry/04_pki_auth/README.md

### Microfab OPC Client

**Hardware**: Raspberry Zero W1

**OS**: Raspian headless

- Install Raspberry Pi OS Lite with Raspberry Pi Imager
    - Hostname: MicrofabOPCClient001 ... MicrofabOPCClient006
    - Activate SSH
    - Set User: pi
    - Set password: microfoo123
    - Set WiFi network:
      - SSID: Microfab_001 ...Microfab_006
      - PIN: find in microfab/hardware/router/setup_router.md
      

- Put the MicroSD card in the Pi


- Login via ssh
    ``` bash
    ssh pi@MicrofabOPCClient001.local
    ```

- Download the script for the setup
    ``` bash
    curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/initial_setup_control.sh >install.sh
    ```
  
- Execute the script
    ``` bash
    bash ./install.sh
    ```

