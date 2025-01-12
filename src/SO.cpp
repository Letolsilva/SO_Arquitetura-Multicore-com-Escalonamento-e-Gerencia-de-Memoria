#include "SO.hpp"
#include <iostream>
#include <unistd.h>

vector<int> listaCircular_SO;
vector<SO> listaCircular_SO_2;
size_t indiceAtual = 0;
mutex mutexListaCircular;

void add_ListaCircular(PCB processo)
{
    lock_guard<mutex> lock(mutexListaCircular);
    // Verifica se o processo já está na lista
    auto it = find(listaCircular_SO.begin(), listaCircular_SO.end(), processo.id);
    if (it == listaCircular_SO.end())
    {
        cout << " ADD : " << processo.id;
        SO aux_job;
        aux_job.id_processo = processo.id;
        aux_job.ciclo_de_vida = processo.ciclo_de_vida;
        aux_job.prioridade = processo.prioridade;
        listaCircular_SO_2.push_back(aux_job); // Adiciona apenas se não existir
        //estadosProcessos[idProcesso] = "PRONTO";
    }
    else
    {
        cout << "Processo já está na fila: " << processo.id << endl;
    }
}

void gerarLista(){

    for (const Page &pag : memoryPages)
    {
        add_ListaCircular(pag.pcb);
    }
}

/*
void *ordenar_Shortest_Job_First(void *arg)
{
    if(!listaCircular_SO_2.empty()){
        sort(listaCircular_SO_2.begin(), listaCircular_SO_2.end(), [](const SO &a, const SO &b) {
            return a.ciclo_de_vida < b.ciclo_de_vida;
        });
    }
    
    return nullptr;
}
*/

void *FCFS(void *arg)
{
    (void)arg;

    while(!listaCircular_SO_2.empty()){
    }
    
    return nullptr;
}

void *First_Job_First(void *arg)
{
    (void)arg;

    while(!listaCircular_SO_2.empty()){
        lock_guard<mutex> lock(mutexListaCircular);
        sort(listaCircular_SO_2.begin(), listaCircular_SO_2.end(), [](const SO &a, const SO &b) {
            return a.ciclo_de_vida < b.ciclo_de_vida;
        });
    }
    
    return nullptr;
}

int obterProximoProcesso()
{
    lock_guard<mutex> lock(mutexListaCircular);
    for (const auto &so : listaCircular_SO_2)
    {

    cout << "ID: " << so.id_processo
                  << ", Ciclo de Vida: " << so.ciclo_de_vida
                  << ", Prioridade: " << so.prioridade << endl;
    }
    if (listaCircular_SO_2.empty())
    {
        return -1;
    }

    // Pega o próximo processo da lista circular
    int idProcesso = listaCircular_SO_2[indiceAtual].id_processo;

    // Remove o processo atual da lista
    listaCircular_SO_2.erase(listaCircular_SO_2.begin() + indiceAtual);
    cout << "\t {Removi:  " << idProcesso << endl;

    // Ajusta o índice atual
    if (listaCircular_SO_2.empty())
    {
        indiceAtual = 0; // Reseta o índice se a lista estiver vazia
    }
    else
    {
        indiceAtual = indiceAtual % listaCircular_SO_2.size(); // Garante que o índice esteja válido
    }

    return idProcesso;
}


int iniciando_SO(pthread_t &thread_SO)
{   
    gerarLista();
    for (const auto &so : listaCircular_SO_2)
    {
        std::cout << "ID: " << so.id_processo
                  << ", Ciclo de Vida: " << so.ciclo_de_vida
                  << ", Prioridade: " << so.prioridade << std::endl;
    }
    

    int op;
    cout <<  "\n\n\t ----------{Escolha a Politica de Escalanomento}---------- "<< endl;
    cout <<  "\n\t\t [1] = FCFS.";
    cout <<  "\n\t\t [2] = Shortest Job First";
    cout <<  "\n\t\t [3] = Prioridade";
    cout <<  "\n\t\t [>] = ";
    cin>>op;
    cout <<  "\n\n\t --------------------------------------------------------- "<< endl;

    int ret =0;
    switch (op){
        case 1:
            ret = pthread_create(&thread_SO, nullptr, FCFS, nullptr);
            break;
        case 2:
            ret = pthread_create(&thread_SO, nullptr, First_Job_First, nullptr);
            cout << " TAM " << memoryPages.size() << endl;
            break;
        case 3:
            break;
        default:
            break;
    }

    if (ret != 0)
    {
        cerr << "Erro ao criar a thread do SO!" << endl;
        return 1;
    }

   // cout << " \n\t - Lista dos jobs carregados:  " << endl << endl;
   // imprimirListaCircular();

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
    if (listaCircular_SO_2.empty())
    {
        cout << "A lista circular está vazia." << endl;
        return;
    }

    cout << "Lista circular atual:" << endl;
    size_t index = 0;
    for (const SO &pag : listaCircular_SO_2)
    {
        cout << "Índice " << index << ": Processo ID " << pag.id_processo;
        if (index == indiceAtual)
        {
            cout << " <- Índice atual";
        }
        cout << endl;
        index++;
    }
}
