#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "include.hpp"

void LerInstrucoesDoArquivo(const string &nomeArquivo, int *registradores);
Processo criarProcesso(int quantumInicial, int idProcesso);
void carregarProcessos(const string &diretorio, vector<PCB> &processos);
void *processarProcesso(void *arg);

#endif
