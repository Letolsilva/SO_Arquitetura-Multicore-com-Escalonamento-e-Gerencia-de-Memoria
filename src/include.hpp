#ifndef INCLUDE_HPP
#define INCLUDE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <unistd.h>
#include <unordered_map>
#include <vector>
#include <filesystem>
#include <pthread.h>
#include <mutex>
#include <chrono>
#include <semaphore.h>
#include <sched.h>
#include <random>
#include <algorithm>
#include <optional>
#include <set>
#include <unordered_set>

#define NUM_PERIFERICOS 5
#define TAM_CACHE 6
#define NUM_CORE 2

using namespace std;
using namespace std::chrono;
namespace fs = filesystem;

enum EstadoProcesso
{
    PRONTO,
    BLOQUEADO,
    EXECUTANDO,
    TERMINADO
};


extern int PC;
extern int CLOCK;
extern int op;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;
extern vector<mutex> mutexCores;

extern bool perifericos[NUM_PERIFERICOS];

struct PCB
{
    int id;        // ID do processo
    int quantum;   // Quantum inicial
    int timestamp; // Timestamp inicial
    int timestamp_inicial;
    int prioridade;  // Prioridade do processo
    int baseMemoria; // Endereço base de memória
    int limiteMemoria;
    int resultado; // Limite de alocação de memória
    int ciclo_de_vida;
    int ciclo_de_vida_inicial;
    int pc;
    vector<int> registradores; // Banco de registradores
    vector<string> conjunto_chaves;
    EstadoProcesso estado;     // Estado atual do processo
    string nomeArquivo;        // Nome do arquivo associado
    vector<string> instrucoes; // Instruções do processo
    vector<int> historico_quantum;
};

struct Page
{
    int base;         // Endereço base
    int limit;        // Limite de memória
    PCB pcb;          // Controle do processo
    pthread_t thread; // Thread associada ao processo
};

struct SO
{
    int id_processo;
    int ciclo_de_vida;
    int prioridade;
    vector<string> conjunto_chaves;
};

//----Memoria
extern vector<Page> memoryPages;
extern unordered_map<string,int> memoriaCache;
extern int currentPageIndex;
extern vector<PCB> memoria; //?
extern mutex mutexProcessos;
extern mutex mutexMemoria;
extern mutex output;

//----SO
extern vector<int> listaCircular_SO;
extern vector<SO> listaCircular_SO_2;
extern size_t indiceAtual;
extern mutex mutexListaCircular;
extern unordered_map<int, string> estadosProcessos;

//----Hash para tempos de execução das operações (para SJF)
extern unordered_map<string, int> temposExecucao;

struct ThreadArgs
{
    PCB *processoAtual;
};

#endif
