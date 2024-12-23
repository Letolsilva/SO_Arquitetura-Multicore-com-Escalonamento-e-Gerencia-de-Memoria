#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <pthread.h>
#include <mutex>
#include <semaphore.h>
#include <random> 
#include <algorithm>

#define NUM_PERIFERICOS 5
#define NUM_CORE 2

using namespace std;
namespace fs = filesystem;

extern int PC;
extern int CLOCK;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;
extern vector<mutex> mutexCores;
extern sem_t semaforoCores;

extern bool perifericos[NUM_PERIFERICOS];

struct Processo
{
    int quantum;
    int timestamp;
    int id; // Identificador do processo (se necessário)
};

struct PCB
{
    int id;                    // ID do processo
    int quantum;               // Quantum inicial
    int timestamp;             // Timestamp inicial
    int tempoEspera; 
    string nomeArquivo;        // Nome do arquivo associado
    vector<string> instrucoes; // Instruções do processo
};

struct Page
{
    int base;         // Endereço base
    int limit;        // Limite de memória
    PCB pcb;          // Controle do processo
    pthread_t thread; // Thread associada ao processo
};

//----Memoria
extern vector<Page> memoryPages;
extern int currentPageIndex;
extern vector<PCB> memoria;  //?
extern mutex mutexProcessos; //?

//----SO
extern vector<int> listaCircular_SO;
extern size_t indiceAtual;
extern mutex mutexListaCircular;

#endif
