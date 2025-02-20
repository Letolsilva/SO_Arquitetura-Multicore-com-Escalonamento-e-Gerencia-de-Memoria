#include "unidadeControle.hpp"
#include "monitora.hpp"
#include "memoria.hpp"
#include "functions.hpp"
#include "SO.hpp"

vector<PCB> memoria;
vector<Page> memoryPages; // Memoria
mutex mutexProcessos;     // Mutex para controle de acesso ao vetor
mutex output;

string gerardor_Chave(char instrucao, int info1, int info2){
    string chave ;

    if(instrucao == '+' || instrucao == '*')
    {
        if(info1 > info2){
            chave = to_string(info1) + "." + to_string(instrucao) + "." + to_string(info2);
        }
        else{
            chave =  to_string(info2) + "." + to_string(instrucao) + "."  + to_string(info1);
        }
    }
    else
    {
        chave = to_string(info1) + "." + to_string(instrucao) + "."  + to_string(info2);
    }

    return chave;

}

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
        int idProcesso = -1;
        if(op  != 5){
            idProcesso = obterProximoProcesso();
        }
        else{
            idProcesso = obterProximoProcesso_pelo_enderecoVirtual() +1;
            //cout << "\n\t id Processo: " << idProcesso << endl;
        }
        
        int timestamp_inicial = 0;

        if (idProcesso == -1)
        {
            cout << "Nenhum processo disponível, thread encerrando." << endl;
            break;
        }

        timestamp_inicial = var;

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
            pthread_exit(nullptr);
            break; // Se não encontrar o processo, termina o loop
        }

        if (processoAtual.estado != BLOQUEADO)
        {
            lock_guard<mutex> lock(mutexProcessos);
            processoAtual.timestamp_inicial = timestamp_inicial;
            for (auto it = memoryPages.begin(); it != memoryPages.end(); ++it)
            {
                if (it->pcb.id == processoAtual.id)
                {
                    PCB pcbAtual = it->pcb;
                    it->pcb.timestamp_inicial = processoAtual.timestamp_inicial;
                    break;
                }
            }
        }

        if (processoAtual.quantum == 0 && processoAtual.estado == BLOQUEADO)
        {
            lock_guard<mutex> lock(mutexProcessos);
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> dist(20, 50);
            processoAtual.quantum = dist(gen);
            if (processoAtual.prioridade != 0)
            {
                processoAtual.prioridade--;
            }

            processoAtual.historico_quantum.push_back(processoAtual.quantum);
            processoAtual.estado = PRONTO;
            atualizarEstadoProcesso(processoAtual.id, "PRONTO");
            salvarNaMemoria(&processoAtual);
            
            if(op!=5){
                add_ListaCircular(processoAtual);

            }else{
                add_vetor_endereco_virtual(processoAtual);
            }
        }

        int quantumInicial = processoAtual.quantum;

        stringstream ss;
        ss << "Thread_CPU" << coreIndex << " processando processo ID=" << processoAtual.id << endl;
        ss << "Estado: " << obterEstadoProcesso(processoAtual) << endl;

        // cout << "Thread_CPU" << coreIndex << " processando processo ID=" << processoAtual.id << endl;
        // cout << "Estado: " << obterEstadoProcesso(processoAtual) << endl;

        // Executar as instruções do processo
        processarInstrucoes(processoAtual);

        var += abs(quantumInicial - processoAtual.quantum);
        processoAtual.timestamp += var;

        if (processoAtual.estado == TERMINADO)
        {
            
            lock_guard<mutex> lock(mutexProcessos);
            for (auto it = memoryPages.begin(); it != memoryPages.end(); ++it)
            {
                if (it->pcb.id == processoAtual.id)
                {
                    PCB pcbAtual = it->pcb;
                    atualizarESalvarProcesso(it->pcb, ss, quantumInicial, var);
                    memoryPages.erase(it);
                    break;
                }
            }
        }
        usleep(1000);

        if (listaCircular_SO_2.empty())
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
    if (processo.estado == TERMINADO)
        return "TERMINADO";
    return "EXECUTANDO";
}

//
void *monitorQuantum(void *args)
{
    ThreadArgs *threadArgs = static_cast<ThreadArgs *>(args);
    string linha;
    char instrucao;

    // Acessar os argumentos
    PCB *processoAtual = threadArgs->processoAtual;

    if (processoAtual == nullptr)
    {
        cerr << "Erro: 'processoAtual' é nulo na função monitorQuantum." << endl;
        pthread_exit(nullptr);
    }

    processoAtual->estado = EXECUTANDO;
    atualizarEstadoProcesso(processoAtual->id, "EXECUTANDO");

    while (processoAtual->quantum > 0 && processoAtual->estado != BLOQUEADO && static_cast<int>(processoAtual->instrucoes.size()) > processoAtual->pc)
    {
        UnidadeControle(processoAtual->registradores.data(), processoAtual->instrucoes[processoAtual->pc], processoAtual->quantum, *processoAtual);
        if (op == 2)
        {
            int x = processoAtual->pc - 1;
            linha = processoAtual->instrucoes[x];
            stringstream ss(linha);
            ss >> instrucao;
            x = getTempoExecucao(string(1, instrucao));
            processoAtual->ciclo_de_vida = processoAtual->ciclo_de_vida - x;
        }
    }

    // Aqui faz a verificacao do job está bloqueado para colocar na lista novamente
    if (processoAtual->quantum <= 0 && processoAtual->estado == BLOQUEADO)
    {
        lock_guard<mutex> lock(mutexProcessos);
        if (static_cast<int>(processoAtual->instrucoes.size()) > processoAtual->pc)
        {
            stringstream ss;
            ss << "Ocorreu Preempção no processo: " << processoAtual->id << endl;
            salvarNoArquivo(ss.str());
        }
        processoAtual->quantum = 0;
        //adicionar aqui para a fila de endereco virtual -------------------------------------------
       // add_ListaCircular(*processoAtual);

        if(op!=5){
            add_ListaCircular(*processoAtual);

        }else{
            add_vetor_endereco_virtual(*processoAtual);
        }

        atualizarEstadoProcesso(processoAtual->id, "BLOQUEADO");
        salvarNaMemoria(processoAtual);
        pthread_exit(nullptr);
    }
    else
    {
        // Aqui conclui o Job
        Page paginaAtual;
        lock_guard<mutex> lock(mutexProcessos);
        {
            processoAtual->estado = TERMINADO;
            atualizarEstadoProcesso(processoAtual->id, "TERMINADO");
            salvarNaMemoria(processoAtual);
            remover_ListaCircular(processoAtual->id);
        }
    }

    pthread_exit(nullptr);
}

// Função que processa as instruções do processo
void processarInstrucoes(PCB &processoAtual)
{
    try
    {
        ThreadArgs args = {&processoAtual};
        // cout << "\n Abrindo thread id= " << processoAtual.id << " quantum= " << processoAtual.quantum << " estado= " << processoAtual.estado << endl;
        pthread_t monitorThread = {};
        int status_monitor = pthread_create(&monitorThread, nullptr, monitorQuantum, &args);
        if (status_monitor != 0)
        {
            cerr << "Erro ao criar a thread do monitor!" << endl;
        }

        // Aguarda a finalização da thread
        pthread_join(monitorThread, nullptr);
    }
    catch (const runtime_error &e)
    {
        cout << "Erro durante execução do processo ID=" << processoAtual.id << endl;
        processoAtual.estado = BLOQUEADO;
        atualizarEstadoProcesso(processoAtual.id, "BLOQUEADO");
        usleep(1000000);
        //add_ListaCircular(processoAtual);

        if(op!=5){
            add_ListaCircular(processoAtual);

        }else{
            add_vetor_endereco_virtual(processoAtual);
        }
    }
}

// Função que atualiza o processo e salva as informações no arquivo
void atualizarESalvarProcesso(PCB &processoAtual, stringstream &ss, int &quantumInicial, int &timestamp_final)
{
    lock_guard<mutex> lock(output);
    {
        ss << "=== Processo ID: " << processoAtual.id << " ===" << endl;
        ss << "Nome Arquivo: " << processoAtual.nomeArquivo << endl;
        ss << "Base Memoria: " << processoAtual.baseMemoria << endl;
        ss << "Limite Memoria: " << processoAtual.limiteMemoria << endl;
        ss << "Quantum Inicial: " << quantumInicial << endl;
        ss << "Quantum Historico: ";
        for (const auto &x : processoAtual.historico_quantum)
        {
            ss << x << " - ";
        }
        ss << endl;
        ss << "Timestamp Inicial: " << processoAtual.timestamp_inicial << endl;
        ss << "Instruções:" << endl;

        for (const auto &instrucao : processoAtual.instrucoes)
        {
            ss << "  - " << instrucao << endl;
        }

        // Finaliza a execução do processo
        ss << "Resultado: " << processoAtual.resultado << endl;
        ss << "Quantum Final: " << processoAtual.quantum << endl;
        ss << "Timestamp Final: " << timestamp_final << endl;
        ss << "PC: " << processoAtual.pc << endl;
        ss << "Ciclo de Vida Inicial: " << processoAtual.ciclo_de_vida_inicial << endl;
        ss << "Ciclo de Vida: " << processoAtual.ciclo_de_vida << endl;
        ss << "Prioridade: " << processoAtual.prioridade << endl;
        ss << "Estado Final: " << obterEstadoProcesso(processoAtual) << endl;
        ss << "CLOCK: " << CLOCK << endl;
        ss << "=============================" << endl;
        salvarNoArquivo(ss.str());
    }
}