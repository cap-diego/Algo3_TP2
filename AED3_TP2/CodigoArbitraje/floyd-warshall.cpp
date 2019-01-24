#include "floyd-warshall.h"

vector<int> problemaArbitrajeFW(MatrizAdyacencias& parametro) {
    MatrizAdyacencias p(parametro);
   // p.invertir();
    return floydWarshall(p);
}

// Devuelve una matriz con las distancias mínimas de cada nodo al resto de los vértices.
vector<int> floydWarshall(MatrizAdyacencias distancias){
    int cn = distancias.size(); // Cantidad de nodos
    MatrizAdyacencias padres(cn);

    for (int x = 0; x < cn; ++x) {
        for (int y = 0; y < cn; ++y) {
            if (x == y || distancias[x][y] == INFINITO)
                padres[x][y] = -1;
            else
                padres[x][y] = x;
        }
    }

    for(int i = 0; i < cn; i++)
        distancias[i][i] = 1;

    for(int k = 0; k < cn; k++)
        for(int i = 0; i < cn; i++)
            for(int j = 0; j < cn; j++){
                Distancia dt = distancias[i][k] * distancias[k][j];
                if(distancias[i][j] < dt) {
                    distancias[i][j] = dt;
                    padres[i][j] = padres[k][j];
                }
            }

    int comienzo_ciclo_negativo;
    vector<int> ciclo;

    for (int i = 0; i < cn; ++i) {
        if (distancias[i][i] > 1) {
            comienzo_ciclo_negativo = i;

            int current = comienzo_ciclo_negativo;
            int padre = comienzo_ciclo_negativo;
            vector<int>::iterator busqueda;
            do {
                ciclo.push_back(padre);
                int nuevoPadre = (int)padres[current][padre];
                current = padre;
                padre = nuevoPadre;
                busqueda = find(ciclo.begin(), ciclo.end(), padre);
            } while (busqueda == ciclo.end());

            ciclo.push_back(padre);

            if (padre == comienzo_ciclo_negativo) break;
            else ciclo.clear();
        }
    }

    reverse(ciclo.begin(), ciclo.end());

    return ciclo;
}
