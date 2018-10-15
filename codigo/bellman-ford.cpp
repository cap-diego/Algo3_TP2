#include "bellman-ford.h"

vector<int> bellmanFordMA1(vector<int> listaNodos, MatrizAdyacencias& matrix, int nodo_S){
    vector<int> distancias(listaNodos.size(),-1);//Vector de distancias del nodo_S a los demas nodos.
    distancias[nodo_S] = 0;
    int n = listaNodos.size();
    for(int k = 1; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0;j < n; j++) { //En el pseudocodigo se usan vecinos de i, pero aca el grafo es completo entonces todos son vecinos de todos.
                if (distancias[j] == -1 && distancias[i] == -1) {
                    //Nada
                } else if (distancias[j] == -1) {
                    distancias[j] = distancias[i] + matrix[i][j];
                } else if (distancias[i] == -1) {
                    distancias[j] = distancias[j]; //Valga la redundancia
                } else {
                    distancias[j] = min(distancias[j], distancias[i] + matrix[i][j]);
                }
            }
        }
    }
    return distancias;
}

vector<int> bellmanFordMA2(vector<int> listaNodos, MatrizAdyacencias& matrix, int nodo_S){
    unsigned int infinito = pow(2,32) - 1;
    vector<int> distancias(listaNodos.size(),infinito);//Vector de distancias del nodo_S a los demas nodos.
    distancias[nodo_S] = 0;
    int n = listaNodos.size();
    for(int k = 1; k < n; k++){
        for(int i = 0; i < n; i++){
            for(int j = 0;j < n; j++) { //En el pseudocodigo se usan vecinos de i, pero aca el grafo es completo entonces todos son vecinos de todos.
                distancias[j] = min(distancias[j], distancias[i]+ matrix[i][j]);
            }
        }
    }
    return distancias;
}