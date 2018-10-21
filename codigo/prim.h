#ifndef CODIGO_PRIM_H
#define CODIGO_PRIM_H

#include "kruskal.h"

bool todosVisitados(vector<bool>& v);
Nodo menorNoVisitado(vector<int>& distancia, vector<bool>& visitados);
vector<Nodo> prim(ListaAdyacencias& G);
vector<Nodo> prim2(ListaAdyacencias& G);

#endif //CODIGO_PRIM_H