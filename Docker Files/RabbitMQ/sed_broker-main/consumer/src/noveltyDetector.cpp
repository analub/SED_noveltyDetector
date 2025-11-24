#include "noveltyDetector.h"
#include <fstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>
#include "arquivoTxt.h"

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

    // Arquivos de Texto
    arquivoTxt out_FiltroNutall;
    out_FiltroNutall.abrirEscrita("Resultados/Filtro/SinalFiltrado.txt");
    arquivoTxt out_SinalEntrada;
    out_SinalEntrada.abrirEscrita("Resultados/Filtro/SinalEntrada.txt");

    arquivoTxt out_ZC;
    out_ZC.abrirEscrita("Resultados/zeroCrossing/ZC.txt");

    arquivoTxt out_FreqEstimada;
    out_FreqEstimada.abrirEscrita("Resultados/zeroCrossing/FreqEstimada.txt");
     
    
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
                    cout << "[NoveltyDetector] ID: " << id_amostra << " | Valor: " << Sample_ASDU[0] << " | Saida filtro: " << this->processar(Sample_ASDU[0]) << endl;
                    
                    out_FiltroNutall.escrever(to_string(this->processar(Sample_ASDU[0])) + "\n");  //salva no txt valor filtrado
                    out_SinalEntrada.escrever(to_string(Sample_ASDU[0]) + "\n");   //salva no txt valor entrada
                }  
        }
    }

    //cout << "[NoveltyDetector] Processamento concluído com sucesso!" << endl;

    //=================== Fecha arquivos de texto ===================//
    out_FiltroNutall.fechar();
    out_SinalEntrada.fechar();
    out_ZC.fechar();
    out_FreqEstimada.fechar();

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

//=================================================================//
