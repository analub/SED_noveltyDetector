#include <iostream>
#include <rmqbroker.h>
#include <consumer.h>
#include <cstring>
#include <vector>

#include <filtro_nutall.h>

using namespace std;
using json = nlohmann::json;

Filtro_Nutall teste;

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

bool Consumer::process()
{

  //Place your code here

  //Example
  // this->processed_message_json = this->message_json;

  // Processa cada item do lote recebido
  for (int ii = 0; ii < BATCHSIZE; ii++) 
  {
    // Obtém as amostras do JSON
    auto amostras_canais = this->message_json[ii]["samples"].get<map<int, vector<float>>>();

    // Para cada conjunto de amostras recebidas
    for (const auto& [id_amostra, Sample_ASDU] : amostras_canais) //sample count = id_amostra
    {
      // output_samplecount << id_amostra << endl;  
      // Escreve os valores de cada canal em seu respectivo arquivo

      //for (int canal = 0; canal < NCH; canal++) 
      //{
          //output_files[canal] << Sample_ASDU[canal] << endl;
      //}

      if(id_amostra%4 == 0 || id_amostra == 0 )
      {
        //output_samplecount << filtroTeste.processar(Sample_ASDU[0]) << endl;
        //cout << "Consumer 1:" << "Id "<<  id_amostra << ": " << Sample_ASDU[0] << " --- " <<filtroTeste.processar(Sample_ASDU[0]) << endl;
        cout << "Valor: "<< Sample_ASDU[0] << "---- Filtrado: " << teste.processar(Sample_ASDU[0]) << endl;
      }
    }

  // Filtro_Nutall teste;

  // cout << "Funcionou UM!" << teste.getSaida() << endl;

  }

  return true;
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
