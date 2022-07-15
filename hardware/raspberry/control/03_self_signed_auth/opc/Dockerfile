FROM python:3.8

RUN apt-get update && apt-get install -yq stress wget

RUN apt install libxml2 -y
RUN apt-get install libxslt-dev -y
RUN apt-get install python-dev -y
RUN apt-get install gcc -y
RUN apt-get install zlib1g-dev

# Set our working directory
RUN mkdir /app
RUN mkdir /app/certs
WORKDIR /app

# Copy requirements.txt first for better cache on later pushes
COPY requirements.txt /requirements.txt

RUN pip install -r /requirements.txt

# This will copy all files in our root to the working  directory in the container
COPY . .

# opc-client.py will run when container starts up on the device
CMD ["python","-u","opc-client.py"]