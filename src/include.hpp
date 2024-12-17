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

#define NUM_PERIFERICOS 5
#define NUM_CORE 2

using namespace std;
namespace fs = std::filesystem;

extern int PC;
extern int CLOCK;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;
extern vector<mutex> mutexCores;
extern sem_t semaforoCores; // Semáforo para limitar threads simultâneas

extern bool perifericos[NUM_PERIFERICOS];

// Estruturas de dados
struct Processo {
    int quantum;
    int timestamp;
    int id; // Identificador do processo (se necessário)
};

struct PCB {
    int id;                     // ID do processo
    int quantum;                // Quantum inicial
    int timestamp;              // Timestamp inicial
    string nomeArquivo;         // Nome do arquivo associado
    vector<string> instrucoes;  // Instruções do processo
};

// Declarações de variáveis globais
extern vector<PCB> processos;
extern std::mutex mutexProcessos;

#endif
