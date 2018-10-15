
#ifndef CODIGO_BASICS_H
#define CODIGO_BASICS_H

#include <iostream>
#include <vector>
#include <climits>
#include <cstring>
#include <cmath>
#include <utility>
#include <cstdio>
using namespace std;

typedef int Nodo;
typedef int Peso;

struct Arista {
    Nodo desde;
    Nodo hasta;
    Arista(Nodo d, Nodo h) : desde(d), hasta(h) {}
    Arista(pair<Nodo, Nodo> a) : desde(a.first), hasta(a.second) {}
};

struct AristaAd{
    Nodo adyacente;
    Peso peso;
    AristaAd(Nodo a, Peso p) : adyacente(a), peso(p) {};
};

class ListaAdyacencias {
private:
    //vector< vector<Nodo> > rep;
    //vector< vector<std::pair<Nodo, Peso>> > rep;
    vector< vector<AristaAd> > rep;
public:
    ListaAdyacencias() {}
    ListaAdyacencias(const ListaAdyacencias& c) : rep(c.rep) {}
    ListaAdyacencias(int cantidad_de_nodos) {
        for (int i = 0; i < cantidad_de_nodos; ++i) {
            rep.push_back({});
        }
    }

    void agregarNodo() { rep.push_back({}); }
    void sacarUltimo() { rep.pop_back(); }
    void sacarNodo(Nodo x) { rep.erase(rep.begin() + x); }
    int size() { return (int)rep.size(); }
    vector<AristaAd>& operator[](Nodo x) { return rep[x]; }
    void operator=(ListaAdyacencias& m) { rep = m.rep; }

    //Este print se pudo haber roto
    void print() {
        for (int x = 0; x < rep.size(); ++x) {
            for (int y = 0; y < rep[x].size(); ++y) {
                if (rep[x][y].adyacente != INT_MAX) {
                    printf("%i", rep[x][y].adyacente);
                    int tam;
                    if (rep[x][y].adyacente != 0)
                        tam = int(log10(rep[x][y].adyacente));
                    else
                        tam = 0;
                    for (int i = 0; i < 12 - tam; ++i) {
                        printf(" ");
                    }
                } else {
                    printf("inf          ");
                }
            }
            printf("\n");
        }
    }

    void agregarArista(Nodo inicio, Nodo fin, Peso p){
        rep[inicio].emplace_back(fin, p);
        rep[fin].emplace_back(inicio, p);
    }
};

class MatrizAdyacencias {
private:
    vector< vector<Peso> > rep;
public:
    MatrizAdyacencias(const MatrizAdyacencias& c) : rep(c.rep) {}
    MatrizAdyacencias(int cantidad_de_nodos) {
        for (int i = 0; i < cantidad_de_nodos; ++i) {
            rep.push_back({});
            for (int j = 0; j < cantidad_de_nodos; ++j) {
                rep[i].push_back(INT_MAX);
            }
        }
    }
    int size() { return (int)rep.size(); }
    vector<Nodo>& operator[](Nodo x) { return rep[x]; }
    void operator=(const MatrizAdyacencias& m) { rep = m.rep; }
    void print() {
        for (int x = 0; x < rep.size(); ++x) {
            for (int y = 0; y < rep[x].size(); ++y) {
                if (rep[x][y] != INT_MAX) {
                    printf("%i", rep[x][y]);
                    int tam;
                    if (rep[x][y] != 0)
                        tam = int(log10(rep[x][y]));
                    else
                        tam = 0;
                    for (int i = 0; i < 12 - tam; ++i) {
                        printf(" ");
                    }
                } else {
                    printf("inf          ");
                }
            }
            printf("\n");
        }
    }
};

#endif //CODIGO_BASICS_H
