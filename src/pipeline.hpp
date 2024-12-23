#ifndef PIPELINE_HPP
#define PIPELINE_HPP

#include "include.hpp"

void WriteBack(int resultado, int &quantum);
void MemoryAccess(int resultado, int *registradores, int info1, int &quantum);
void Execute(char instrucao, int info1, int info2, int info3, string info4, int *registradores, int &quantum);
void InstructionDecode(char instrucao, int info1, int info2, int info3, string info4, int *registradores, int &quantum);
void InstructionFetch(int *registradores, string linha, int &quantum);

#endif