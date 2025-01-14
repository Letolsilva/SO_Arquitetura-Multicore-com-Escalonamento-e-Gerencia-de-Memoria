#ifndef MEMORIA_HPP
#define MEMORIA_HPP
#include "include.hpp"

void carregarProcessosNaMemoria();
void salvarNaMemoria(PCB *processo);
void *threadCarregarProcessos(void *arg);
int povoando_Memoria(pthread_t &thread_memoria);
int getTempoExecucao(const string &chave);
void imprimirMemoria();

#endif