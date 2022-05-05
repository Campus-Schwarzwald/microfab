# OPCUA-Sample
Sample implementation of a OPC-UA Server &amp; Client running in Docker on two Raspberry Pi 4B

# Prepare Raspberries
## Server

+ Create a Raspberry Pi with the latest Version of Raspberry Pi OS Lite (32 bit)
  + Set Hostname to **opcua-server**
  + Enable **SSH**
  + Set user:password (admin:foo123)
  + Set WiFi

## Client

+ Create a Raspberry Pi with the latest Version of Raspberry Pi OS Lite (32 bit)
  + Set Hostname to **opcua-client**
  + Enable **SSH**
  + Set user:password (admin:foo123)
  + Set WiFi

## Setup on Pi

### General Setup Raspberry

- Update & Upgrade
    ```bash
    sudo apt-get update
    sudo apt-get dist-upgrade
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
        sudo usermod -aG docker admin
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
  
- Clone repository

    ``` bash
    git clone https://github.com/FHatCSW/OPCUA-Sample.git
    ```
  
- Reboot Raspberry

    ``` bash
    sudo reboot
    ```
  
### Client

Got to directory

    cd /home/admin/OPCUA-Sample/opcua-client

Start docker container

    docker-compose up -d

### Server

Got to directory

    cd /home/admin/OPCUA-Sample/opcua-server

Start docker container

    sudo docker-compose up -d

# Git

## Useful commands

Stash all local changes on the Raspberry

    git stash push --include-untracked

# Docker

## Useful commands

List all docker containers:

    docker container ls -s

find all running docker processes
    
    ps aux | grep docker

... or a certainer container

    docker ps -a | grep <certain_container>

Shutdown a certain docker compose process:

    docker-compose -f docker-compose<...>.yml down

remove a conatiner

    docker rm -f influxdb

# Code

## Client

## Server

https://github.com/FreeOpcUa/python-opcua/blob/master/examples/server-minimal.py


