#ifndef SO_HPP
#define SO_HPP
#include "include.hpp"

void *fazerListaCircular_SO(void *arg);
int iniciando_SO(pthread_t &thread_SO, vector<int> processos);
void atualizarListaCircular(int idProcesso);
int obterProximoProcesso();

#endif