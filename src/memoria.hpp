#ifndef MEMORIA_HPP
#define MEMORIA_HPP
#include "include.hpp"

void carregarProcessosNaMemoria(const string &diretorio);
void *threadCarregarProcessos(void *arg);
int povoando_Memoria(pthread_t &thread_memoria, string diretorio);
void printProcessos();

#endif