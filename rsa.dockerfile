# Using the latest long-term-support Ubuntu OS
FROM ubuntu:16.04

RUN apt -y update
RUN apt -y upgrade
RUN apt install -y vim man
RUN apt install -y tree curl
RUN apt install -y git wget cmake

RUN apt install -y g++ sdb libjsoncpp-dev gdb
RUN apt install -y iputils-ping
RUN apt install -y curl zip unzip tar

RUN git clone https://github.com/99x/timercpp /home/oe/timercpp

ENV HOME=/home/oe

RUN git clone https://github.com/open-source-parsers/rapidjson /home/oe/rapidjson
RUN cp -r /home/oe/rapidjson/include/* /usr/include

# Installing g++ 7
# RUN apt-get install -y software-properties-common
# RUN add-apt-repository ppa:ubuntu-toolchain-r/test
# RUN apt update -y
# RUN apt-get install -y gcc g++ gcc-5 g++-5 gcc-6 g++-6 gcc-7 g++-7
# RUN update-alternatives --remove-all gcc
# RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-5 90 --slave /usr/bin/g++ g++ /usr/bin/g++-5
# RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 80 --slave /usr/bin/g++ g++ /usr/bin/g++-6
# RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-7 70 --slave /usr/bin/g++ g++ /usr/bin/g++-7
# RUN update-alternatives --config gcc

ENV CPLUS_INCLUDE_PATH=/usr/include;/home/oe/rapidjson/include
ENV C_INCLUDE_PATH=/usr/include:/home/oe/rapidjson/include

WORKDIR /home/oe

# RUN mkdir -p /home/oe/vcpkg && \
# 	git clone https://github.com/Microsoft/vcpkg /home/oe/vcpkg &&\
# 	/home/oe/vcpkg/bootstrap-vcpkg.sh

COPY ./ $HOME

RUN mv vimrc .vimrc

#RUN cd $HOME &&\
#		g++ -g -o server -ljsoncpp server.cpp

# EXPOSE 6000
