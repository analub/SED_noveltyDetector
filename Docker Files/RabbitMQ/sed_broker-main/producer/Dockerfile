FROM gcc:latest

ENV DST_MAC="01:0c:cd:04:00:02"
ENV BATCH_SIZE=3

RUN apt update && update-alternatives --install /usr/bin/gfortran gfortran /usr/local/bin/gfortran 999
RUN DEBIAN_FRONTEND=noninteractive apt-get install -yq cmake tshark libssl-dev libboost-all-dev libpcap-dev

# PcapPlusPlus
RUN mkdir to-install && cd to-install &&  wget https://github.com/seladb/PcapPlusPlus/releases/download/v21.11/pcapplusplus-21.11-source-linux.tar.gz && tar -vxzf pcapplusplus*
RUN cd to-install/pcapplusplus-21.11-source-linux && set -e && cd PcapPlusPlus && ./configure-linux.sh --default && make libs && make install

# RabbitMQ
RUN wget https://github.com/alanxz/rabbitmq-c/archive/refs/tags/v0.13.0.tar.gz && tar -zxvf v0.13.0.tar.gz
RUN cd rabbitmq-c-0.13.0 && mkdir build && cd build && cmake -DBUILD_STATIC_LIBS=ON  ..  && cmake --build . --target install && rm -rf rabbitmq-c-0.13.0
RUN wget https://github.com/alanxz/SimpleAmqpClient/archive/v2.4.0.tar.gz && tar -zxvf v2.4.0.tar.gz
RUN cd SimpleAmqpClient-2.4.0 && mkdir build && cd build && cmake ..  && cmake --build . --target install && rm -rf  SimpleAmqpClient-2.4.0

# JSON
RUN wget https://github.com/nlohmann/json/releases/download/v3.6.1/json.hpp

WORKDIR /acquisition_module
COPY ./entrypoint.sh /acquisition_module
RUN chmod +x /acquisition_module/entrypoint.sh
CMD ./entrypoint.sh

