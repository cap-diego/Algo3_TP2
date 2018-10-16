
#ifndef CODIGO_PRIM_H
#define CODIGO_PRIM_H

//#include "basics.h"
#include "kruskal.h"
#include <vector>

bool todosVisitados(vector<bool>& v);
<<<<<<< HEAD
Nodo menorNoVisitado(vector<int>& distancia, vector<bool>& visitados);
std::vector<Nodo> prim(ListaAdyacencias G);
vector<Nodo> prim2(ListaAdyacencias G);
=======
Nodo menorNoVisitado(vector<Nodo> nodos, vector<int>& distancia, vector<bool>& visitados);
vector<Nodo> prim(ListaAdyacencias& G);
vector<Nodo> primSinCola(ListaAdyacencias& G);
//vector<Nodo> prim2(ListaAdyacencias G);
>>>>>>> 011efd61039637a904f5eed584b038e68505ca1a

#endif //CODIGO_PRIM_H
