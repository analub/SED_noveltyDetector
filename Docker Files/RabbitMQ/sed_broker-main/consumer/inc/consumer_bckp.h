#ifndef CONSUMER_H
#define CONSUMER_H

#include <string>
#include <vector>
#include <map>
#include <thread>
#include <iostream>
#include <datatypes.h>
#include <json.hpp>
#include <broker.h>

#include <filtro_nutall.h>

using namespace std;
using json = nlohmann::json;

class Consumer
{

protected:

  Broker* broker;
  json message_json, processed_message_json;;
  SVData* data[BATCHSIZE];
  string message_payload, processed_message_payload;

public:

  Consumer(Broker*);
  void run();
  bool acquire();
  bool process();
  virtual bool process() = 0;
  bool from_json(const json &j, SVData &p);
  
};

#endif