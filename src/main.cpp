#include "functions.hpp"
#include "include.hpp"

int PC = 0;
int CLOCK = 0;
bool perifericos[NUM_PERIFERICOS] = {false};
vector<int> principal;

int main()
{

    string nomeArquivo = "data/input.data";
    int *registradores = (int *)malloc(32 * sizeof(int));
    LerInstrucoesDoArquivo(nomeArquivo, registradores);

    cout << endl
         << "PC: " << PC << " " << "CLOCK: " << CLOCK << endl
         << endl;

    // Processo processo = criarProcesso(5, 1); // teste

    return 0;
}
