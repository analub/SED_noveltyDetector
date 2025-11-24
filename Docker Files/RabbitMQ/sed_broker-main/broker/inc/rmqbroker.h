#ifndef RMQBROKER_H
#define RMQBROKER_H

#include <broker.h>
#include <thread>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

using namespace AmqpClient;

class RMQBroker: public Broker
{   

    private:

        string broker_name;
        string amqp_host;
        int amqp_port;
        string amqp_username;
        string amqp_password;
        string queue_name;
        string svdata_exchange_name;
        string routing_key;
        string consumer_tag;
        AmqpClient::Channel::ptr_t connection;

    public:

        RMQBroker(string host, int port, string username, string password);

        void connect(string svdata_exchange_name, string key);
        void connect(string queue_name, string svdata_exchange_name, string key);
        void connect(string queue_name);
        
        void publish(string);
        string consume();

};

#endif