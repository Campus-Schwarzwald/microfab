
  
- Configure Docker:
  
    - Create folders:
        ``` bash
        sudo mkdir -p $HOME/docker
        sudo mkdir -p $HOME/docker/grafana
        sudo mkdir -p $HOME/docker/mosquitto
        sudo mkdir -p $HOME/docker/telegraf
        sudo mkdir -p $HOME/docker/influxdb
        ```
    - [hint](https://blog.anoff.io/2020-12-run-influx-on-raspi-docker-compose/)
        ``` bash
        docker run --rm arm32v7/influxdb influxd config > influxdb.conf
        ```
    - Create Mosquitto config [bugfix](https://it-obey.com/index.php/connecting-telegraf-to-mosquitto-with-influxdb/)
        - Create mosquitto.conf
            ``` bash
            sudo nano $HOME/docker/mosquitto/mosquitto.conf
            ```
        - Insert code
            ``` bash
            persistence true
            persistence_location /mosquitto/data/
            log_dest file /mosquitto/log/mosquitto.log
            listener 1883
            allow_anonymous true
            ```
      
    - Docker compose configuration
        - Create docker-compose.yml
  
            ``` bash
            sudo nano $HOME/docker/docker-compose.yml
            ```
  
        - Add config
  
            ``` bash
            version: '3'
            services:
              mosquitto:
                image: 'eclipse-mosquitto:latest'
                container_name: mosquitto
                ports:
                  - '1883:1883'
                volumes:
                  - /home/pi/docker/mosquitto/mosquitto.conf:/mosquitto/config/mosquitto.conf
                  - /home/pi/docker/mosquitto/data/:/mosquitto/data/
                  - /home/pi/docker/mosquitto/log/:/mosquitto/log/
            
              influxdb:
                image: arm32v7/influxdb:latest
                ports:
                  - '8086:8086'
                volumes:
                  - 'vol_influxdb:/var/lib/influxdb'
                environment:
                  - INFLUXDB_DB=telegraf
                  - INFLUXDB_ADMIN_USER=admin
                  - INFLUXDB_ADMIN_PASSWORD=admin
            
              telegraf:
                image: 'telegraf:latest'
                volumes:
                  - '/home/pi/docker/telegraf/telegraf.conf:/etc/telegraf/telegraf.conf'
                environment:
                  - INFLUXDB_ADMIN_USER=admin
                  - INFLUXDB_ADMIN_PASSWORD=admin
                depends_on:
                  - influxdb
                links:
                  - influxdb
                ports:
                  - '8125:8125'
          
              grafana:
                image: 'grafana/grafana:latest'
                ports:
                  - '3000:3000'
                volumes:
                  - '/home/pi/docker/grafana/grafana.ini:/etc/grafana/grafana.ini'
                  - 'vol_grafana_var:/var/lib/grafana'
                  - 'vol_grafana_etc:/etc/grafana/'
                depends_on:
                  - influxdb
                environment:
                  - GF_SECURITY_ADMIN_USER=admin
                  - GF_SECURITY_ADMIN_PASSWORD=admin
            
              iobroker:
                image: 'buanet/iobroker:latest'
                ports:
                  - '8081:8081'
                  - '8082:8082'
                volumes:
                  - 'vol_iobroker:/opt/iobroker'
            
            volumes:
              vol_influxdb:
              vol_telegraf:
              vol_grafana_var:
              vol_grafana_etc:
              vol_iobroker:
            ```
    
    - Other usefull Docker commands:
        - Get latest logs: ``` docker container logs -f --since 30m telegraf```
        - 

    - Create Telegraf config:
        ``` bash
        cd $HOME/docker
        sudo docker run --rm telegraf telegraf config > ./telegraf/telegraf.conf
        ```
        - Change telegraf.conf
            ``` bash
            [[outputs.influxdb]]

              urls = ["http://influxdb:8086"]
            
              database = "telegraf"
            
            [[inputs.mqtt_consumer]]
              servers = ["tcp://mosquitto:1883"]
            
              topics = [
              "test/#"
              ] 
            
              data_format = "json"
            ```
    - Create Grafana config:
        ``` bash
        docker run --rm --entrypoint /bin/bash grafana/grafana:latest -c 'cat $GF_PATHS_CONFIG' > grafana.ini
        sudo docker cp grafana:/etc/grafana/grafana.ini ./grafana/
        ```
    - Start containers:
        ``` bash
        sudo docker-compose up -d
        ```
    - See active containers:
        ``` bash
        sudo docker ps
        ```
    - Test MQTT
        - Install paho-mqtt
            ``` bash
            pip3 install paho-mqtt
            ```
        - Create python script
            ``` bash
            nano ping.py
            ```
        - Add code:
            ``` bash
            import subprocess
            import re
            import paho.mqtt.client as mqtt
            import json
            import time
              
            
            client = mqtt.Client()
            client.connect("localhost", 1883, 60)
              
            data = {}
            
            while True:
                for host in ["192.168.88.1"]:
                    try:
                        x = str(subprocess.Popen(
                            ["ping", "-c", "1", host], stdout=subprocess.PIPE).communicate()[0])
                        m = re.search(
                            "\d* bytes from (.*?): icmp_seq=\d* ttl=\d* time=(\d*\.?\d*) ms", x)
                        data[host] = {
                            "addr": m.group(1),
                            "duration": float(m.group(2))
                        }
                    except:
                        pass
              
                print(data)
                client.publish(topic="test/ping", payload=json.dumps(data))
                time.sleep(10)
            ```
          
    - Restart Docker
        ``` bash
        sudo docker-compose down
        sudo docker-compose up -d
        ```
    - Start Python ping.py
        ``` bash
        python ping.py &
        ```
        - Stop it :)
            ```bash
            # List processes
            ps
            # kill process
            sudo kill -9 <pid>
            ```
      
- Configure Grafana
    - Add a database
        - Go to ```http://192.168.88.100:3000/login```
        - Login with ```admin:admin``
        - Change password
        - Go to Configuration -> Data Sources
        - Add Data Source -> InfluxDB
        - Define Server URL: http://influxdb:8086
        - Define Database (InfluxDb Details)
        - Safe & test
    - Create a dashboard
        - Go to Create -> Dashboard -> Add new panel
        - Select Data source: InfluxDB
        - FROM autogen mqtt_consumer WHERE topic = test/ping SELECT field(192.168.88.1_duration)
    
    
Sources:
- https://www.eydam-prototyping.com/2021/01/09/smart-home-zentrale-mit-dem-raspberry-pi/