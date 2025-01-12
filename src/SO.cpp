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
    auto it = find(listaCircular_SO.begin(), listaCircular_SO.end(), idProcesso);
    if (it == listaCircular_SO.end())
    {
        listaCircular_SO.push_back(idProcesso); // Adiciona apenas se não existir
        estadosProcessos[idProcesso] = "PRONTO";
    }
    else
    {
        cout << "Processo já está na fila: " << idProcesso << endl;
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

    // Remove o processo atual da lista
    listaCircular_SO.erase(listaCircular_SO.begin() + indiceAtual);

    // Ajusta o índice atual
    if (listaCircular_SO.empty())
    {
        indiceAtual = 0; // Reseta o índice se a lista estiver vazia
    }
    else
    {
        indiceAtual = indiceAtual % listaCircular_SO.size(); // Garante que o índice esteja válido
    }

    return idProcesso;
}


int iniciando_SO(pthread_t &thread_SO, vector<int> processos)
{   
    int op;
    cout <<  "\n\n\t ----------{Escolha a Politica de Escalanomento}---------- "<< endl;
    cout <<  "\n\t\t [1] = FCFS.";
    cout <<  "\n\t\t [2] = Shortest Job First";
    cout <<  "\n\t\t [3] = Prioridade";
    cout <<  "\n\t\t [>] = ";
    cin>>op;
    cout <<  "\n\n\t --------------------------------------------------------- "<< endl;

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
