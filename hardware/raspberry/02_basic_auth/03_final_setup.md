# final setup

This documentation describes the final setup of the containers

## InfluxDB

### Setup

- Go to: http://192.168.88.100:8086

![](./img/influxdb_001.png)

- Login with admin:microfoo123

- Go to Data > API Token

- Generate new API Token (Read/Write API Token)

- Name your token (grafana_api_token) and select read access for micro_db

- Copy the API Token to the clipboard

### Query Definition

- Go to Explore and define your Query by selecting the bucket, measurement tag, field tag and topic

- Press Query builder

## Grafana

- Go to: http://192.168.88.100:3000

- Login with admin:microfoo123

- Go to Configuration > Datasources

- Create a new InfluxDB Datasource

- Select the Query Language Flux

- Define the Connection and Press Save & Test


