#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "include.hpp"

Processo criarProcesso(int quantumInicial, int idProcesso);
void *processarProcesso(void *arg);
void salvarNoArquivo(const string &conteudo);
void atualizarStatusCores();

#endif
