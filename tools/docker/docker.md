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