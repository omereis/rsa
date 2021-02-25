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

# RUN git clone https://github.com/99x/timercpp /home/oe/timercpp

ENV HOME=/home/oe

RUN git clone https://github.com/open-source-parsers/rapidjson /home/oe/rapidjson
RUN cp -r /home/oe/rapidjson/include/* /usr/include

ENV CPLUS_INCLUDE_PATH=/usr/include;/home/oe/rapidjson/include
ENV C_INCLUDE_PATH=/usr/include:/home/oe/rapidjson/include

WORKDIR /home/oe

# RUN mkdir -p /home/oe/vcpkg && \
# 	git clone https://github.com/Microsoft/vcpkg /home/oe/vcpkg &&\
# 	/home/oe/vcpkg/bootstrap-vcpkg.sh

COPY ./ $HOME

RUN mv vimrc .vimrc
RUN mkdir -p /usr/include/redpitaya/include/
RUN cp /home/oe/rp.h /usr/include/redpitaya/include/

RUN mkdir -p /opt/redpitaya/include/
RUN cp /home/oe/rp.h /opt/redpitaya/include/

ENV CPLUS_INCLUDE_PATH = $CPLUS_INCLUDE_PATH:/opt
ENV C_INCLUDE_PATH = $C_INCLUDE_PATH:/opt

RUN ln -s $HOME/rp_signal.dat $HOME/signal.dat

#RUN cd $HOME &&\
#	make

EXPOSE 5500
EXPOSE 5501
