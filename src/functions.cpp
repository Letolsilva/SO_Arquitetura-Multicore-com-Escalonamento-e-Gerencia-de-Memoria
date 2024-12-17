#include "unidadeControle.hpp"
#include "functions.hpp"

// Definir as variáveis globais
vector<PCB> processos;       // Vetor compartilhado de processos
std::mutex mutexProcessos;   // Mutex para controle de acesso ao vetor

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

void carregarProcessos(const string &diretorio, vector<PCB> &processos)
{
    int idAtual = 1; // ID inicial para os processos

    for (const auto &entry : fs::directory_iterator(diretorio))
    {
        if (entry.path().extension() == ".data")
        {
            PCB pcb;
            pcb.id = idAtual++;
            pcb.nomeArquivo = entry.path().string();
            pcb.quantum = 10;
            pcb.timestamp = CLOCK;

            ifstream arquivo(pcb.nomeArquivo);
            string linha;
            while (getline(arquivo, linha))
            {
                pcb.instrucoes.push_back(linha);
            }
            arquivo.close();

            processos.push_back(pcb);
        }
    }
}

void *processarProcesso(void *arg)
{
    int idProcesso = *static_cast<int *>(arg);

    PCB processo;
    {
        std::lock_guard<std::mutex> lock(mutexProcessos);
        processo = processos[idProcesso];
    }

    sem_wait(&semaforoCores);

    int coreId = idProcesso % NUM_CORE; // Distribui os processos para os núcleos
    {
        std::lock_guard<std::mutex> lock(mutexCores[coreId]);

        cout << "Thread processando: ID=" << processo.id
             << " no Core=" << coreId
             << " Arquivo=" << processo.nomeArquivo << endl;

        for (const auto &instrucao : processo.instrucoes)
        {
            cout << "Core=" << coreId << " ID=" << processo.id
                 << " -> " << instrucao << endl;
            usleep(1000); // Simula o tempo de execução
        }
    }

    // Libera o semáforo
    sem_post(&semaforoCores);
    pthread_exit(nullptr);
}
