#include "noveltyDetector.h"
#include <fstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

vector<float> NoveltyDetector::coeficientes; //veio do .h para nao ter roblema com static e linker

NoveltyDetector::NoveltyDetector(Broker* broker_ptr) : Consumer(broker_ptr) // chamada explícita ao construtor base
{
    this->incializacao();                   // Inicializa o filtro
    cout << "[NoveltyDetector] Inicializado." << endl;
}

void NoveltyDetector :: run()
{
  cout << "[NoveltyDetector] Iniciando processamento..." << endl;
  
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

bool NoveltyDetector :: process()
{
    /*Place your code here
        ETAPAS:
            1. Filtragem
            2. Zero Crossing e frequência estimada
            3. DTW
            4. Flag Novidade
    */

    // cout << "[NoveltyDetector] Iniciando processamento..." << endl;

    //_________________ DECLARAÇÕES E INICIALIZAÇÕES  _________________ //     

    // Vetor de arquivos de entrada para os filtros base
    vector<ifstream> input_W(3);            //ver sobre isso: quantidade não será 3? que tipo de dado é esse? 
    
    //DEBUG
    ofstream output_samplecount;            // Sample count -> contador de amostras
    vector<ofstream> output_files(NCH);     // Um arquivo para cada canal

    ofstream output_latencia;               // Derivada 1ª fasor fundamental -> guardar latencia

    
    // ____________________ PROCESSAMENTO DE DADOS  ___________________ //     

    // Processa cada item do lote recebido
    for (int ii = 0; ii < BATCHSIZE; ii++)   //BATCHSIZE -> numero de amostras por lote, variavel de ambiente
    {
        // Obtém as amostras do JSON
        auto amostras_canais = this->message_json[ii]["samples"].get<map<int, vector<float>>>();

        // Para cada conjunto de amostras recebidas
        for (const auto& [id_amostra, Sample_ASDU] : amostras_canais) //id_amostra = samplecount
        {
            //Obtém as amostras do JSON
            auto amostras_canais = this->message_json[ii]["samples"].get<map<int, vector<float>>>();
            
                if(id_amostra%4 == 0 || id_amostra == 0 )
                {
                    // cout << "Valor: "<< Sample_ASDU[0] << "---- Filtrado: " << this->processar(Sample_ASDU[0]) << endl;
                    cout << "[NoveltyDetector] ID: " << id_amostra << " | Valor: " << Sample_ASDU[0] << " | Saida filtro: " << this->processar(Sample_ASDU[0]) << endl;
                }  
        }
    }

    //cout << "[NoveltyDetector] Processamento concluído com sucesso!" << endl;

    return true;
}

//=============== Filtro Nutall - MÉTODOS ===============//

//Inicializa o buffer e carrega os coeficientes de arquivo
void NoveltyDetector :: incializacao() 
{
    saida = 0.0f;                                   //inicializa saída
    carregarCoeficientes("Num_Nutall.txt");         //coeficientes do filtro
    xn.assign(ORDEM + 1, 0.0f);                     //inicializa buffer com zeros   
}

// Leitura do arquivo de coeficientes
void NoveltyDetector :: carregarCoeficientes(const string& arquivo) 
{
    ifstream file(arquivo);
    if (!file.is_open()) {
        throw runtime_error("Erro ao abrir arquivo de coeficientes: " + arquivo);
    }

    coeficientes.clear();
    float valor;
    while (file >> valor) {
        coeficientes.push_back(valor);
    }
    file.close();

    if (coeficientes.size() != ORDEM + 1) {
        throw runtime_error("Quantidade incorreta de coeficientes no arquivo (" +
                            to_string(coeficientes.size()) + " encontrados).");
    }
}

// Processamento de uma amostra
double NoveltyDetector :: processar(double sinal_entrada) 
{
    //Calcula saída -> sinal filtrado
    saida = 0.0f;

    //Atualiza o buffer
    for (size_t i = ORDEM; i > 0; --i) {
        xn[i] = xn[i - 1];
    }
    
    xn[0] = sinal_entrada;             // Insere nova amostra no início do buffer

    for (size_t i = 0; i <= ORDEM; ++i) 
    {
        saida += (coeficientes[i] * xn[i]);
    }

    return saida/1000;
}

// Zera o buffer
void NoveltyDetector :: resetFiltro() noexcept 
{
    fill(xn.begin(), xn.end(), 0.0f);
}
