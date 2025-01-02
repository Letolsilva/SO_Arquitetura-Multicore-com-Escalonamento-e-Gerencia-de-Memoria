#ifndef MONITORA_HPP
#define MONITORA_HPP

#include "include.hpp"


void* start(void* arg) ;
void atualizarEstadoProcesso(int idProcesso, const string &novoEstado);
void *monitorarEstados(void *);

#endif