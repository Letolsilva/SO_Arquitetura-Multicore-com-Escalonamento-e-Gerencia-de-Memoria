#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP
#include "include.hpp"

void *processarProcesso(void *arg);
void salvarNoArquivo(const string &conteudo);
bool buscarProcessoNaMemoria(int idProcesso, Page &paginaAtual, PCB &processoAtual);
string obterEstadoProcesso(const PCB &processo);
void processarInstrucoes(PCB &processoAtual, stringstream &ss);
void atualizarESalvarProcesso(PCB &processoAtual, stringstream &ss, int &quantumInicial, int &var);

#endif
