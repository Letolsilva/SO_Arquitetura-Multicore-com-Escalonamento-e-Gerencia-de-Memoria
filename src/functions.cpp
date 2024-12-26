#include "unidadeControle.hpp"
#include "memoria.hpp"
#include "functions.hpp"
#include "SO.hpp"

// Definir as variáveis globais
vector<PCB> memoria;
vector<Page> memoryPages; // Nossa memoria
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

    int coreIndex = *(int*)arg;
    delete (int*)arg; // Liberar a memória alocada

    int cpu = sched_getcpu();

    int *registradores = new int[8](); // Registradores do processo
    int var = 0;
    
    while (true)
    {
        int idProcesso = obterProximoProcesso();
        cout << "Thread_CPU" << coreIndex << " processando processo ID=" << idProcesso << " no núcleo " << cpu << endl;

        if (idProcesso == -1)
        {
            usleep(1000); // Aguarde se não houver processos
            continue;
        }

        // Buscar a página associada ao processo
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
                    memoryPages.erase(it); // Remove o processo da memória assim que for encontrado
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
                // Processar instrução e capturar resultado
                UnidadeControle(registradores, instrucao, processoAtual.quantum, processoAtual);
            }
            catch (const runtime_error &e)
            {
                cout << "Quantum esgotado para o processo ID=" << idProcesso << ". Interrompendo execução." << endl;
                break;
            }
        }

        cout << "Processando processo ID=" << idProcesso << endl;
        stringstream ss;
        ss << "=== Processo ID: " << idProcesso << " ===" << endl;
        ss << "Quantum Inicial: " << quantumInicial << endl;
        ss << "Timestamp Inicial: " << processoAtual.timestamp << endl;
        ss << "Instruções:" << endl;

        for (const auto &instrucao : processoAtual.instrucoes)
        {
            ss << "  - " << instrucao << endl;
        }

        cout << "Final da Pipeline: Quantum = " << processoAtual.quantum << endl;
        var += (quantumInicial - processoAtual.quantum);
        processoAtual.timestamp = var; 

        cout << " timestamp do processo: " <<processoAtual.timestamp << endl;
        usleep(1000); // Simular tempo de execução do processoo9a

        ss << "Resultado: " << processoAtual.resultado << endl;
        ss << "Quantum Final: " << processoAtual.quantum << endl;
        ss << "Timestamp Final: " << processoAtual.timestamp << endl;
        ss << "=============================" << endl;
        cout << ss.str();
        salvarNoArquivo(ss.str());

        // Verifica se a memória está vazia
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

