FROM ubuntu:20.04

RUN apt-get update && apt-get install -y \
    make \
    libasan5 \
    libubsan1 \
    ghostscript

WORKDIR /usr/src/app

COPY . .

RUN make all