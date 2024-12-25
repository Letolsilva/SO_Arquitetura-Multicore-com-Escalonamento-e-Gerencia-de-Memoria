#include "pipeline.hpp"
#include "ula.hpp"

void WriteBack(int resultado, int &quantum)
{
    principal.push_back(resultado);
    quantum--;
    CLOCK++;
}

void MemoryAccess(int resultado, int *registradores, int info1, int &quantum, PCB &processoAtual)
{

    // Leitura ou escrita na memória

    registradores[info1] = resultado;
    processoAtual.resultado = resultado;
    WriteBack(resultado, quantum);
    quantum--;
    CLOCK++;
}

void Execute(char instrucao, int info1, int info2, int info3, string info4, int *registradores, int &quantum, PCB &processoAtual)
{

    int soma = 0, registradorAtual = info1;

    // Executa a operação na ULA

    if (instrucao == '=')
    {
        registradores[info1] = info2;
    }
    else if (instrucao == '@')
    {
        for (int i = 0; i < info3; i++)
        {
            soma += registradores[registradorAtual];
            registradorAtual++;
            if (registradorAtual > info2)
            {
                registradorAtual = info1;
            }
            quantum--;
            CLOCK++;
        }
        MemoryAccess(soma, registradores, info1, quantum, processoAtual);
    }
    else if ((instrucao != '&') && (instrucao != '@') && (instrucao != '?'))
    {
        int resultado = ULA(registradores[info2], registradores[info3], instrucao);
        MemoryAccess(resultado, registradores, info1, quantum, processoAtual);
        quantum--;
        CLOCK++;
    }
    else if (instrucao == '?')
    {
        cout << registradores[info1] << " " << info4 << " " << registradores[info2] << ": ";
        if (info4 == "<")
        {
            if (registradores[info1] < registradores[info2])
            {
                cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {
                cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
        else if (info4 == ">")
        {
            if (registradores[info1] > registradores[info2])
            {
                cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {
                cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
        else if (info4 == "=")
        {
            if (registradores[info1] == registradores[info2])
            {
                cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {
                cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
        else if (info4 == "!")
        {
            if (registradores[info1] != registradores[info2])
            {
                cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {
                cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
    }
}

void InstructionDecode(char instrucao, int info1, int info2, int info3, string info4, int *registradores, int &quantum, PCB &processoAtual)
{
    // Decodifica a instrução e prepara a execução
    sleep(0.1);
    Execute(instrucao, info1, info2, info3, info4, registradores, quantum, processoAtual);
    quantum--;
    CLOCK++;
}

void InstructionFetch(int *registradores, string linha, int &quantum, PCB &processoAtual)
{
    char instrucao;
    int info1 = 0, info2 = 0, info3 = 0;
    string info4 = "";

    stringstream ss(linha);
    ss >> instrucao >> info1;

    if (instrucao != '&')
    {
        ss >> info2;
    }
    if ((instrucao != '=') && (instrucao != '?'))
    {
        ss >> info3;
    }
    if (instrucao == '?')
    {
        ss >> info4;
    }

    InstructionDecode(instrucao, info1, info2, info3, info4, registradores, quantum, processoAtual);
    PC++;
    quantum--;
    CLOCK++;
}
