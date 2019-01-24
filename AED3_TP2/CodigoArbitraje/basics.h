
#ifndef CODIGO_BASICS_H
#define CODIGO_BASICS_H

#include <iostream>
#include <vector>
#include <climits>
#include <cfloat>
#include <cstring>
#include <cmath>
#include <utility>
#include <cstdio>
#include <algorithm>
using namespace std;

#define INFINITO 0

typedef int Nodo;
typedef double Peso;
typedef double Distancia;

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

/*
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
                    if (rep[x][y].adyacente  != 0)
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
        rep[fin].push_back(inicio, p);
    }
};
*/
class MatrizAdyacencias {
private:
    vector< vector<Peso> > rep;
public:
    MatrizAdyacencias() {}
    MatrizAdyacencias(const MatrizAdyacencias& c) : rep(c.rep) {}
    MatrizAdyacencias(int cantidad_de_nodos) {
        for (int i = 0; i < cantidad_de_nodos; ++i) {
            rep.push_back({});
            for (int j = 0; j < cantidad_de_nodos; ++j) {
                rep[i].push_back(-INFINITO);
            }
        }
    }
    int size() { return (int)rep.size(); }
    vector<Peso>& operator[](Nodo x) { return rep[x]; }
    void operator=(const MatrizAdyacencias& m) { rep = m.rep; }
    void invertir() {
        for (int i = 0; i < rep.size(); ++i) {
            for (int j = 0; j < rep[i].size(); ++j) {
                rep[i][j] = 1/rep[i][j];
            }
        }
    }
    void print() {
        for (int x = 0; x < rep.size(); ++x) {
            for (int y = 0; y < rep[x].size(); ++y) {
                int tam = 4;
                if (rep[x][y] != -INFINITO) {
                    printf("%.3f", rep[x][y]);
                    if (rep[x][y] != 0)
                        tam += int(log10(abs(rep[x][y])));
                        if (rep[x][y] < 0) tam++;
                    else
                        tam += 0;
                } else {
                    printf("inf");
                    tam = 3;
                }
                for (int i = 0; i < 12 - tam; ++i) {
                    printf(" ");
                }
            }
            printf("\n");
        }
    }
};

#endif //CODIGO_BASICS_H
