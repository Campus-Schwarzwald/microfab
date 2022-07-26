# Network Analysis

## Find machines in the same network

### nmap

1. Install nmap

    on Mac:
    ```bash
    brew install nmap
    ```
   
2. Analyse a single host:

    ```bash
    nmap 192.168.88.100 -Pn
   
    # -Pn is optional if the host is blocking your ping probes
    ```
   
3. Analyse your network:

    ```bash
    sudo nmap 192.168.88.0/24 -oG nmap_output
   
    #somehow only sudo is shoing all hosts
    ```
   
   For our microfab we should then get the following output:

    ```bash
    Starting Nmap 7.92 ( https://nmap.org ) at 2022-07-26 14:05 CEST
   Nmap scan report for 192.168.88.1
   Host is up (0.0087s latency).
   Not shown: 996 closed tcp ports (reset)
   PORT     STATE SERVICE
   22/tcp   open  ssh
   53/tcp   open  domain
   80/tcp   open  http
   1900/tcp open  upnp
   MAC Address: 00:5F:67:A1:E5:1C (TP-Link Limited)
   
   Nmap scan report for 192.168.88.100
   Host is up (0.0098s latency).
   Not shown: 999 closed tcp ports (reset)
   PORT   STATE SERVICE
   22/tcp open  ssh
   MAC Address: DC:A6:32:F7:62:D2 (Raspberry Pi Trading)
   
   Nmap scan report for 192.168.88.101
   Host is up (0.012s latency).
   Not shown: 999 closed tcp ports (reset)
   PORT   STATE SERVICE
   22/tcp open  ssh
   MAC Address: B8:27:EB:22:08:3D (Raspberry Pi Foundation)
   
   Nmap scan report for 192.168.88.102
   Host is up (0.11s latency).
   All 1000 scanned ports on 192.168.88.102 are in ignored states.
   Not shown: 1000 closed tcp ports (reset)
   MAC Address: 94:B9:7E:D6:0D:48 (Espressif)
    ```

4. Getting hte OS & services

    ```bash
    nmap -A 192.168.88.100 
   
   Nmap scan report for 192.168.88.100
   Host is up (0.0051s latency).
   Not shown: 999 closed tcp ports (conn-refused)
   PORT   STATE SERVICE VERSION
   22/tcp open  ssh     OpenSSH 8.4p1 Debian 5+deb11u1 (protocol 2.0)
   | ssh-hostkey: 
   |   3072 2c:d6:04:d8:ef:42:78:8f:93:cd:76:aa:ad:cf:66:c3 (RSA)
   |   256 52:d1:22:08:c3:d5:d9:59:80:e6:78:ec:cb:6c:de:9b (ECDSA)
   |_  256 3f:a3:18:f2:7d:ea:88:7d:40:7a:38:bd:02:19:78:91 (ED25519)
   Service Info: OS: Linux; CPE: cpe:/o:linux:linux_kernel
    ```
   

### ping

If you know the IP of the machine just ping it and you see if it is visible:
```bash
ping 192.168.88.100
```

## Capture traffic

### tcpdump

   > List all possible options with `tcpdump --help`

1. To capture traffic on a remote host you first need to connect via SSH

   ```bash
   ssh pi@MicrofabControl002.local
   ```

2. Choose an interface for your analysis. Therfore list all network interfaces with `ifconfig` (Linux)

3. Capture traffic from a defined interface (wlan0) and save it to a file:

   ```bash
   sudo tcpdump -s 0 -i wlan0 -w tcpdump.pcap
   
   # -s snaplen --> sets an unlimited packet length
   # -i interface
   # -w file --> write traffic to a Wireshark readable file
   ```
   
4. After pressing `Ctrl + C` the capture will stop

5. To copy the file to your local machine you need to adjust the permissions:

   ```bash
   sudo chmod 644 tcpdump.pcap
   ```
   
6. Copy the files t your local machine

   ```bash
   scp pi@MicrofabControl005.local:/home/pi/tcpdump.pcap ./
   ```
7. For analysing the traffic download `Wireshark` and open your file



