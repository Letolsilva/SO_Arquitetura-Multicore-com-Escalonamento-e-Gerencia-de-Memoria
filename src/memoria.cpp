#include "memoria.hpp"
#include "SO.hpp"
#include "functions.hpp"

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

void carregarProcessosNaMemoria(const string &diretorio)
{
    int idAtual = 1;
    int baseAtual = 0, limiteAtual = 1;

    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(20, 50);

    for (const auto &entry : fs::directory_iterator(diretorio))
    {
        if (entry.path().extension() == ".data")
        {
            cout <<  entry.path().string() << endl;
            PCB pcb;
            pcb.id = idAtual++;
            pcb.nomeArquivo = entry.path().string();
            pcb.quantum = dist(gen);
            pcb.timestamp = CLOCK;
            pcb.estado = PRONTO;
            pcb.baseMemoria = baseAtual++;
            pcb.limiteMemoria = limiteAtual++;
            pcb.prioridade = dist(gen) % 10; // Define prioridade aleatória
            pcb.registradores.resize(8, 0);

            ifstream arquivo(pcb.nomeArquivo);
            string linha;
            while (getline(arquivo, linha))
            {
                pcb.instrucoes.push_back(linha);
            }
            arquivo.close();

            Page nova_pagina_memoria;
            nova_pagina_memoria.base = memoryPages.size();
            nova_pagina_memoria.limit = memoryPages.size() + 1;
            nova_pagina_memoria.pcb = pcb;

            memoryPages.push_back(nova_pagina_memoria);
            atualizarListaCircular(pcb.id);
        }
    }
}

void *threadCarregarProcessos(void *arg)
{
    string diretorio = *static_cast<string *>(arg);
    carregarProcessosNaMemoria(diretorio);
    return nullptr;
}

int povoando_Memoria(pthread_t &thread_memoria, string diretorio)
{

    // Thread_Memoria = Carregar todos os processos, paginando na memoria, lendo os inputs e colocando em wait, e o ret é apenas para verificação...
    int ret = pthread_create(&thread_memoria, nullptr, threadCarregarProcessos, &diretorio);

    if (ret != 0)
    {
        cerr << "Erro ao criar a thread da memoria!" << endl;
        return 1;
    }

    // A thread principal aguarda a thread de carregamento terminar
    // pthread_join(thread_memoria, nullptr);
    return 0;
}
