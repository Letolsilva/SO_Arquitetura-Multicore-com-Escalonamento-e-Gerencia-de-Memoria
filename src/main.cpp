#include "functions.hpp"
#include "memoria.hpp"
#include "include.hpp"
#include "SO.hpp"

int PC = 0;
int CLOCK = 0;
string diretorio = "data";
bool perifericos[NUM_PERIFERICOS] = {false};
vector<int> principal;
sem_t semaforoCores; // Definição do semáforo
vector<mutex> mutexCores(NUM_CORE);
vector<int> processosNaMemoria;
int main()
{
    // Bootloader ------------------------------------ //
    pthread_t thread_memoria = {};
    pthread_t thread_so = {};
    pthread_t thread_cpu[NUM_CORE]; // Criar múltiplas threads de CPU
    // ------------------------------------------------ //

    int status_memoria = povoando_Memoria(thread_memoria, diretorio);
    pthread_join(thread_memoria, nullptr);

    // Inicializa o SO com os processos carregados
    int status_so = iniciando_SO(thread_so, processosNaMemoria);

    if (status_memoria == 0 && status_so == 0)
    {
        printProcessos();
        imprimirListaCircular_SO(); // Confirmação dos processos carregados no SO

        // Criar threads para CPU (multicore)
        for (int i = 0; i < NUM_CORE; ++i)
        {
            int status_cpu = pthread_create(&thread_cpu[i], nullptr, processarProcesso, nullptr);
            if (status_cpu != 0)
            {
                cerr << "Erro ao criar a thread da CPU!" << endl;
                return 1;
            }
        }

        // Aguardar o término das threads da CPU
        for (int i = 0; i < NUM_CORE; ++i)
        {
            pthread_join(thread_cpu[i], nullptr);
        }
    }

    pthread_join(thread_so, nullptr); // Aguardar o término da thread do SO
    return 0;
}