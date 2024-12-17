#include "unidadeControle.hpp"
#include "functions.hpp"

// Definir as variáveis globais
vector<PCB> processos;       // Vetor compartilhado de processos
std::mutex mutexProcessos;   // Mutex para controle de acesso ao vetor
pthread_cond_t semaforo;     // Semáforo para sincronização

// Função para inicializar um processo com quantum e timestamp
Processo criarProcesso(int quantumInicial, int idProcesso)
{
    Processo p;
    p.quantum = quantumInicial;
    p.timestamp = CLOCK; // Timestamp inicial é o valor atual do CLOCK
    p.id = idProcesso;
    return p;
}

void LerInstrucoesDoArquivo(const string &nomeArquivo, int *registradores){

    // string nomeArquivo = "input.data";
    ifstream arquivo(nomeArquivo);
    string linha;

    if (!arquivo.is_open())
    {
        cout << "Erro ao abrir o arquivo!" << endl;
        return;
    }

    while (getline(arquivo, linha))
    {
        // stringstream ss(linha);
        UnidadeControle(registradores, linha);
        cout << "Clock: " << CLOCK << endl;
    }

    arquivo.close();
}

void carregarProcessos(const string &diretorio, vector<PCB> &processos) {
    int idAtual = 1; // ID inicial para os processos

    // Iterar sobre todos os arquivos no diretório
    for (const auto &entry : fs::directory_iterator(diretorio)) {
        if (entry.path().extension() == ".data") { // Verificar extensão .data
            PCB pcb;
            pcb.id = idAtual++;
            pcb.nomeArquivo = entry.path().string();
            pcb.quantum = 10; // Exemplo: quantum fixo (ajustar conforme necessário)
            pcb.timestamp = CLOCK;

            // Abrir o arquivo e ler as instruções
            ifstream arquivo(pcb.nomeArquivo);
            string linha;
            while (getline(arquivo, linha)) {
                pcb.instrucoes.push_back(linha);
            }
            arquivo.close();

            // Adicionar o PCB ao vetor de processos
            processos.push_back(pcb);
        }
    }
}

void *processarProcesso(void *arg) {
    int idProcesso = *static_cast<int *>(arg);

    // Acesso protegido ao processo
    PCB processo;
    {
        std::lock_guard<std::mutex> lock(mutexProcessos);
        processo = processos[idProcesso];
    }

    // Simula o processamento das instruções do processo
    cout << "Thread processando: ID=" << processo.id 
         << " Arquivo=" << processo.nomeArquivo << endl;

    for (const auto &instrucao : processo.instrucoes) {
        cout << "ID=" << processo.id << " -> " << instrucao << endl;
    }

    // Notifica outras threads que terminou o processamento
    pthread_cond_signal(&semaforo);
    pthread_exit(nullptr);
}