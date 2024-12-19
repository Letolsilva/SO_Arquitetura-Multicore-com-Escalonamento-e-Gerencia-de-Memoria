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
    // pthread_t thread_cpu = {};
    pthread_t thread_memoria = {};
    pthread_t thread_so = {};
    // ------------------------------------------------ //

    int status_memoria = povoando_Memoria(thread_memoria, diretorio);
    pthread_join(thread_memoria, nullptr);

    // Inicializa o SO com os processos carregados
    int status_so = iniciando_SO(thread_so, processosNaMemoria);

    if (status_memoria == 0 && status_so == 0)
    {
        printProcessos();
        imprimirListaCircular_SO(); // Confirmação dos processos carregados no SO
    }

    pthread_join(thread_so, nullptr); // Aguardar o término da thread do SO
    return 0;
}
