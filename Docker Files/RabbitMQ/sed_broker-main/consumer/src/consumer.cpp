#include <iostream>
#include <rmqbroker.h>
#include <consumer.h>
#include <cstring>
#include <vector>

using namespace std;
using json = nlohmann::json;

Consumer::Consumer(Broker* broker)
{

  this->broker = broker;
  this->broker->connect(string(getenv("HOSTNAME")), getenv("R_KEY"), getenv("R_KEY"));

}

void Consumer::run()
{
  while(true)
  {

    if(this->acquire())
    {

      if(this->process())
      {

        if(!this->publish())
        {

          cout << "Error Publishing Message!" << endl;

        }
      }
    }
  }
}

bool Consumer::acquire()
{

  try
  {
    
    this->message_payload = this->broker->consume();
    this->message_json = json::parse(this->message_payload);
    
    return true;
  
  }
  catch(const std::exception& e)
  {
    
    std::cerr << e.what() << '\n';

    return false;
  
  }
}

bool Consumer::publish()
{

  try
  {

    this->broker->publish(this->processed_message_json.dump());
    return true;

  }
  catch(const std::exception& e)
  {
    
    std::cerr << e.what() << '\n';
    return false;

  } 
}
