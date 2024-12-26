#include "unidadeControle.hpp"
#include "pipeline.hpp"

void UnidadeControle(int *registradores, string linha, int &quantum, PCB &processoAtual)
{
    InstructionFetch(registradores, linha, quantum, processoAtual);
}