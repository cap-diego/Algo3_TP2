
#ifndef CODIGO_BASICS_2H
#define CODIGO_BASICS_2H

#include <iostream>
#include <vector>
#include <climits>
#include <cstring>
#include <algorithm>
#include <cmath>
#include <utility>
#include <cstdio>
#include <list>
using namespace std;

typedef float Peso;

struct Nodo {
    float x,y;
    int indice;
    Nodo() : indice(-1){}
    bool operator==(Nodo &n) {
        return(this->x == n.x && this->y == n.y);
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
        return(ar.hasta == (*this).hasta && ar.desde == (*this).desde);
        //return (ar.hasta == (*this).hasta && ar.desde == (*this).desde && (*this).indice == ar.indice);
    }
    bool operator<(Arista &ar){
        if(this->indice == -1) return false;
        if(ar.indice == -1) return true;
        return ((*this).peso < ar.peso);
    }
};

class MatrizAdyacencias {
private:
    vector< vector<Peso> > rep;
public:
    MatrizAdyacencias(const MatrizAdyacencias& c) : rep(c.rep) {}
    MatrizAdyacencias(int cantidad_de_nodos) {
        for (int i = 0; i < cantidad_de_nodos; ++i) {

            for (int j = 0; j < cantidad_de_nodos; ++j) {
                rep[i].push_back(-1);
            }
        }
    }
    void agregarArista(Arista ar){
        rep[ar.desde.indice][ar.hasta.indice] = ar.peso;
    }
    void sacarArista(Arista ar){
        rep[ar.desde.indice][ar.hasta.indice] = -1;
    }
    int size() { return (int)rep.size(); }
    vector<Peso>& operator[](int x) { return rep[x]; }
    void operator=(const MatrizAdyacencias& m) { rep = m.rep; }

};

class ListaAdyacencias {
private:
    //vector< vector<Nodo> > rep;
    //vector< vector<std::pair<Nodo, Peso>> > rep;
    vector< list< pair<Nodo,Peso> > > rep;
public:
    ListaAdyacencias() {}
    ListaAdyacencias(const ListaAdyacencias& c) : rep(c.rep) {}
    ListaAdyacencias(int cantidad_de_nodos) {
        rep.resize(cantidad_de_nodos);
    }

    void sacarArista(Arista ar) { //O(grado(ar.desde)
        list<pair<Nodo,Peso> >::iterator it = rep[ar.desde.indice].begin();
        while(it != rep[ar.desde.indice].end() && (*it).first.indice!=ar.hasta.indice) {
            it++;
        }
        rep[ar.desde.indice].erase(it);
    }
    int size() { return (int)rep.size(); }
    list<pair<Nodo, Peso> >& operator[](Nodo x) { return rep[x.indice]; }
    void operator=(ListaAdyacencias& m) { rep = m.rep; }
    void agregarArista(Arista ar) {
        pair<Nodo,Peso> p = make_pair(ar.hasta,ar.peso);
        rep[ar.desde.indice].push_back(p);
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
    MatrizAdyacencias& convetirAMatriz(int cNodos){ //O(m)
        MatrizAdyacencias m(cNodos);
        for (int i = 0; i < rep.size(); ++i) {
            m.agregarArista(rep[i]);//O(1)
        }

    }
    ListaAdyacencias& convertirALista(int cNodos) {
        ListaAdyacencias la(cNodos);
        for (int i = 0; i < rep.size(); ++i) {//O(m * m)
            la.agregarArista(rep[i]);//O(grado de rep[i].desde)
        }
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
    Arista& getArista(Nodo a, Nodo b) {
        return getArista(indiceArista(a,b));
    }
    Nodo& getPrimerNodo(Arista& ar){
        return ar.desde;
    }
    Nodo& getSegundoNodo(Arista& ar) {
        return ar.hasta;
    }
    int indiceArista(Nodo a, Nodo b) {
        Arista ar;
        ar.desde=a;
        ar.hasta=b;
        for (int i = 0; i < cantidad_aristas(); ++i) {
            if(ar == rep[i]){ //si existe la arista de nodos que ingrese
                sacarArista(ar);
                return i;
            }
        }

    }
    void sacarArista(Nodo a , Nodo b) {
        sacarArista(getArista(indiceArista(a,b)));
    }
    void sacarArista(Arista& a) {
        int i;
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
            if(rep[i].indice!=-1) cout<<"Arista " << i << " : [(" << rep[i].desde.x << " , " <<  rep[i].desde.y << ") , ("<<rep[i].hasta.x <<" , "<<rep[i].hasta.y <<")]"<< " peso: "<< rep[i].peso<<endl;
        }
    }

};



#endif //CODIGO_BASICS_H
