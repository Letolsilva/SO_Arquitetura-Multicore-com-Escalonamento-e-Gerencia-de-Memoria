// #include "functions.hpp"
// #include "include.hpp"

// int PC = 0;
// int CLOCK = 0;
// bool perifericos[NUM_PERIFERICOS] = {false};
// vector<int> principal;

// int main()
// {

//     string nomeArquivo = "data/processo1.data";
//     int *registradores = (int *)malloc(32 * sizeof(int));
//     LerInstrucoesDoArquivo(nomeArquivo, registradores);

//     cout << endl
//     << "PC: " << PC << " " << "CLOCK: " << CLOCK << endl
//     << endl;

//     // Processo processo = criarProcesso(5, 1); // teste

//     return 0;
// }
#include "functions.hpp"
#include "include.hpp"

int PC = 0;
int CLOCK = 0;
bool perifericos[NUM_PERIFERICOS] = {false};
vector<int> principal;

int main()
{
    //vector<PCB> processos; // Vetor para armazenar os processos
    string diretorio = "data"; // Diretório com os arquivos .data

    // Carregar todos os processos
    carregarProcessos(diretorio, processos);

    // Exibir informações dos processos carregados
    for (const auto &pcb : processos) {
        cout << "Processo ID: " << pcb.id << endl;
        cout << "Arquivo: " << pcb.nomeArquivo << endl;
        cout << "Quantum: " << pcb.quantum << endl;
        cout << "Timestamp: " << pcb.timestamp << endl;
        cout << "Instruções: " << endl;
        for (const auto &instrucao : pcb.instrucoes) {
            cout << "  " << instrucao << endl;
        }
        cout << endl;
    }

    vector<pthread_t> threads(processos.size());
    vector<int> ids(processos.size());
    pthread_cond_init(&semaforo, nullptr);

    for (size_t i = 0; i < processos.size(); ++i) {
        ids[i] = i; // Armazena o ID para passar como argumento
        pthread_create(&threads[i], nullptr, processarProcesso, &ids[i]);
    }

    // Aguardar todas as threads terminarem
    for (size_t i = 0; i < threads.size(); ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_cond_destroy(&semaforo);




    return 0;
}
