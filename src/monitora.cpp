#include "functions.hpp"
#include "monitora.hpp"
#include "memoria.hpp"
#include "include.hpp"
#include "SO.hpp"

void* start(void* arg){
    string diretorio = "data";
    vector<int>* processosNaMemoria = static_cast<vector<int>*>(arg);

    // ---------------- Bootloader ------------------//
    pthread_t thread_memoria = {};
    pthread_t thread_so = {};
    pthread_t thread_cpu[NUM_CORE];
    // ----------------------------------------------//

    ofstream arquivo("./output/output.data", ios::trunc);
    if (!arquivo.is_open()) {
        cerr << "Erro ao inicializar o arquivo output.data!" << endl;
        return nullptr;  // Retorno seguro sem erro
    }
    arquivo.close();

    int status_memoria = povoando_Memoria(thread_memoria, diretorio);
    pthread_join(thread_memoria, nullptr);

    int status_so = iniciando_SO(thread_so, *processosNaMemoria);

    if (status_memoria == 0 && status_so == 0) {
        for (int i = 0; i < NUM_CORE; ++i) {
            int* coreIndex = new int(i);  // Não é mais necessário, mas mantido para ilustração
            int status_cpu = pthread_create(&thread_cpu[i], nullptr, processarProcesso, (void*)coreIndex);
            if (status_cpu != 0) {
                cerr << "Erro ao criar a thread da CPU!" << endl;
                return nullptr;  // Saída da thread em caso de erro
            }
        }

        for (int i = 0; i < NUM_CORE; ++i) {
            pthread_join(thread_cpu[i], nullptr);
        }
    }

    pthread_join(thread_so, nullptr);
    return nullptr;  // Indica que a thread terminou
}

