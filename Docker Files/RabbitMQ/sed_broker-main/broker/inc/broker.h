#ifndef BROKER_H
#define BROKER_H

#include <json.hpp>
#include <string>

using namespace std;

using json = nlohmann::json;

class Broker
{

    public:

        Broker(){};
        virtual void connect(string, string, string) = 0;
        virtual void connect(string, string) = 0;
        virtual void connect(string) = 0;
        virtual void publish(string) = 0;
        virtual string consume() = 0;

};

#endif