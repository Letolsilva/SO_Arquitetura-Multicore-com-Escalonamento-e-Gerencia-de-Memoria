#include "SO.hpp"
#include <iostream>
#include <unistd.h>

vector<int> listaCircular_SO;
size_t indiceAtual = 0;
mutex mutexListaCircular;

void atualizarListaCircular(int idProcesso)
{
    lock_guard<mutex> lock(mutexListaCircular);
    listaCircular_SO.push_back(idProcesso);
    cout << "Processo ID=" << idProcesso << " adicionado à lista circular." << endl;
}

// Função da thread do SO que preenche a lista circular
void *fazerListaCircular_SO(void *arg)
{
    vector<int> *processos = static_cast<vector<int> *>(arg);
    for (int id : *processos){
        atualizarListaCircular(id);
    }
    return nullptr;
}

int obterProximoProcesso()
{
    lock_guard<mutex> lock(mutexListaCircular);
    if (listaCircular_SO.empty())
    {
        return -1; // Indica que não há processos na lista
    }
    int idProcesso = listaCircular_SO[indiceAtual];
    indiceAtual = (indiceAtual + 1) % listaCircular_SO.size();
    return idProcesso;
}

void executarProcessoCircular()
{
    while (true)
    {
        int idProcesso = obterProximoProcesso();
        if (idProcesso == -1)
        {
            usleep(1000); // Aguarde para evitar loop excessivo
            continue;
        }

        // Aqui ocorre a execução do processo
        cout << "Executando processo ID=" << idProcesso << endl;

        // Simule o tempo de execução
        usleep(1000);
    }
}
int iniciando_SO(pthread_t &thread_SO, vector<int> processos)
{
    int ret = pthread_create(&thread_SO, nullptr, fazerListaCircular_SO, &processos);

    if (ret != 0)
    {
        cerr << "Erro ao criar a thread do SO!" << endl;
        return 1;
    }

    return 0;
}

// Imprime a lista circular
void imprimirListaCircular_SO()
{
    lock_guard<mutex> lock(mutexListaCircular);
    for (int id : listaCircular_SO)
    {
        cout << "Processo na lista circular: ID=" << id << endl;
    }
}
