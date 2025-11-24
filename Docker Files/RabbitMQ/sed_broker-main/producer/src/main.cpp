#include <rmqbroker.h>
#include <svgts.h>
#include <iostream>
#include <stdio.h>
#include <json.hpp>

using json = nlohmann::json;
using namespace std;

int main(int argc, char **argv)
{

  Broker* broker = new RMQBroker("rabbitmq", 5672, "dam", "password");
    
  string dst_mac = getenv("DST_MAC");
  string src_mac = getenv("SRC_MAC");
  string file_path = getenv("FILE_PATH");

  const char* env_var = std::getenv("USE_PCAP");
  ProcessBusGateway* svg;

  if (env_var != nullptr && std::string(env_var) == "true") 
  {
    
    svg = new SampledValuesGateWayTShark(broker, file_path);
  
  } 
  else 
  {
    
    svg = new SampledValuesGateWayTShark(broker, dst_mac, src_mac);

  }

  cout << "Sending packets..." << endl;
  svg->run();

  return 0;
}