#include "kruskal.h"

void init(vector<int>& padre, int cantNodos , vector<int>&);
int find(int indiceNodo, vector<int>&, vector<int>& altura, int subidos);
void unir_componentes(int indiceNodo1, int indiceNodo2, vector<int>&, int cantNodos,vector<int>& altura);

ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, int cantNodos);


int main() {
    ListaIncidencia l1(9);
    Nodo n0,n1,n2,n3,n4,n5;

    n0.indice=0;
    n1.indice=1;
    n2.indice=2;
    n3.indice=3;
    n4.indice=4;
    n5.indice=5;

    n0.valor = 0;
    n1.valor = 1;
    n2.valor = 2;
    n3.valor = 3;
    n4.valor = 4;
    n5.valor = 5;




    Arista ar1(n0,n1,4),ar2(n0,n2,7),ar3(n0,n5,3), ar4(n1,n2,3),ar5(n1,n4,1), ar6(n2,n4,1), ar7(n4,n2,1), ar8(n4,n3,4), ar9(n5,n4,3);
    l1.agregarArista(ar1);
    l1.agregarArista(ar2);
    l1.agregarArista(ar3);
    l1.agregarArista(ar4);
    l1.agregarArista(ar5);
    l1.agregarArista(ar6);
    l1.agregarArista(ar7);
    l1.agregarArista(ar8);
    l1.agregarArista(ar9);

    l1.ordenarPorPeso();
    cout <<"GRAFO NORMAL"<<endl;
    l1.mostrar();
    ListaIncidencia agm = AGM_Kruskal(l1,6);
    cout <<"AGM: "<<endl;
    agm.mostrar();
    return 0;
}


ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, int cantNodos){
    //Como es agm, m = n-1
    ListaIncidencia agm(cantNodos-1);//O(n)
    vector<int> padre(cantNodos);//O(n)
    vector<int> altura(cantNodos); //O(n)
    init(padre,cantNodos,altura); //O(n)
    grafo.ordenarPorPeso();//O( n * Log(n)) + O(n)
    for (int i = 0; i < grafo.cantidad_aristas(); ++i) { //O(n)*O(log n)
        if(not( find(grafo.getArista(i).desde.indice,padre,altura,0) == find(grafo.getArista(i).hasta.indice,padre,altura,0) ) ) {  //si pertenecen a diferentes comp conexas
            //agrego la arista i al agm
            agm.agregarArista(grafo.getArista(i));
            //ahora uno las comp conexas
            unir_componentes(grafo.getArista(i).desde.indice,grafo.getArista(i).hasta.indice,padre,cantNodos,altura);
        }
    }
    return agm;
}

void init(vector<int>& padre, int cantNodos, vector<int>& altura){
    for (int i = 0; i < cantNodos; ++i) {
        altura[i] = 1;
        padre[i] = i;
    }
}

//O(log n), no recuerdo la justif exacta
int find(int indiceNodo, vector<int>& padre, vector<int>& altura, int subidos) { //devuelvo  el indice del nodo que es rep
    //padre tiene en la pos iesima el nodo "representante" del iesimo nodo
    if(padre[indiceNodo] == indiceNodo) {
        altura[indiceNodo] = subidos;
        return indiceNodo;
    }else if(padre[indiceNodo] != indiceNodo){
        padre[indiceNodo] = find(padre[indiceNodo],padre,altura,1+subidos);
        return padre[indiceNodo];
    }
}
void unir_componentes(int indiceNodo1, int indiceNodo2, vector<int>& padre, int cantNodos, vector<int>& altura){ //O(1)
    int indicePadreNodo1 = find(indiceNodo1,padre,altura,0);
    int indicePadreNodo2 = find(indiceNodo2,padre,altura,0);

    if(altura[indicePadreNodo1] > altura[indicePadreNodo2]) { //si el arbol de representantes de la arista 1 es mas alto que el de la arista 2, entonces colgamos al de ar2 al de ar1, ya que como mucho los find van a ser de O(altura(ar1)), y es menos costoso pasar del mas chico al mas grande
        padre[indicePadreNodo2] = indicePadreNodo1;
    }else if(altura[indicePadreNodo1] < altura[indicePadreNodo2]) {
        padre[indicePadreNodo1] = indicePadreNodo2;
    }else {
        padre[indicePadreNodo1] = indicePadreNodo2;
        altura[indicePadreNodo1]++;
    }
    //tenemos en ar1 la que tiene una altura menor

}