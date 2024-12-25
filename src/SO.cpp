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
}

void *fazerListaCircular_SO(void *arg)
{
    vector<int> *processos = static_cast<vector<int> *>(arg);
    for (int id : *processos)
    {
        atualizarListaCircular(id);
    }
    return nullptr;
}

int obterProximoProcesso()
{
    lock_guard<mutex> lock(mutexListaCircular);
    if (listaCircular_SO.empty())
    {
        return -1;
    }
    int idProcesso = listaCircular_SO[indiceAtual];
    indiceAtual = (indiceAtual + 1) % listaCircular_SO.size();
    return idProcesso;
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
