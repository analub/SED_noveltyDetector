#ifndef ARQUIVO_TXT_H
#define ARQUIVO_TXT_H

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#define FS 15360  //1000                       // Frequência de amostragem em Hz
#define LIMITE_AMOSTRAS 60*FS   

class arquivoTxt {
private:
    ifstream fin;
    ofstream fout;
    string ultimoConteudo;

    size_t contadorAmostras;

public:
    arquivoTxt();

    bool abrirEscrita(const string& nomeArquivo);
    bool abrirLeitura(const string& nomeArquivo);

    void escrever(const string& texto);

    string lerTudo();
    void printConteudo() const;

    void fechar();

    ~arquivoTxt();
};

#endif
