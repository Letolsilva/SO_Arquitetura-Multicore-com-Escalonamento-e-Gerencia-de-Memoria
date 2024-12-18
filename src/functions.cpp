#include "unidadeControle.hpp"
#include "memoria.hpp"
#include "functions.hpp"

// Definir as variáveis globais
vector<PCB> memoria;
vector<Page> memoryPages; //Nossa memoria
mutex mutexProcessos;   // Mutex para controle de acesso ao vetor

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
        UnidadeControle(registradores, linha);
        cout << "Clock: " << CLOCK << endl;
    }

    arquivo.close();
}

void *processarProcesso(void *arg){
    int idMemoria = *static_cast<int *>(arg);

    PCB processo;
    {
        lock_guard<mutex> lock(mutexProcessos);
        processo = memoria[idMemoria];
    }

    sem_wait(&semaforoCores);

    int coreId = idMemoria % NUM_CORE; // Distribui os processos para os núcleos
    {
        lock_guard<mutex> lock(mutexCores[coreId]);

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

// colocar vetor para simbolizar a ordem de quem ja foi usado , michel deu o nome de SO
//pipeline é uma barreira