# base-image for python on any machine using a template variable,
# see more about dockerfile templates here: https://www.balena.io/docs/learn/develop/dockerfile/
FROM python:3.9-slim-bullseye

# use apt-get if you need to install dependencies,
# for instance if you need ALSA sound utils, just uncomment the lines below.
# RUN apt-get update && apt-get install -yq \
#    alsa-utils libasound2-dev && \
#    apt-get clean && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -yq stress wget

# Set our working directory
WORKDIR /usr/src/app

# Copy requirements.txt first for better cache on later pushes
COPY ./requirements.txt /requirements.txt

# pip install python deps from requirements.txt on the resin.io build server
#RUN pip install local_packages/tp_manager_backend-1.0.4-py3-none-any.whl

RUN pip install -r /requirements.txt

# This will copy all files in our root to the working  directory in the container
COPY . ./

ENV PYTHONPATH /usr/src/app
# switch on systemd init system in container
ENV INITSYSTEM on

# main.py will run when container starts up on the device
CMD ["python","-u","src/app.py"]