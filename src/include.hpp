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
#include <sched.h>
#include <random>
#include <algorithm>

#define NUM_PERIFERICOS 5
#define NUM_CORE 3

using namespace std;
namespace fs = filesystem;

enum EstadoProcesso
{
    PRONTO,
    BLOQUEADO,
    EXECUTANDO
};

extern int PC;
extern int CLOCK;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;
extern vector<mutex> mutexCores;

extern bool perifericos[NUM_PERIFERICOS];

struct PCB
{
    int id;          // ID do processo
    int quantum;     // Quantum inicial
    int timestamp;   // Timestamp inicial
    int prioridade;  // Prioridade do processo
    int baseMemoria; // Endereço base de memória
    int limiteMemoria;
    int resultado;             // Limite de alocação de memória
    vector<int> registradores; // Banco de registradores
    EstadoProcesso estado;     // Estado atual do processo
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

// struct ThreadArgs {
//     pthread_t thread_memoria;
//     pthread_t thread_so;
//     pthread_t *thread_cpu;
//     vector<int> processosNaMemoria;
// };

//----Memoria
extern vector<Page> memoryPages;
extern int currentPageIndex;
extern vector<PCB> memoria;  //?
extern mutex mutexProcessos; //?

//----SO
extern vector<int> listaCircular_SO;
extern size_t indiceAtual;
extern mutex mutexListaCircular;
extern unordered_map<int, string> estadosProcessos; // Mapeia o ID do processo para seu estado


#endif
