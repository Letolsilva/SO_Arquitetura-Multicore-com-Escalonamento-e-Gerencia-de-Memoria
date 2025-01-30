#include "SO.hpp"
#include <iostream>
#include <unistd.h>

vector<int> listaCircular_SO;
vector<SO> listaCircular_SO_2;
size_t indiceAtual = 0;
mutex mutexListaCircular;

void remover_ListaCircular(int id_processo)
{
    lock_guard<mutex> lock(mutexListaCircular);

    auto it = find_if(listaCircular_SO_2.begin(), listaCircular_SO_2.end(),
                      [id_processo](const SO &so)
                      { return so.id_processo == id_processo; });

    if (it != listaCircular_SO_2.end())
    {
        // Remove o processo encontrado
        listaCircular_SO_2.erase(it);
    }
    else
    {
        // Caso o processo não esteja na lista
        // cout << "Processo não encontrado na lista: " << id_processo << endl;
    }
}

void add_ListaCircular(PCB processo)
{
    lock_guard<mutex> lock(mutexListaCircular);
    // Verifica se o processo já está na lista
    auto it = find_if(listaCircular_SO_2.begin(), listaCircular_SO_2.end(),
                      [&processo](const SO &so) { return so.id_processo == processo.id; });    if (it == listaCircular_SO_2.end())
    {
        SO aux_job;
        aux_job.id_processo = processo.id;
        aux_job.ciclo_de_vida = processo.ciclo_de_vida;
        aux_job.prioridade = processo.prioridade;
        aux_job.conjunto_chaves = processo.conjunto_chaves;
        listaCircular_SO_2.push_back(aux_job); // Adiciona apenas se não existir
        // estadosProcessos[idProcesso] = "PRONTO";
    }
    else
    {
        cout << "Processo já está na fila: " << processo.id << endl;
    }
}

void add_ListaCircular_adptado(SO processo)
{
    lock_guard<mutex> lock(mutexListaCircular);
    // Verifica se o processo já está na lista
    auto it = find_if(listaCircular_SO_2.begin(), listaCircular_SO_2.end(),
                      [&processo](const SO &so) { return so.id_processo == processo.id_processo; });

    if (it == listaCircular_SO_2.end())
    {
        listaCircular_SO_2.push_back(processo); // Adiciona apenas se não existir
        // estadosProcessos[idProcesso] = "PRONTO";
    }
    else
    {
        cout << "Processo já está na fila: " << processo.id_processo << endl;
    }
}

void gerarLista()
{
    for (const Page &pag : memoryPages)
    {
        add_ListaCircular(pag.pcb);
    }
}

double jaccardIndex(const vector<string>& vec1, const vector<string>& vec2) {
    // Transformando os vetores em conjuntos para facilitar o cálculo
    unordered_set<string> set1(vec1.begin(), vec1.end());
    unordered_set<string> set2(vec2.begin(), vec2.end());

    int intersection = 0;
    for (const auto& chave : set1) {
        if (set2.find(chave) != set2.end()) {
            intersection++;
        }
    }
    int unionSet = set1.size() + set2.size() - intersection;
    return static_cast<double>(intersection) / unionSet;
}

vector<map<int, SO>> aplicarLSH(const vector<PCB>& jobs, double threshold) {
    vector<map<int, SO>> buckets;  // Para armazenar os grupos de jobs
    
    // Aplicando uma função hash simples
    for (const auto& job : jobs) {
        bool added = false;

        SO aux_job;
        aux_job.id_processo = job.id;
        aux_job.ciclo_de_vida = job.ciclo_de_vida;
        aux_job.prioridade = job.prioridade;
        aux_job.conjunto_chaves = job.conjunto_chaves;
        
        // Verificando em quais buckets este job deve ser inserido
        for (auto& bucket : buckets) {
            for (const auto& existingJobPair : bucket) {
                const SO& existingJob = existingJobPair.second;  // Acesso ao valor do map
                if (jaccardIndex(job.conjunto_chaves, existingJob.conjunto_chaves) >= threshold) {
                    bucket[aux_job.id_processo] = aux_job;  // Adiciona o job ao bucket com alta interseção
                    added = true;
                    break;
                }
            }
            if (added) break;
        }

        // Se o job não foi adicionado a nenhum bucket, criamos um novo bucket
        if (!added) {
            map<int, SO> newBucket;
            newBucket[aux_job.id_processo] = aux_job;
            buckets.push_back(newBucket);
        }
    }

    return buckets;
}

void gerar_lista_similiaridade()
{
    vector<PCB> listaJob;
    double threshold = 0.1;

    for (const Page &pag : memoryPages) {
        listaJob.push_back(pag.pcb);
    }
    
    auto grupos = aplicarLSH(listaJob, threshold);

    int contador = 1;
    cout << "\n-------------1-----------" << endl;
    for (const auto& grupo : grupos) {
        cout << "Grupo " << contador++ << ": ";
        for (const auto& jobPair : grupo) {
            const SO& job = jobPair.second;  // Acesso ao valor do map
            add_ListaCircular_adptado(job);
            cout << "\n\t Job ID: " << job.id_processo << endl;
        }
        cout << endl;
        cout << "-------------------------" << endl;
    }
}

void *FCFS(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
    }

    return nullptr;
}

void *Similiaridade(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
    }

    return nullptr;
}

void *First_Remain_Job_First(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
        lock_guard<mutex> lock(mutexListaCircular);
        sort(listaCircular_SO_2.begin(), listaCircular_SO_2.end(), [](const SO &a, const SO &b)
            { return a.ciclo_de_vida < b.ciclo_de_vida; });
    }

    return nullptr;
}

void *Prioridade(void *arg)
{
    (void)arg;

    while (!listaCircular_SO_2.empty())
    {
        lock_guard<mutex> lock(mutexListaCircular);
        sort(listaCircular_SO_2.begin(), listaCircular_SO_2.end(), [](const SO &a, const SO &b)
             { return a.prioridade > b.prioridade; });
    }

    return nullptr;
}

int obterProximoProcesso()
{
    lock_guard<mutex> lock(mutexListaCircular);

    if (listaCircular_SO_2.empty())
    {
        return -1;
    }

    // Pega o próximo processo da lista circular
    int idProcesso = listaCircular_SO_2[indiceAtual].id_processo;

    // Remove o processo atual da lista
    listaCircular_SO_2.erase(listaCircular_SO_2.begin() + indiceAtual);

    // Ajusta o índice atual
    if (listaCircular_SO_2.empty())
    {
        indiceAtual = 0; // Reseta o índice se a lista estiver vazia
    }
    else
    {
        indiceAtual = indiceAtual % listaCircular_SO_2.size(); // Garante que o índice esteja válido
    }

    return idProcesso;
}

int iniciando_SO(pthread_t &thread_SO)
{

    if(op  == 4){
        auto inicio = high_resolution_clock::now();
        gerar_lista_similiaridade();
        auto fim = high_resolution_clock::now();
        auto duracao = duration_cast<nanoseconds>(fim - inicio);
        cout << "\n\n\t [!] - Tempo para calcular similiaridade: " << duracao.count() << " nanosegundos.\n" << endl;

    }
    else{
        gerarLista();
    }

    //cout << "-------------1-----------" << endl;
    for (const auto& so : listaCircular_SO_2) {
        cout << "ID do Processo: " << so.id_processo << endl;
        cout << "Ciclo de Vida: " << so.ciclo_de_vida << endl;
        cout << "Prioridade: " << so.prioridade << endl;
        cout << "-------------------------" << endl;
    }
    //cout << "------------@------------" << endl;

    int ret = 0;

    switch (op)
    {
    case 1:
        ret = pthread_create(&thread_SO, nullptr, FCFS, nullptr);
        break;

    case 2:
        ret = pthread_create(&thread_SO, nullptr, First_Remain_Job_First, nullptr);
        break;
    case 3:
        ret = pthread_create(&thread_SO, nullptr, Prioridade, nullptr);
        break;
    case 4:
        ret = pthread_create(&thread_SO, nullptr, Similiaridade, nullptr);
        break;
    default:
        return 1;
        break;
    }

    if (ret != 0)
    {
        cerr << "Erro ao criar a thread do SO!" << endl;
        return 1;
    }

    return 0;
}

void imprimirProcessosEsperando()
{
    lock_guard<mutex> lock(mutexListaCircular);
    for (int id : listaCircular_SO)
    {
        cout << "Processo ID: " << id << endl;
    }
}

void imprimirListaCircular()
{
    lock_guard<mutex> lock(mutexListaCircular);
    if (listaCircular_SO_2.empty())
    {

        return;
    }

    cout << "Lista circular atual:" << endl;
    size_t index = 0;
    for (const SO &pag : listaCircular_SO_2)
    {
        cout << "Índice " << index << ": Processo ID " << pag.id_processo;
        if (index == indiceAtual)
        {
            cout << " <- Índice atual";
        }
        cout << endl;
        index++;
    }
}
