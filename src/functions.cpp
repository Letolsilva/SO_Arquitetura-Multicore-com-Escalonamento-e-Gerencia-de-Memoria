#include "unidadeControle.hpp"
#include "monitora.hpp"
#include "memoria.hpp"
#include "functions.hpp"
#include "SO.hpp"

vector<PCB> memoria;
vector<Page> memoryPages; // Memoria
mutex mutexProcessos;     // Mutex para controle de acesso ao vetor

// Função auxiliar para salvar no arquivo
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

// Função que processa um processo
void *processarProcesso(void *arg)
{
    auto coreIndexPtr = std::unique_ptr<int>(reinterpret_cast<int *>(arg)); // Ponteiro inteligente

    int coreIndex = *coreIndexPtr;

    auto registradores = std::make_unique<int[]>(8);
    int var = 0;

    while (true)
    {
        int idProcesso = obterProximoProcesso();

        if (idProcesso == -1)
        {
            usleep(1000);
            continue;
        }

        Page paginaAtual;
        PCB processoAtual;

        // Buscar o processo na memória
        if (!buscarProcessoNaMemoria(idProcesso, paginaAtual, processoAtual))
        {
            break; // Se não encontrar o processo, termina o loop
        }

        processoAtual.estado = EXECUTANDO;
        atualizarEstadoProcesso(processoAtual.id, "EXECUTANDO");
        int quantumInicial = processoAtual.quantum; // 0

        stringstream ss;
        ss << "Thread_CPU" << coreIndex << " processando processo ID=" << processoAtual.id << endl;
        ss << "Estado: " << obterEstadoProcesso(processoAtual) << endl;

        int timestamp_inicial = var;

        // Executar as instruções do processo
        processarInstrucoes(processoAtual, ss);

        var += (quantumInicial - processoAtual.quantum);
        processoAtual.timestamp += var;
        // Atualizar timestamp e salvar no arquivo
        atualizarESalvarProcesso(processoAtual, ss, quantumInicial, timestamp_inicial);

        usleep(1000);

        if (memoryPages.empty())
        {
            break;
        }
    }

    pthread_exit(nullptr);
}

// Função que busca o processo na memória
bool buscarProcessoNaMemoria(int idProcesso, Page &paginaAtual, PCB &processoAtual)
{
    bool encontrou = false;

    lock_guard<mutex> lock(mutexProcessos);
    for (auto it = memoryPages.begin(); it != memoryPages.end(); ++it)
    {
        if (it->pcb.id == idProcesso)
        {
            paginaAtual = *it;
            memoryPages.erase(it);
            encontrou = true;
            processoAtual = paginaAtual.pcb;
            break;
        }
    }

    return encontrou;
}

// Função que retorna o estado do processo em formato de string
string obterEstadoProcesso(const PCB &processo)
{
    if (processo.estado == PRONTO)
        return "PRONTO";
    if (processo.estado == BLOQUEADO)
        return "BLOQUEADO";
    return "EXECUTANDO";
}

// Função que processa as instruções do processo
void processarInstrucoes(PCB &processoAtual, stringstream &ss)
{
    for (const auto &instrucao : processoAtual.instrucoes)
    {
        if (processoAtual.quantum <= 0)
        {
            ss << "Quantum esgotado para o processo ID=" << processoAtual.id << ". Preempcao ocorrida." << endl;
            processoAtual.estado = BLOQUEADO;
            atualizarEstadoProcesso(processoAtual.id, "BLOQUEADO");
            atualizarListaCircular(processoAtual.id);
            break;
        }
        try
        {
            processoAtual.estado = EXECUTANDO;
            atualizarEstadoProcesso(processoAtual.id, "EXECUTANDO");
            UnidadeControle(processoAtual.registradores.data(), instrucao, processoAtual.quantum, processoAtual);
            atualizarListaCircular(processoAtual.id);
            processoAtual.estado = PRONTO;
            atualizarEstadoProcesso(processoAtual.id, "PRONTO");

        }
        catch (const runtime_error &e)
        {
            cout << "Erro durante execução do processo ID=" << processoAtual.id << endl;
            processoAtual.estado = BLOQUEADO;
            atualizarEstadoProcesso(processoAtual.id, "BLOQUEADO");
            usleep(1000000);
            atualizarListaCircular(processoAtual.id);
            break;
        }
    }
}

// Função que atualiza o processo e salva as informações no arquivo
void atualizarESalvarProcesso(PCB &processoAtual, stringstream &ss, int &quantumInicial, int &var)
{
    ss << "=== Processo ID: " << processoAtual.id << " ===" << endl;
    ss << "Nome Arquivo: " << processoAtual.nomeArquivo << endl;
    ss << "Base Memoria: " << processoAtual.baseMemoria << endl;
    ss << "Limite Memoria: " << processoAtual.limiteMemoria << endl;
    ss << "Quantum Inicial: " << quantumInicial << endl;
    ss << "Timestamp Inicial: " << var << endl;
    ss << "Instruções:" << endl;

    for (const auto &instrucao : processoAtual.instrucoes)
    {
        ss << "  - " << instrucao << endl;
    }

    // Finaliza a execução do processo
    ss << "Resultado: " << processoAtual.resultado << endl;
    ss << "Quantum Final: " << processoAtual.quantum << endl;
    ss << "Timestamp Final: " << processoAtual.timestamp << endl;
    ss << "Prioridade: " << processoAtual.prioridade << endl;
    ss << "Estado Final: " << obterEstadoProcesso(processoAtual) << endl;
    ss << "=============================" << endl;
    // cout << ss.str();

    salvarNoArquivo(ss.str());
}
