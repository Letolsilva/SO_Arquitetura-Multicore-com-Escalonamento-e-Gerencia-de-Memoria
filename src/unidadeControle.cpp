#include "unidadeControle.hpp"
#include "pipeline.hpp"

void UnidadeControle(int *registradores, string linha, int &quantum)
{
    InstructionFetch(registradores, linha, quantum);
}