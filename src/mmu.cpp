#include "mmu.hpp"

string intParaBinario(int n) {
    if (n == 0) return "0";
    string binario = "";
    while (n > 0) {
        binario = to_string(n % 2) + binario;
        n /= 2;
    }
    return binario;
}

int binarioParaInt(const string &binario) {
    int valor = 0;
    // Itera por cada caractere da string
    for (char digito : binario) {
        // Multiplica o valor atual por 2 e adiciona o dígito atual (0 ou 1)
        valor = valor * 2 + (digito - '0');
    }
    return valor;
}