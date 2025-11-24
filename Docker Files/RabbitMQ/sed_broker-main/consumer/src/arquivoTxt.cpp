#include "arquivoTxt.h"

arquivoTxt::arquivoTxt()
    : contadorAmostras(0)
{
}

bool arquivoTxt::abrirEscrita(const string& nomeArquivo) {
    fout.open(nomeArquivo, ofstream::out | ofstream::trunc);

    if (!fout.is_open()) {
        cerr << "Erro ao abrir arquivo para escrita!\n";
        return false;
    }

    contadorAmostras = 0;  // Zera o contador quando abre o arquivo

    return true;
}

bool arquivoTxt::abrirLeitura(const string& nomeArquivo) {
    fin.open(nomeArquivo);

    if (!fin.is_open()) {
        cerr << "Erro ao abrir arquivo para leitura!\n";
        return false;
    }

    return true;
}

void arquivoTxt::escrever(const string& texto) {
    if (!fout.is_open()) {
        cerr << "Arquivo não está aberto para escrita!\n";
        return;
    }

    // Verifica se atingiu o limite de amostras
    if (contadorAmostras >= LIMITE_AMOSTRAS) {
        cerr << "Limite de amostras atingido! (" << LIMITE_AMOSTRAS << ")\n";
        return;
    }

    fout << texto << "\n";
    contadorAmostras++;
}

string arquivoTxt::lerTudo() {
    if (!fin.is_open()) {
        cerr << "Arquivo não está aberto para leitura!\n";
        return "";
    }

    ultimoConteudo.clear();
    string linha;

    while (getline(fin, linha)) {
        ultimoConteudo += linha + "\n";
    }

    return ultimoConteudo;
}

void arquivoTxt::printConteudo() const {
    cout << "=== Conteúdo do arquivo ===\n";
    cout << ultimoConteudo;
}

void arquivoTxt::fechar() {
    if (fin.is_open()) fin.close();
    if (fout.is_open()) fout.close();
}

arquivoTxt::~arquivoTxt() {
    fechar();
}
