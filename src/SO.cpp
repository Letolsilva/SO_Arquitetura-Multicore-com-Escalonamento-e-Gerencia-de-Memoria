#include "SO.hpp"
#include <iostream>
#include <unistd.h>

vector<int> listaCircular_SO;
size_t indiceAtual = 0;
mutex mutexListaCircular;

void atualizarListaCircular(int idProcesso)
{
    lock_guard<mutex> lock(mutexListaCircular);

    // Verifica se o processo já está na lista
    auto it = std::find(listaCircular_SO.begin(), listaCircular_SO.end(), idProcesso);
    if (it == listaCircular_SO.end())
    {
        listaCircular_SO.push_back(idProcesso); // Adiciona apenas se não existir
    }
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

    // Pega o próximo processo da lista circular
    int idProcesso = listaCircular_SO[indiceAtual];

    // Move o processo atual para o final da lista
    listaCircular_SO.push_back(idProcesso);
    listaCircular_SO.erase(listaCircular_SO.begin() + indiceAtual);

    // O índice atual vai sempre para o próximo processo
    // O próximo índice será o 0, pois removemos o primeiro elemento
    indiceAtual = 0;

    // Retorna o ID do processo retirado da lista
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

void imprimirProcessosEsperando()
{
    lock_guard<mutex> lock(mutexListaCircular);
    cout << "Processos esperando:" << endl;
    for (int id : listaCircular_SO)
    {
        cout << "Processo ID: " << id << endl;
    }
}

void imprimirListaCircular()
{
    lock_guard<mutex> lock(mutexListaCircular);
    if (listaCircular_SO.empty())
    {
        cout << "A lista circular está vazia." << endl;
        return;
    }

    cout << "Lista circular atual:" << endl;
    size_t index = 0;
    for (int id : listaCircular_SO)
    {
        cout << "Índice " << index << ": Processo ID " << id;
        if (index == indiceAtual)
        {
            cout << " <- Índice atual";
        }
        cout << endl;
        index++;
    }
}
