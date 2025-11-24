#include <iostream>
#include <rmqbroker.h>
#include <consumer.h>

using namespace std;

int main(int argc, char **argv)
{
  
  printf("Running consumer...\n");

  Broker* broker =  new RMQBroker("rabbitmq", 5672, "dam", "password");

  Consumer* msg_consumer = new Consumer(broker);

  msg_consumer->run();

  return 0;

}
