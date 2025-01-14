#ifndef MEMORIA_HPP
#define MEMORIA_HPP
#include "include.hpp"

void carregarProcessosNaMemoria(int op);
void salvarNaMemoria(PCB *processo);
void *threadCarregarProcessos(void *arg);
int povoando_Memoria(pthread_t &thread_memoria, int op);
void imprimirMemoria();

#endif