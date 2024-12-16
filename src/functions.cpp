#include "unidadeControle.hpp"
#include "functions.hpp"

// Função para inicializar um processo com quantum e timestamp
Processo criarProcesso(int quantumInicial, int idProcesso)
{
    Processo p;
    p.quantum = quantumInicial;
    p.timestamp = CLOCK; // Timestamp inicial é o valor atual do CLOCK
    p.id = idProcesso;
    return p;
}

void LerInstrucoesDoArquivo(const string &nomeArquivo, int *registradores){

    // string nomeArquivo = "input.data";
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    while (getline(arquivo, linha))
    {
        // stringstream ss(linha);
        UnidadeControle(registradores, linha);
        cout << "Clock: " << CLOCK << endl;
    }

    arquivo.close();
}