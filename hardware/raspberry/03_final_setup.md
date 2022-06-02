# final setup

This documentation describes the final setup of the containers

## InfluxDB

### Setup

- Go to: http://192.168.88.100:8086

![](/img/influxdb_001.png)

- Login with admin:microfoo123

- Go to Data > API Token

![](/img/influxdb_003.png)

- Generate new API Token (Read/Write API Token)

- Name your token (grafana_api_token) and select read access for micro_db

![](/img/influxdb_004.png)

- Copy the API Token to the clipboard

![](/img/influxdb_005.png)


### Query Definition

- Go to Explore and define your Query by selecting the bucket, measurement tag, field tag and topic

![](/img/influxdb_006.png)

- Press Query builder

![](/img/influxdb_007.png)


## Grafana

- Go to: http://192.168.88.100:3000

![](/img/grafana_001.png)

- Login with admin:microfoo123

- Go to Configuration > Datasources

![](/img/grafana_004.png)

- Create a new InfluxDB Datasource

- Select the Query Language Flux

- Define the Connection and Press Save & Test

![](/img/grafana_005.png)

# OPC UA

- To acces the OPC UA Web Client go to: http://192.168.88.100:8000

- Define the Server adress: opc.tcp://192.168.88.101:4840/freeopcua/server/

- Press connect

![](/img/opc_001.png)



