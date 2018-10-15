#include "floyd-warshall.h"

// Devuelve una matriz con las distancias mínimas de cada nodo al resto de los vértices.
MatrizAdyacencias floydWarshall(MatrizAdyacencias& ady){
    MatrizAdyacencias path(ady);
    int cn = ady.size(); // Cantidad de nodos

    for(int i = 0; i < cn; i++)
        path[i][i] = 0;

    for(int k = 0; k < cn; k++)
        for(int i = 0; i < cn; i++)
            for(int j = 0; j < cn; j++){
                long long int dt = path[i][k];
                dt += path[k][j];
                if(path[i][j] > dt)
                    path[i][j] = (int)dt;
            }

    return path;
}
