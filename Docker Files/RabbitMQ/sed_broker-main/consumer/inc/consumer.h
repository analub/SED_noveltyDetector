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
  void run()=0;
  virtual bool acquire();    
  virtual bool process() =0;  //para fazer derivação do Detector de Novidades
  bool publish(); 
  bool from_json(const json &j, SVData &p);
  
};

#endif