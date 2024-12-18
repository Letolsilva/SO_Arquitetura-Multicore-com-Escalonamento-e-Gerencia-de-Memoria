#include "functions.hpp"
#include "memoria.hpp"
#include "include.hpp"

int PC = 0;
int CLOCK = 0;
string diretorio = "data";
bool perifericos[NUM_PERIFERICOS] = {false};
vector<int> principal;
sem_t semaforoCores; // Definição do semáforo
vector<mutex> mutexCores(NUM_CORE);

int main(){

    // Bootloader ------------------------------------ //
    //pthread_t thread_cpu = {};
    pthread_t thread_memoria = {};
    pthread_t thread_so = {};
    // ------------------------------------------------ //

    int status_memoria = povoando_Memoria(thread_memoria,diretorio);
    int status_so = iniciando_SO(thread_so);


    pthread_join(thread_memoria, nullptr);
    
    if(status_memoria == 0 && status_so == 0){
        printProcessos();
        imprimirListaCircular_SO();
    }

    /*
    vector<pthread_t> threads(memoria.size());
    vector<int> ids(memoria.size());

    // Inicializar o semáforo com valor 3 (3 cores disponíveis)
    sem_init(&semaforoCores, 0, NUM_CORE);

    // Criar threads para processar os processos
    for (size_t i = 0; i < memoria.size(); ++i)
    {
        ids[i] = i; // Armazena o ID para passar como argumento
        pthread_create(&threads[i], nullptr, processarProcesso, &ids[i]);
    }

    // Aguardar todas as threads terminarem
    for (auto &thread : threads)
    {
        pthread_join(thread, nullptr);
    }

    // Destruir o semáforo
    sem_destroy(&semaforoCores);
    */

    return 0;
}
