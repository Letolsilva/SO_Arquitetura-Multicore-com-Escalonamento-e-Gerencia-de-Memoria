#include "unidadeControle.hpp"
#include "memoria.hpp"
#include "functions.hpp"
#include "SO.hpp"

vector<PCB> memoria;
vector<Page> memoryPages; // Memoria
mutex mutexProcessos;     // Mutex para controle de acesso ao vetor

// Função para inicializar um processo com quantum e timestamp
Processo criarProcesso(int quantumInicial, int idProcesso)
{
    Processo p;
    p.quantum = quantumInicial;
    p.timestamp = CLOCK; // Timestamp inicial é o valor atual do CLOCK
    p.id = idProcesso;
    return p;
}

void *processarProcesso(void *arg)
{

    int coreIndex = *(int *)arg;
    delete (int *)arg; // Liberar a memória alocada

    int cpu = sched_getcpu();

    int *registradores = new int[8]();
    int var = 0;

    while (true)
    {
        stringstream ss;
        int idProcesso = obterProximoProcesso();
        ss << "Thread_CPU" << coreIndex << " processando processo ID=" << idProcesso << cpu << endl;

        if (idProcesso == -1)
        {
            usleep(1000);
            continue;
        }

        Page paginaAtual;
        bool encontrou = false;
        PCB processoAtual;

        {
            lock_guard<mutex> lock(mutexProcessos);
            for (auto it = memoryPages.begin(); it != memoryPages.end(); ++it)
            {
                if (it->pcb.id == idProcesso)
                {
                    paginaAtual = *it;
                    memoryPages.erase(it);
                    encontrou = true;
                    break;
                }
            }
        }

        if (!encontrou)
        {
            break;
            //  continue;
        }

        paginaAtual.pcb.timestamp = var;
        processoAtual = paginaAtual.pcb;
        int quantumInicial = processoAtual.quantum;

        for (const auto &instrucao : processoAtual.instrucoes)
        {
            try
            {
                UnidadeControle(registradores, instrucao, processoAtual.quantum, processoAtual);
            }
            catch (const runtime_error &e)
            {
                cout << "Quantum esgotado para o processo ID=" << idProcesso << ". Interrompendo execução." << endl;
                break;
            }
        }

        ss << "=== Processo ID: " << idProcesso << " ===" << endl;
        ss << "Quantum Inicial: " << quantumInicial << endl;
        ss << "Timestamp Inicial: " << processoAtual.timestamp << endl;
        ss << "Instruções:" << endl;

        for (const auto &instrucao : processoAtual.instrucoes)
        {
            ss << "  - " << instrucao << endl;
        }

        var += (quantumInicial - processoAtual.quantum);
        processoAtual.timestamp = var;
        usleep(1000);

        ss << "Resultado: " << processoAtual.resultado << endl;
        ss << "Quantum Final: " << processoAtual.quantum << endl;
        ss << "Timestamp Final: " << processoAtual.timestamp << endl;
        ss << "=============================" << endl;
        salvarNoArquivo(ss.str());

        if (memoryPages.empty())
        {
            break;
        }
    }

    delete[] registradores;
    pthread_exit(nullptr);
}

void salvarNoArquivo(const string &conteudo)
{
    filesystem::create_directories("./output");
    ofstream arquivo("./output/output.data", ios::app);
    if (arquivo.is_open())
    {
        arquivo << conteudo << endl;
        arquivo.close();
    }
    else
    {
        cerr << "Erro ao abrir o arquivo output.data!" << endl;
    }
}
