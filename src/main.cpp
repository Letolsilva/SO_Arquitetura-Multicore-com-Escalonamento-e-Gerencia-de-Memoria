#include "functions.hpp"
#include "memoria.hpp"
#include "include.hpp"
#include "SO.hpp"

int PC = 0;
int CLOCK = 0;
string diretorio = "data";
bool perifericos[NUM_PERIFERICOS] = {false};
vector<int> principal;
vector<mutex> mutexCores(NUM_CORE);
vector<int> processosNaMemoria;

int main()
{
    ofstream arquivo("./output/output.data", ios::trunc);
    if (!arquivo.is_open())
    {
        cerr << "Erro ao inicializar o arquivo output.data!" << endl;
    }
    arquivo.close();


    // ---------------- Bootloader ------------------//
    pthread_t thread_memoria = {};
    pthread_t thread_so = {};
    pthread_t thread_cpu[NUM_CORE];
    // ----------------------------------------------//

    int status_memoria = povoando_Memoria(thread_memoria, diretorio);
    pthread_join(thread_memoria, nullptr);

    int status_so = iniciando_SO(thread_so, processosNaMemoria);

    if (status_memoria == 0 && status_so == 0)
    {
        for (int i = 0; i < NUM_CORE; ++i)
        {
            int *coreIndex = new int(i); // apenas a thread_cpu
            int status_cpu = pthread_create(&thread_cpu[i], nullptr, processarProcesso, coreIndex);
            if (status_cpu != 0)
            {
                cerr << "Erro ao criar a thread da CPU!" << endl;
                return 1;
            }
        }

        for (int i = 0; i < NUM_CORE; ++i)
        {
            pthread_join(thread_cpu[i], nullptr);
        }
    }

    pthread_join(thread_so, nullptr);
    return 0;
}