# Usefull commands

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

rebuild an existing conatiner

    docker-compose -f docker-compose<...> up --build opc-client

Show all running containers

    docker-compose -f docker-compose.pki.yml  ps --services --filter "status=running"

