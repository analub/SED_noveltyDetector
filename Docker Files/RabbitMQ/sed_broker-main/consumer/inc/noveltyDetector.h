#ifndef NOVELTY_DETECTOR_H
#define NOVELTY_DETECTOR_H

#include "consumer.h"

#define NUN_HARM 10             // numero de harmonico salvos em txt
#define NUM_CH_ESTIMADOS 3      // numero de canais estimas e salvos em txt

/*
    CLASSE DERIVADA PARA IMPLEMENTAR O DETECTOR DE NOVIDADES
        > Apenas process será sobrescrito
        > Todas as etapas do detector estão aqui
        > Fazer um vetor para as amostras -> limitar o tamanho para 1s
*/

class NoveltyDetector : public Consumer
{
public:
        NoveltyDetector(Broker* broker_ptr);

        //Sobrescreve o método virtual da base
        bool process() override;
        void run() override;

private:
        //=============== ETAPAS DETECTOR ===============//
        double filtroNutall(double &sinal); //passagem por referencia


        //=============== Filtro Nutall - ATRIBUTOS ===============//

        static constexpr size_t ORDEM = 40;     // Ordem fixa do filtro
        static vector<float> coeficientes;             // Coeficientes FIR
        vector<float> xn;                       // Buffer de amostras
        float saida;                            // Saída do filtro = sinal filtrado 

        //=============== Filtro Nutall - MÉTODOS ===============//
        void carregarCoeficientes(const string& arquivo);

        // Processa uma amostra e retorna a saída filtrada
        double processar(double sinal_entrada);

        // Reseta o buffer (zera todas as amostras)
        void resetFiltro() noexcept;

        // Inicializa o buffer e carrega os coeficientes de arquivo
        void incializacao();
    };  

#endif
