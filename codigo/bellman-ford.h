#ifndef CODIGO_BELLMAN_FORD_H
#define CODIGO_BELLMAN_FORD_H
#include "basics.h"

//Version con Matriz de Adyacencia:
vector<int> bellmanFordMA1(vector<int> listaNodos, MatrizAdyacencias & matrix, int nodo_S); //El retorno va a ser el vector de distancias

vector<int> bellmanFordMA2(vector<int> listaNodos, MatrizAdyacencias& matrix, int nodo_S);

#endif //CODIGO_BELLMAN_FORD_H
