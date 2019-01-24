#include "bellman-ford.h"

vector<int> problemaArbitrajeBF(MatrizAdyacencias& parametro) {
    MatrizAdyacencias p(parametro);
    vector<int> res;

    for (int i = 0; i < p.size(); ++i) {
        res = bellmanFordMA2(p, i);
        if (!res.empty())
            break;
    }
    return res;
}

vector<int> bellmanFordMA2(MatrizAdyacencias& matrix, int nodo_S){
    int n = matrix.size();
    vector<Distancia> distancias(n, -INFINITO);//Vector de distancias del nodo_S a los demas nodos.
    vector<int> padre(n, -1);
    vector<int> ciclo;
    distancias[nodo_S] = 1;

    bool cambio = true;
    bool finished = false;
    for(int k = 1; k < n+1 && cambio && !finished; k++){
        cambio = false;
        for(int i = 0; i < n && !finished; i++){
            for(int j = 0;j < n && !finished; j++) { //En el pseudocodigo se usan vecinos de i, pero aca el grafo es completo entonces todos son vecinos de todos.
                if (distancias[i] * matrix[i][j] > distancias[j]) {
                    cambio = true;
                    distancias[j] = distancias[i] * matrix[i][j];
                    padre[j] = i;
                    if (distancias[nodo_S] > 1) finished = true;
                }
            }
        }
    }
    if (!cambio || distancias[nodo_S] <= 1) return ciclo;

    int comienzo_ciclo_negativo =  nodo_S;

    int current = comienzo_ciclo_negativo;
    do {
        ciclo.push_back(current);
        current = padre[current];
    } while (current != comienzo_ciclo_negativo);
    ciclo.push_back(comienzo_ciclo_negativo);

    reverse(ciclo.begin(), ciclo.end());

    return ciclo;
}
