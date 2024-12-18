#ifndef MEMORIA_HPP
#define MEMORIA_HPP
#include "include.hpp"

void carregarProcessosNaMemoria(const string &diretorio);
void* threadCarregarProcessos(void* arg);
int povoando_Memoria(pthread_t &thread_memoria, string diretorio);
void printProcessos();


int iniciando_SO(pthread_t &thread_SO);
void* fazerListaCircular_SO(void* arg);
void imprimirListaCircular_SO();

#endif