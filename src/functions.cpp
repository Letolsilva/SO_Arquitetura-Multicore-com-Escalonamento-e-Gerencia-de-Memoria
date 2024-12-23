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
    (void)arg;
    int *registradores = new int[8](); // Registradores do processo

    while (true)
    {
        // Obter o próximo processo da lista circular
        int idProcesso = obterProximoProcesso();
        if (idProcesso == -1)
        {
            usleep(1000); // Aguarde se não houver processos
            continue;
        }

        // Buscar a página associada ao processo
        Page paginaAtual;
        bool encontrou = false;

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
            cout << "Processo ID=" << idProcesso << " não encontrado na memória." << endl;
            continue;
        }

        // Processar as instruções do processo
        cout << "Processando processo ID=" << idProcesso << endl;

        PCB processoAtual = paginaAtual.pcb;
        int quantumInicial = processoAtual.quantum;
       
        for (const auto &instrucao : processoAtual.instrucoes) {
            try {
                UnidadeControle(registradores, instrucao, processoAtual.quantum);               
            } catch (const runtime_error &e) {
                cout << "Quantum esgotado para o processo ID=" << idProcesso << ". Interrompendo execução." << endl;
                break;
            }
        }
        cout << "Final da Pipeline: Quantum = " << processoAtual.quantum << endl;
        processoAtual.timestamp += (quantumInicial - processoAtual.quantum); 
        cout << " timestamp do processo: " <<processoAtual.timestamp << endl;
        usleep(1000); // Simular tempo de execução do processo

        // Verifica se a memória está vazia
        if (memoryPages.empty())
        {
            cout << "Todos os processos foram executados. Encerrando o programa." << endl;
            break; // Sai do loop, encerrando a execução do processo
        }
    }

    delete[] registradores;
    pthread_exit(nullptr);
}
// colocar vetor para simbolizar a ordem de quem ja foi usado , michel deu o nome de SO
// pipeline é uma barreira

// gerar um output, timestamp 