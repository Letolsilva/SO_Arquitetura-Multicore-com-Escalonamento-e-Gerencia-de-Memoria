#include "cache.hpp"
#include "functions.hpp"

unordered_map<string, int> memoriaCache;

bool check_memoria_Cache(char instrucao, int info1, int info2, int resultado, bool controle)
{

    string x = gerardor_Chave(instrucao, info1, info2);

    auto it = memoriaCache.find(x);
    if (it != memoriaCache.end())
    {

        return true;
    }
    else
    {
        if (!controle)
        {
        }
        else
        {
            if (memoriaCache.size() >= TAM_CACHE)
            {
                // Remover um elemento existente antes de adicionar o novo
                // Aqui estamos removendo o primeiro elemento (FIFO), mas você pode usar outra política de remoção
                memoriaCache.erase(memoriaCache.begin());
            }
            memoriaCache[x] = resultado;
        }
    }

    return false;
}
