FROM python:3.8

RUN apt-get update && apt-get install -yq stress wget

RUN apt install libxml2 -y
RUN apt-get install libxslt-dev -y
RUN apt-get install python-dev -y
RUN apt-get install gcc -y
RUN apt-get install zlib1g-dev

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