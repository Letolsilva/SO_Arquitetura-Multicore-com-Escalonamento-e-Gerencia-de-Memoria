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
#include <condition_variable>

#define NUM_PERIFERICOS 5

using namespace std;
namespace fs = std::filesystem;

extern int PC;
extern int CLOCK;

extern unordered_map<int, int> cache;
extern vector<int> principal;
extern vector<vector<int>> disco;

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
extern pthread_cond_t semaforo;

#endif
