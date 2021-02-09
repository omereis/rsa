# Using the latest long-term-support Ubuntu OS
FROM ubuntu:16.04

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y vim man
RUN apt install -y tree curl
RUN apt install -y git wget cmake

RUN apt install -y g++ sdb libjsoncpp-dev gdb
RUN apt install -y iputils-ping

RUN git clone https://github.com/99x/timercpp /home/oe/timercpp

ENV HOME=/home/oe
ENV CPLUS_INCLUDE_PATH=/usr/include;/home/oe/timercpp

WORKDIR /home/oe

COPY ./ $HOME

RUN mv vimrc .vimrc

#RUN cd $HOME &&\
#		g++ -g -o server -ljsoncpp server.cpp

# EXPOSE 6000
