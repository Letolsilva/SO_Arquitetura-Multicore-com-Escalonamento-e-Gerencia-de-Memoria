#include "memoria.hpp"
#include "SO.hpp"

void carregarProcessosNaMemoria(const string &diretorio)
{
    int idAtual = 1;

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
    string diretorio = *static_cast<string *>(arg); // Recupera o argumento passado para a thread
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

void printProcessos()
{

    // Exibir informações dos processos carregados
    for (const auto &page : memoryPages)
    {
        const PCB &pcb = page.pcb;
        cout << "Processo ID: " << pcb.id << endl;
        cout << "Arquivo: " << pcb.nomeArquivo << endl;
        cout << "Quantum: " << pcb.quantum << endl;
        cout << "Timestamp: " << pcb.timestamp << endl;
        cout << "Instruções: " << endl;

        for (const string &instrucao : pcb.instrucoes)
        {
            cout << "  " << instrucao << endl;
        }
        cout << "-----------------------------" << endl;
    }
}