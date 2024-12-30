#include "functions.hpp"
#include "monitora.hpp"
#include "memoria.hpp"
#include "include.hpp"
#include "SO.hpp"

int PC = 0;
int CLOCK = 0;
bool perifericos[NUM_PERIFERICOS] = {false};
vector<int> principal;
vector<mutex> mutexCores(NUM_CORE);
vector<int> processosNaMemoria;

int main()
{
    pthread_t monitor = {};
    int status_monitor = pthread_create(&monitor, nullptr, start, (void*)&processosNaMemoria);
    if (status_monitor != 0) {
        cerr << "Erro ao criar a thread do Monitor!" << endl;
        return 1;
    }
    pthread_join(monitor, nullptr);

    return 0;
}