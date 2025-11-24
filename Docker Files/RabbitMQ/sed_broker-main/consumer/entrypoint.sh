#!/bin/sh
ldconfig
mv /json.hpp ./json.hpp 2>/dev/null

# Compilação dos objetos
g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/main.cpp -o bin/main.o
g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/consumer.cpp -o bin/consumer.o 
g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/filtro_nutall.cpp -o bin/filtro_nutall.o
g++ -std=c++11 -Wall -fexceptions -g -pthread -I libs/inc -c libs/src/rmqbroker.cpp -o bin/rmqbroker.o
g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/noveltyDetector.cpp -o bin/noveltyDetector.o
g++ -std=c++11 -Wall -fexceptions -g -pthread -I inc -I libs/inc -c src/arquivoTxt.cpp -o bin/arquivoTxt.o



# Linkagem correta
g++ -std=c++11 -o consumer_module bin/rmqbroker.o bin/consumer.o bin/main.o bin/noveltyDetector.o bin/ArquivoTxt.o -L/usr/local/lib -lz -lpthread -lSimpleAmqpClient

# Permissões e execução
chmod +x ./consumer_module
LD_LIBRARY_PATH=/usr/local/lib/x86_64-linux-gnu/ ./consumer_module
