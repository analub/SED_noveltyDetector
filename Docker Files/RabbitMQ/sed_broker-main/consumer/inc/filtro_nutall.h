#ifndef FILTRO_NUTALL_H
#define FILTRO_NUTALL_H

#include <vector>
#include <string>
using namespace std;

class Filtro_Nutall 
{
    private:
            static constexpr size_t ORDEM = 40;     // Ordem fixa do filtro
            vector<float> coeficientes;             // Coeficientes FIR
            vector<float> xn;                       // Buffer de amostras
            float saida;                            //Sinal Filtrado

            // Método para carregar coeficientes do arquivo
            void carregarCoeficientes(const string& arquivo);

    public:
            //Construtor: inicializar buffer e carregar coeficientes fixos de arquivo
            Filtro_Nutall();
        //     ~Filtro_Nutall();

            //Processa uma amostra e retorna a saída filtrada
            float processar(float amostraEntrada);

            //Reseta o buffer (zera todas as amostras)
            void reset() noexcept;

            //Metodos get
            size_t getOrdem() const noexcept;
            float getSaida();
};

#endif