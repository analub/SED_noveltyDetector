#include <iostream>
#include <rmqbroker.h>
#include <consumer.h>
#include <noveltyDetector.h>

using namespace std;

int main(int argc, char **argv)
{
  
  printf("Running consumer...\n");

  Broker* broker =  new RMQBroker("rabbitmq", 5672, "dam", "password");

  //Consumer* msg_consumer = new Consumer(broker);
  Consumer* msg_novelty = new NoveltyDetector(broker);

  //msg_consumer->run();
  msg_novelty->run();

  return 0;

}
