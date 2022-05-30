# Stop and restart a docker container

- Got to the folder where the actual docker container runs

- Stop the actual docker-compose process


    docker-compose -f docker-compose<...>.yml down

- ... or find all running docker processes and remove them


    ps aux | grep docker
    docker rm -f influxdb

- Change your directory and start docker-compose

