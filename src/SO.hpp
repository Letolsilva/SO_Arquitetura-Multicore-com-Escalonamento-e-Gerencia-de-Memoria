#ifndef SO_HPP
#define SO_HPP
#include "include.hpp"

int iniciando_SO(pthread_t &thread_SO);
int obterProximoProcesso();
void imprimirProcessosEsperando();
void imprimirListaCircular();

void add_ListaCircular(PCB processo);
void *FCFS(void *arg);
void *First_Remain_Job_First(void *arg);
void remover_ListaCircular(int id_processo);

void gerar_lista_2();
string codificandoJob(Page pag);
std::map<std::string, std::vector<PCB>> agruparJobsPorChaves(const std::vector<PCB>& jobs);
void gerar_lista_similiaridade();

#endif