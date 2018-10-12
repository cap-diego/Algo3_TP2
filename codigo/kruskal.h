
#ifndef CODIGO_BASICS_H
#define CODIGO_BASICS_H

#include <iostream>
#include <vector>
#include <climits>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <utility>
#include <cstdio>
using namespace std;

typedef float Peso;

struct Nodo {
    float valor;
    int indice;
    Nodo() : indice(-1){}
    bool operator==(Nodo &n) {
        return(n.indice == this->indice && this->valor == valor);
    }
};

struct Arista {
    Nodo desde;
    Nodo hasta;
    int indice;
    Peso peso;
    Arista(){}
    Arista(Nodo d, Nodo h, Peso p) : desde(d), hasta(h), peso(p),indice(-1) {}
    Arista(Nodo d, Nodo h, Peso p, int i) : desde(d), hasta(h), peso(p),indice(i) {}
    Arista(pair<Nodo, Nodo> a, Peso p) : desde(a.first), hasta(a.second), peso(p), indice(-1) {}
    bool operator==(Arista &ar) {
        return (ar.hasta == (*this).hasta && ar.desde == (*this).desde && (*this).indice == ar.indice);
    }
    bool operator<(Arista &ar){
        if(this->indice == -1) return false;
        if(ar.indice == -1) return true;
        return ((*this).peso < ar.peso);
    }
};

class ListaIncidencia {
private:
    vector< Arista> rep;
    int aristasActuales;
    int aristasTotales;
public:

    ListaIncidencia(){
        aristasTotales = 0;
        aristasActuales = 0;
    }
    ListaIncidencia(const ListaIncidencia &c) : rep(c.rep){};
    ListaIncidencia(int cantidad_de_aristas) {
        for (int i = 0; i < cantidad_de_aristas; ++i) {
            rep.push_back(Arista());
        }
        this->aristasActuales = 0;
        this->aristasTotales = cantidad_de_aristas;
    }
    void agregarArista(Arista& a) { //pre: al iniciar la lista de incidencia, ya sabemos la cantidad de aristas.
        if(aristasTotales > aristasActuales) {
            a.indice=aristasActuales;
            rep[aristasActuales] = a;
            aristasActuales++;
        }
    }
    void actualizarArista(int pos, Arista nueva){ //pre: pos esta en rango
        nueva.indice = pos;
        rep[pos] = nueva;
    }
    Arista& getArista(int in) {
        return rep[in];
    }
    void sacarArista(Arista& a) {
        rep[a.indice].indice=-1; //deshabilitamos esa arista
        aristasActuales--; //reducimos la cantidad de aristas
    }
    int cantidad_aristas() {
        return rep.size();
    }

    void operator=(ListaIncidencia& m) { rep = m.rep; }

    void ordenarPorPeso(){

        sort(rep.begin(),rep.end());
        for (int i = 0; i < cantidad_aristas(); ++i) {
            if(rep[i].indice !=-1) {
                rep[i].indice = i;
            }
        }
    }

    void mostrar(){
        for (int i = 0; i < rep.size(); ++i) {
            cout<<"Arista " << i << " : (" << rep[i].desde.valor << ","<<rep[i].hasta.valor <<")"<< " peso: "<< rep[i].peso<<endl;
        }
    }

};

#endif //CODIGO_BASICS_H
