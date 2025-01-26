#include "SO.hpp"
#include <iostream>
#include <unistd.h>

vector<int> listaCircular_SO;
vector<SO> listaCircular_SO_2;
size_t indiceAtual = 0;
mutex mutexListaCircular;

void remover_ListaCircular(int id_processo)
{
    lock_guard<mutex> lock(mutexListaCircular);

    auto it = find_if(listaCircular_SO_2.begin(), listaCircular_SO_2.end(),
                      [id_processo](const SO &so)
                      { return so.id_processo == id_processo; });

    if (it != listaCircular_SO_2.end())
    {
        // Remove o processo encontrado
        listaCircular_SO_2.erase(it);
    }
    else
    {
        // Caso o processo não esteja na lista
        // cout << "Processo não encontrado na lista: " << id_processo << endl;
    }
}

void add_ListaCircular(PCB processo)
{
    lock_guard<mutex> lock(mutexListaCircular);
    // Verifica se o processo já está na lista
    auto it = find_if(listaCircular_SO_2.begin(), listaCircular_SO_2.end(),
                      [&processo](const SO &so) { return so.id_processo == processo.id; });    if (it == listaCircular_SO_2.end())
    {
        SO aux_job;
        aux_job.id_processo = processo.id;
        aux_job.ciclo_de_vida = processo.ciclo_de_vida;
        aux_job.prioridade = processo.prioridade;
        aux_job.conjunto_chaves = processo.conjunto_chaves;
        listaCircular_SO_2.push_back(aux_job); // Adiciona apenas se não existir
        // estadosProcessos[idProcesso] = "PRONTO";
    }
    else
    {
        cout << "Processo já está na fila: " << processo.id << endl;
    }
}

std::map<std::string, std::vector<PCB>> agruparJobsPorChaves(const std::vector<PCB>& jobs) {
    std::map<std::string, std::vector<PCB>> grupos;

    for (const auto& job : jobs) {
        for (const auto& chave : job.conjunto_chaves) {
            grupos[chave].push_back(job);
        }
    }

    return grupos;
}

void gerarLista()
{
    for (const Page &pag : memoryPages)
    {
        add_ListaCircular(pag.pcb);
    }
}

void gerar_lista_similiaridade()
{
    vector<PCB> listaJob;

    for (const Page &pag : memoryPages)
    {
       // add_ListaCircular(pag.pcb);
        listaJob.push_back(pag.pcb);
    }

    auto grupos = agruparJobsPorChaves(listaJob);

    for (const auto& grupo : grupos) {
        cout << "\n\t -> Grupo  ... " << endl;

        std::cout << "Chave: " << grupo.first << std::endl;
        for (const auto& job : grupo.second){
            cout << "ID do Job: " << job.id << std::endl;
            add_ListaCircular(job);
        }
    }
}

void *FCFS(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
    }

    return nullptr;
}

void *Similiaridade(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
    }

    return nullptr;
}

void *First_Remain_Job_First(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
        lock_guard<mutex> lock(mutexListaCircular);
        sort(listaCircular_SO_2.begin(), listaCircular_SO_2.end(), [](const SO &a, const SO &b)
            { return a.ciclo_de_vida < b.ciclo_de_vida; });
    }

    return nullptr;
}

void *Prioridade(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
        lock_guard<mutex> lock(mutexListaCircular);
        sort(listaCircular_SO_2.begin(), listaCircular_SO_2.end(), [](const SO &a, const SO &b)
             { return a.prioridade > b.prioridade; });
    }

    return nullptr;
}

int obterProximoProcesso()
{
    lock_guard<mutex> lock(mutexListaCircular);

    if (listaCircular_SO_2.empty())
    {
        return -1;
    }

    // Pega o próximo processo da lista circular
    int idProcesso = listaCircular_SO_2[indiceAtual].id_processo;

    // Remove o processo atual da lista
    listaCircular_SO_2.erase(listaCircular_SO_2.begin() + indiceAtual);

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
    if(op  == 4){
        gerar_lista_similiaridade();
    }
    else{
        gerarLista();
    }

    std::cout << "-------------1-----------" << std::endl;
    for (const auto& so : listaCircular_SO_2) {
        std::cout << "ID do Processo: " << so.id_processo << std::endl;
        std::cout << "Ciclo de Vida: " << so.ciclo_de_vida << std::endl;
        std::cout << "Prioridade: " << so.prioridade << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
    std::cout << "------------@------------" << std::endl;

    
    using namespace std::chrono;

    int ret = 0;

    switch (op)
    {
    case 1:
        ret = pthread_create(&thread_SO, nullptr, FCFS, nullptr);
        break;

    case 2:
        ret = pthread_create(&thread_SO, nullptr, First_Remain_Job_First, nullptr);
        break;
    case 3:
        ret = pthread_create(&thread_SO, nullptr, Prioridade, nullptr);
        break;
    case 4:
        ret = pthread_create(&thread_SO, nullptr, Similiaridade, nullptr);
        break;
    default:
        return 1;
        break;
    }

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
