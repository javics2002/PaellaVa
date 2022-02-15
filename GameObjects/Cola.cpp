#include "Cola.h"

bool Cola::esValido(int tamGrupo)
{
    return capacidadActual + tamGrupo <= T_Max;
}

void Cola::add(GrupoClientes grupo)
{

}

