
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
#include <queue>
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
    bool operator!=(Nodo& n) {
        return(this->x != n.x || this->y != n.y);
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
        return((ar.hasta == (*this).hasta && ar.desde == (*this).desde) || (ar.desde == (*this).hasta && ar.hasta == (*this).desde));
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
    //devuelvo el vector con los pesos de los ejes de los vecinos de k pasos de nodo y la cantidad de vecinos de k pasos
    pair<vector<Peso>, int> pesosVecinosDeKPasosDesdeNodo(Nodo &nodo, int k){

        vector<Peso> pesos;
        int cantVecinos=0;
        queue<Nodo> colaDeNodos;
        Nodo nodoactual;
        //pongo en cola todos los nodos que se llegan en k pasos desde nodo
        colaDeNodos.push(nodo);
        while(k > 0){
            nodoactual = colaDeNodos.front();
            for(auto l : rep[nodoactual.indice]){
                colaDeNodos.push(l.first);
            }
            //tengo que remover el primero en haber ingresado
            //pero antes tengo que ver sus vecinos
            for(auto p: pesosDeVecinosDesdeNodo(nodoactual).first) {
                pesos.push_back(p);
            }
            cantVecinos+= pesosDeVecinosDesdeNodo(nodoactual).second;
            //elimino el primero de la cola
            colaDeNodos.pop();
            k--;
        }
        while(colaDeNodos.size() >= 0) {
            nodoactual = colaDeNodos.front();
            for(auto p: pesosDeVecinosDesdeNodo(nodoactual).first) {
                pesos.push_back(p);
            }
            cantVecinos+= pesosDeVecinosDesdeNodo(nodoactual).second;
        }
        return make_pair(pesos,cantVecinos);
    }
    pair<vector<Peso>, int> pesosDeVecinosDesdeNodo(Nodo& nodo) {
        pair<vector<Peso>, int> p = make_pair(vector<Peso>(),0);
        for(auto l : rep[nodo.indice]){
            p.first.push_back(l.second);
            p.second++;
        }
        return p;
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
    ListaIncidencia(const ListaIncidencia &c) : rep(c.rep){
        this->aristasActuales = c.aristasActuales;
        this->aristasTotales = c.aristasTotales;
    };
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
            //if(aristasActuales%1000==0) cout <<"ag arista: "<<aristasActuales<<endl;
            a.indice=aristasActuales;
            rep[aristasActuales] = a;
            aristasActuales++;
        }else{
            a.indice = aristasActuales;
            rep.push_back(a);
            aristasActuales++;
            aristasTotales++;
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
        return aristasActuales;
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
//funciones DSU, para kruskal
void init(vector<int>& padre, int cantNodos , vector<int>&);
int find(int indiceNodo, vector<int>&, vector<int>& altura, int subidos);
void unir_componentes(int indiceNodo1, int indiceNodo2, vector<int>&, int cantNodos,vector<int>& altura);


//algoritmos
ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, int cantNodos);
void retirarEjesInconsistentes(ListaIncidencia& l, float media, float varianza, float std_desv);


//funciones aux
float variance ( vector<float>& v , float mean, int );

//funciones de input
void cargarInfo(vector<Nodo>&, int&, float&, float&, string);

//funcion de conversion a grafo
void convertirNodosAAristas(ListaIncidencia&, vector<Nodo>&, float&,float&,int k_vecinos); //el parametro k_vecinos es para generar un grafo con la metodologia k_vecinos. si es -1 significa que la metodologia va a ser usando la varianza y media


//funciones para el output
void exportarGrafo(ListaIncidencia&, string nombre);
void exportarNodos(vector<Nodo>& vec, string nombre);


#endif //CODIGO_BASICS_H
