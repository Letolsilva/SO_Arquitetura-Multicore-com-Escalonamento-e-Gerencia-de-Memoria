#ifndef SO_HPP
#define SO_HPP
#include "include.hpp"

int iniciando_SO(pthread_t &thread_SO);
int obterProximoProcesso();
void imprimirProcessosEsperando();
void imprimirListaCircular();

void add_ListaCircular(PCB processo);
void *FCFS(void *arg);
void *First_Job_First(void *arg);

#endif