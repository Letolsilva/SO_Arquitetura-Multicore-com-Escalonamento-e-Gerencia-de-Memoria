#include "pipeline.hpp"
#include "ula.hpp"

void WriteBack(int resultado, int &quantum, PCB &processoAtual)
{
    if (quantum <= 0){
        processoAtual.estado = BLOQUEADO;
        return;
    }

    quantum--;
    CLOCK++;
    principal.push_back(resultado);
}

void MemoryAccess(int resultado, int *registradores, int info1, int &quantum, PCB &processoAtual)
{
    // Leitura ou escrita na memória
    if (quantum <= 0){
        processoAtual.estado = BLOQUEADO;
        return;
    } 

    registradores[info1] = resultado;
    processoAtual.resultado = resultado;
    quantum--;
    CLOCK++;
    WriteBack(resultado, quantum, processoAtual);
}

void Execute(char instrucao, int info1, int info2, int info3, string info4, int *registradores, int &quantum, PCB &processoAtual)
{
    if (quantum <= 0){
        processoAtual.estado = BLOQUEADO;
        return;
    } 

 //   cout << instrucao << endl;
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
            if (quantum <= 0){
                processoAtual.estado = BLOQUEADO;
                return;
            } 
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
        if (quantum <= 0){
                processoAtual.estado = BLOQUEADO;
            return;
        }
                   

        int resultado = ULA(registradores[info2], registradores[info3], instrucao);
        quantum--;
        CLOCK++;
        MemoryAccess(resultado, registradores, info1, quantum, processoAtual);
        
    

    }
    else if (instrucao == '?')
    {
        // cout << registradores[info1] << " " << info4 << " " << registradores[info2] << ": ";
        if (info4 == "<")
        {
            if (registradores[info1] < registradores[info2])
            {   
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {   
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
        else if (info4 == ">")
        {
            if (registradores[info1] > registradores[info2])
            {   
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {   
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
        else if (info4 == "=")
        {
            if (registradores[info1] == registradores[info2])
            {   
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "False" << endl;
                quantum--;
                CLOCK++;

            }
        }
        else if (info4 == "!")
        {
            if (registradores[info1] != registradores[info2])
            {
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "True" << endl;
                quantum--;
                CLOCK++;
            }
            else
            {   
                if (quantum <= 0){
                    processoAtual.estado = BLOQUEADO;
                    return;
                } 
                // cout << "False" << endl;
                quantum--;
                CLOCK++;
            }
        }
    }
}

void InstructionDecode(char instrucao, int info1, int info2, int info3, string info4, int *registradores, int &quantum, PCB &processoAtual)
{
    // Decodifica a instrução e prepara a execução
    if (quantum <= 0){
        processoAtual.estado = BLOQUEADO;
        return;
    } 

    sleep(0.1);
    quantum--;
    CLOCK++;
    Execute(instrucao, info1, info2, info3, info4, registradores, quantum, processoAtual);
    
}

void InstructionFetch(int *registradores, string linha, int &quantum, PCB &processoAtual)
{
    if (quantum <= 0){
        processoAtual.estado = BLOQUEADO;
        return;
    } 

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

   // PC++;
    quantum--;
    CLOCK++;
    
    
    InstructionDecode(instrucao, info1, info2, info3, info4, registradores, quantum, processoAtual);

    if(processoAtual.estado != BLOQUEADO){
        
        PC++;
        processoAtual.pc++;
    
    }
    /*
    if (quantum <= 0){
        processoAtual.estado = BLOQUEADO;
        return;
    } */
    


}
