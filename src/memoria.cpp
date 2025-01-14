#include "memoria.hpp"
#include "SO.hpp"
#include "functions.hpp"

mutex mutexMemoria;

unordered_map<string, int> temposExecucao = {
    {"+", 5}, // Soma
    {"*", 5}, // Multiplicação
    {"-", 5}, // Subtração
    {"/", 5}, // Divisão
    {"=", 2}, // Igual
    {"&", 2}, // Leitura
    {"?", 3}, // Condicional (<, >, =, !=)
    {"@", 5}  // Loop (o cálculo de tempo adicional depende de (info3 - 1) e será feito dinamicamente)
};

void imprimirMemoria()
{
    cout << "========== Conteúdo da Memória ==========" << endl;
    for (size_t i = 0; i < memoryPages.size(); ++i)
    {
        const Page &pagina = memoryPages[i];
        const PCB &pcb = pagina.pcb;

        cout << "Página: " << i << endl;
        cout << "  Base: " << pagina.base << endl;
        cout << "  Limite: " << pagina.limit << endl;
        cout << "  PCB ID: " << pcb.id << endl;
        cout << "  Nome do Arquivo: " << pcb.nomeArquivo << endl;
        cout << "  Quantum: " << pcb.quantum << endl;
        cout << "  Timestamp: " << pcb.timestamp << endl;
        cout << "  Estado: " << (pcb.estado == PRONTO ? "Pronto" : "Outro") << endl;
        cout << "  Prioridade: " << pcb.prioridade << endl;
        cout << "  Registradores: ";
        for (const auto &reg : pcb.registradores)
        {
            cout << reg << " ";
        }
        cout << endl;
        cout << "  Instruções: " << endl;
        for (size_t j = 0; j < pcb.instrucoes.size(); ++j)
        {
            cout << "    [" << j << "] " << pcb.instrucoes[j] << endl;
        }
        cout << "-----------------------------------------" << endl;
    }
    cout << "=========================================" << endl;
}

int getTempoExecucao(const string &chave)
{
    auto it = temposExecucao.find(chave);
    if (it != temposExecucao.end())
    {
        return it->second; // Retorna o valor associado à chave
    }
    else
    {
        cerr << "Erro: chave \"" << chave << "\" não encontrada no mapa." << endl;
        return -1; // Código de erro
    }
}

void salvarNaMemoria(PCB *processo)
{

    lock_guard<mutex> lock(mutexMemoria);
    {
        for (auto it = memoryPages.begin(); it != memoryPages.end(); ++it)
        {
            if (it->pcb.id == processo->id)
            {
                it->pcb.registradores = processo->registradores;
                it->pcb.ciclo_de_vida = processo->ciclo_de_vida;
                if (static_cast<int>(processo->historico_quantum.size()) != 0)
                {
                    it->pcb.historico_quantum = processo->historico_quantum;
                }
                it->pcb.resultado = processo->resultado;
                it->pcb.quantum = processo->quantum;
                it->pcb.estado = processo->estado;
                it->pcb.pc = processo->pc;

                break;
            }
        }
    }
}

void carregarProcessosNaMemoria()
{
    int idAtual = 1;
    int baseAtual = 0, limiteAtual = 1;

    string linha, instrucao;
    string diretorio = "data";
    int info1, info2, info3;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(20, 50);

    for (const auto &entry : fs::directory_iterator(diretorio))
    {
        if (entry.path().extension() == ".data")
        {
            PCB pcb;
            pcb.id = idAtual++;
            pcb.nomeArquivo = entry.path().string();
            pcb.quantum = dist(gen);
            pcb.historico_quantum.push_back(pcb.quantum);
            pcb.timestamp = CLOCK;
            pcb.estado = PRONTO;
            pcb.baseMemoria = baseAtual++;
            pcb.pc = 0;
            pcb.ciclo_de_vida = 0;
            pcb.limiteMemoria = limiteAtual++;
            pcb.prioridade = dist(gen) % 10;
            pcb.registradores.resize(8, 0);

            ifstream arquivo(pcb.nomeArquivo);

            while (getline(arquivo, linha))
            {
                pcb.instrucoes.push_back(linha);

                if (op == 2)
                {
                    stringstream ss(linha);
                    ss >> instrucao >> info1 >> info2 >> info3;

                    auto it = temposExecucao.find(instrucao);
                    if (it != temposExecucao.end())
                    {
                        pcb.ciclo_de_vida += it->second;
                    }
                    else
                    {
                        cout << "Instrucao: " << instrucao << " não encontrada no map." << endl;
                    }

                    if (instrucao == "@")
                    {
                        int tempoAdicional = (info3 - 1) + temposExecucao["@"];
                        pcb.ciclo_de_vida += tempoAdicional;
                    }
                }
                else
                {
                    pcb.ciclo_de_vida = 0;
                }
                pcb.ciclo_de_vida_inicial = pcb.ciclo_de_vida;
            }

            arquivo.close();

            Page nova_pagina_memoria;
            nova_pagina_memoria.base = memoryPages.size();
            nova_pagina_memoria.limit = memoryPages.size() + 1;
            nova_pagina_memoria.pcb = pcb;

            memoryPages.push_back(nova_pagina_memoria);
        }
    }
}

void *threadCarregarProcessos(void *arg)
{
    (void)arg;
    carregarProcessosNaMemoria();
    return nullptr;
}

int povoando_Memoria(pthread_t &thread_memoria)
{

    // Thread_Memoria = Carregar todos os processos, paginando na memoria, lendo os inputs e colocando em wait, e o ret é apenas para verificação...
    int ret = pthread_create(&thread_memoria, nullptr, threadCarregarProcessos, nullptr);

    if (ret != 0)
    {
        cerr << "Erro ao criar a thread da memoria!" << endl;
        return 1;
    }

    // A thread principal aguarda a thread de carregamento terminar
    // pthread_join(thread_memoria, nullptr);
    return 0;
}
