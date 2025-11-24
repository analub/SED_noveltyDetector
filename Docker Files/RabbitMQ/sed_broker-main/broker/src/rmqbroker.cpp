#include <rmqbroker.h>
#include <iostream>
#include <datatypes.h>

using namespace std;

RMQBroker::RMQBroker(string host, int port, string username, string password)
{
    
    this->broker_name = "RabbitMQ";
    this->amqp_host = host;
    this->amqp_port = port;
    this->amqp_username = username;
    this->amqp_password = password;

}

void RMQBroker::connect(string queue_name, string svdata_exchange_name, string key)
{

    this->svdata_exchange_name = svdata_exchange_name;
    this->routing_key = key;
    this->queue_name = queue_name;
    
    int num_attempts = 20;
    while (num_attempts--)
    {
        
        try
        {
            
            this->connection = AmqpClient::Channel::Create(this->amqp_host, this->amqp_port, this->amqp_username, this->amqp_password);
            this->connection->DeclareQueue(this->queue_name, false, false, false, true);
            this->connection->BindQueue(this->queue_name, "process_bus_data", "sampled_values");
            this->consumer_tag = this->connection->BasicConsume(this->queue_name, "", true, true, false);
            this->connection->DeclareExchange(this->svdata_exchange_name, Channel::EXCHANGE_TYPE_DIRECT);
            cout << "Conexão com RabbitMQ realizada com sucesso." << endl;
            break;

        }
        catch (exception &e)
        {

            cout << "Erro na conexão com o RabbitMQ:" << e.what() << " Tentativas restantes: " << num_attempts << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));

        }
    }
}


void RMQBroker::connect(string svdata_exchange_name, string key)
{

    this->svdata_exchange_name = svdata_exchange_name;
    this->routing_key = key;

    int num_attempts = 20;
    while (num_attempts--)
    {
        
        try
        {
            
            this->connection = AmqpClient::Channel::Create(this->amqp_host, this->amqp_port, this->amqp_username, this->amqp_password);
            this->connection->DeclareExchange(this->svdata_exchange_name, Channel::EXCHANGE_TYPE_DIRECT);
            cout << "Conexão com RabbitMQ realizada com sucesso." << endl;
            break;

        }
        catch (exception &e)
        {

            cout << "Erro na conexão com o RabbitMQ:" << e.what() << " Tentativas restantes: " << num_attempts << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));

        }
    }
}

void RMQBroker::connect(string queue_name)
{

    this->queue_name = queue_name;

    int num_attempts = 20;
    while (num_attempts--)
    {
        
        try
        {
            
            this->connection = AmqpClient::Channel::Create(this->amqp_host, this->amqp_port, this->amqp_username, this->amqp_password);
            this->connection->DeclareQueue(this->queue_name, false, false, false, true);
            this->connection->BindQueue(this->queue_name, "process_bus_data", "sampled_values");
            this->consumer_tag = this->connection->BasicConsume(this->queue_name, "", true, true, false);
            cout << "Conexão com RabbitMQ realizada com sucesso." << endl;
            break;

        }
        catch (exception &e)
        {
            
            cout << "Erro na conexão com o RabbitMQ:" << e.what() << " Tentativas restantes: " << num_attempts << endl;
            std::this_thread::sleep_for(std::chrono::seconds(1));

        }
    }
}

void RMQBroker::publish(string message)
{
    
    BasicMessage::ptr_t message_str = BasicMessage::Create(message);
    connection->BasicPublish(this->svdata_exchange_name, this->routing_key, message_str);

}

string RMQBroker::consume()
{

    Envelope::ptr_t envelope = this->connection->BasicConsumeMessage(this->consumer_tag);
    BasicMessage::ptr_t bodyBasicMessage = envelope->Message();
    return bodyBasicMessage->Body();
    
}
