FROM gcc

RUN apt-get update; \
  apt-get install cmake -y; \
  mkdir -p /root/samplesys/build;

COPY . /root/samplesys

WORKDIR /root/samplesys/build

RUN cmake ..; \
  make;