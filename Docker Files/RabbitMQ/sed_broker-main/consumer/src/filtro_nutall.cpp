#include <filtro_nutall.h>
#include <fstream>
#include <stdexcept>
#include <algorithm>

using namespace std;

// Construtor: inicializa o buffer e carrega os coeficientes de arquivo
Filtro_Nutall :: Filtro_Nutall() 
{
    saida = 0.0f;                                   //inicalizanado variavel saida com zero
    carregarCoeficientes("Num_Nutall.txt");         //carregando coeficientes do filtro
    xn.assign(ORDEM + 1, 0.0f);                     //inicializando buffer xn om zero
}

// Leitura do arquivo de coeficientes
void Filtro_Nutall :: carregarCoeficientes(const string& arquivo) 
{
    ifstream file(arquivo);
    if (!file.is_open()) 
        throw runtime_error("Erro ao abrir arquivo de coeficientes: " + arquivo);

    coeficientes.clear();
    float valor;

    while (file >> valor) 
    {
        coeficientes.push_back(valor);
    }
    file.close();

    if (coeficientes.size() != ORDEM + 1) 
    {
        throw runtime_error("Quantidade incorreta de coeficientes no arquivo (" +
        to_string(coeficientes.size()) + " encontrados).");
    }
}

// Processamento de uma amostra
float Filtro_Nutall ::processar(float sinal_entrada) 
{
    // Desloca o buffer
    for (size_t i = ORDEM; i > 0; --i) 
        xn[i] = xn[i - 1];
    
    xn[0] = sinal_entrada;

    // Calcula saída -> sinal filtrado
    saida = 0.0f;
    for (size_t i = 0; i <= ORDEM; ++i) 
        saida += coeficientes[i] * xn[i];
    
    return saida;
}

// Zera o buffer
void Filtro_Nutall ::reset() noexcept 
{
    fill(xn.begin(), xn.end(), 0.0f);
}

// Retorna a ordem do filtro
size_t Filtro_Nutall ::getOrdem() const noexcept 
{
    return ORDEM;
}

// Retornar saída
float Filtro_Nutall :: getSaida()
{
    return saida;
}
