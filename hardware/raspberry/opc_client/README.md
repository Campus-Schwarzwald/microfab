# OPC UA Server

There are different implementations of the OPC UA Server. If you already did the basic setup of the Pi via 
microfab/hardware/raspberry/opc_server/install_opc_server.sh the version with no authentication is implemented.

## OPC UA client

To verify that everything works as expected it is useful to have an OPC UA Client on your local machine. there are several implmentation like:

- [Matrikon OPC UA Explorer](https://www.matrikonopc.com/opc-ua/products/opc-ua-explorer.aspx)
- [Unified Automation UaExpert](https://www.unified-automation.com/de/downloads/opc-ua-clients/uaexpert.html)
- [Python based Client](https://github.com/FreeOpcUa/opcua-client-gui)

## Setup

- Login to the Pi via SSH 

- Change directory

    ``` bash
    cd /home/pi/asyncua/build
    ```
  
- Choose one of the following implementations:
  - No authentication

      ``` bash
      curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/opc_client/02_basic_auth/01_microfab_opcua_server_basic_auth.py >01_microfab_opcua_server_basic_auth.py
      ```
  - Password authentication
  
      ``` bash
      curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/opc_client/02_basic_auth/02_microfab_opcua_server_basic_auth.py >02_microfab_opcua_server_basic_auth.py
      ```
  - Self signed certificates authentication 
  
      ``` bash
      curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/opc_client/03_self_signed_auth/03_microfab_opcua_server_self_signed_auth.py >03_microfab_opcua_server_self_signed_auth.py
      ```
  - PKI issued certificates authentication 
  
      ``` bash
      curl -s https://raw.githubusercontent.com/Campus-Schwarzwald/microfab/main/hardware/raspberry/opc_client/03_self_signed_auth/03_microfab_opcua_server_self_signed_auth.py >03_microfab_opcua_server_self_signed_auth.py
      ```


- Create a service (Change the script in Service > ExecStart)
    ``` bash
    sudo touch /lib/systemd/system/opc.service

    sudo bash -c 'echo "[Unit]
    Description=OPC UA Server
    After=multi-user.target
    
    [Service]
    Type=idle
    ExecStart=/usr/bin/python3 /home/pi/opcua-asyncio/build/01_microfab_opcua_server_no_auth.py
    
    [Install]
    WantedBy=multi-user.target" > /lib/systemd/system/opc.service'
    ```

- Set permissions for the server (Change the script name according to your implementation)
    ``` bash
    sudo chmod 644 /lib/systemd/system/opc.service
    chmod +x /home/pi/opcua-asyncio/build/01_microfab_opcua_server_no_auth.py
    ```

- Reload, enable and start the service
    ``` bash
    sudo systemctl daemon-reload
    sudo systemctl enable opc.service
    sudo systemctl start opc.service
    ```

