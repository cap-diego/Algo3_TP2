#include "kruskal.h"
#include <fstream>
#include <string>
#include <math.h>

void init(vector<int>& padre, int cantNodos , vector<int>&);
int find(int indiceNodo, vector<int>&, vector<int>& altura, int subidos);
void unir_componentes(int indiceNodo1, int indiceNodo2, vector<int>&, int cantNodos,vector<int>& altura);

ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, int cantNodos);
float variance ( vector<Nodo>& v , float mean );
void cargarInfo(vector<Nodo>&, int&, float&, float&);
void convertirNodosAAristas(ListaIncidencia&, vector<Nodo>&, float,float,int k_vecinos); //el parametro k_vecinos es para generar un grafo con la metodologia k_vecinos. si es -1 significa que la metodologia va a ser usando la varianza y media

void exportarGrafo(ListaIncidencia&, string nombre);
void exportarNodos(vector<Nodo>& vec, string nombre);
int main() {

    int cantidadDeNodos;
    vector<Nodo> input;
    float media, varianza;
    cargarInfo(input,cantidadDeNodos,media,varianza);

    ListaIncidencia l1((cantidadDeNodos*(cantidadDeNodos-1))/2);
    convertirNodosAAristas(l1, input, media,varianza, input.size());


    exportarGrafo(l1, "grafo");
    ListaIncidencia l2 = AGM_Kruskal(l1, cantidadDeNodos);
    cout <<"agm"<<endl;
    l2.mostrar();
    exportarGrafo(l2, "agm");
/*
    int k = 4;
    ListaIncidencia l3(k*cantidadDeNodos);
    convertirNodosAAristas(l3,input,media,varianza,k);
    ListaIncidencia l4 = AGM_Kruskal(l3, cantidadDeNodos);
    exportarGrafo(l3,"4vecinos");
    exportarGrafo(l4, "4vecinosAGM");
*/

    /*for (int i = 0; i < l2.cantidad_aristas(); ++i) {
        ListaIncidencia lAux = l2;
        if(l2.getArista(i).indice!=-1) {
            lAux.sacarArista(l2.getArista(i));
            char carac = char(i);
            exportarGrafo(lAux,string("arista" + carac));
        }
    }*/
    ListaAdyacencias la= l2.convertirALista(cantidadDeNodos);
    vector<Nodo> vec(cantidadDeNodos,Nodo());
    for (int i = 0; i < l2.cantidad_aristas(); ++i) {
        Nodo a = l2.getPrimerNodo(l2.getArista(i));
        Nodo b = l2.getSegundoNodo(l2.getArista(i));
        if(vec[a.indice].indice == -1) {
            vec[a.indice] = a;
        }
        if(vec[b.indice].indice == -1) {
            vec[b.indice] = b;
        }
    }
    exportarNodos(vec,"nodos");
    return 0;
}

float distancia(Nodo& n1, Nodo& n2){
    return(sqrt(pow(n1.x-n2.x,2)+ pow(n1.y- n2.y,2)) );
}

void exportarNodos(vector<Nodo>& vec, string nombre) {
    ofstream myfile;
    myfile.open ("../grafos/" + nombre + ".csv");
    for (int i = 0; i < vec.size(); ++i) {
        myfile<<vec[i].indice<<","<<vec[i].x<<","<<vec[i].y<<endl;
    }
    cout <<"GENERADO lista de nodos: "<<nombre<<endl;
    myfile.close();
}

void exportarGrafo(ListaIncidencia& l, string nombre) {
    ofstream myfile;
    myfile.open ("../grafos/" + nombre + ".csv");
    for (int i = 0; i < l.cantidad_aristas(); ++i) {
        if(l.getArista(i).indice!=-1) myfile<<l.getArista(i).desde.indice<<","<<l.getArista(i).hasta.indice<<","<< l.getArista(i).peso<<endl;
    }
    cout <<"GENERADO GRAFO: "<<nombre<<endl;
    myfile.close();
}

void convertirNodosAAristas(ListaIncidencia& l, vector<Nodo>& v, float u,float var,int k_vecinos) {
    Arista ar;
    if(k_vecinos != -1) {
        vector<float> distancias(v.size(),0);
        int cantAg = 0;
        for (int i = 0; i < v.size(); ++i) { //para cada nodo
        distancias[i] = 0;
            for (int j = 0; j < v.size(); ++j) {//para cada nodo dif del anterior
                if(v[i].x != v[j].x && v[i].y != v[j].y )  { //si no son el mismo nodo
                    if(k_vecinos == v.size()) {
                        ar.indice = cantAg;
                        ar.desde = v[i];
                        ar.hasta = v[j];
                        ar.peso = distancia(v[i],v[j]);
                        l.agregarArista(ar);
                        cantAg++;
                    }else{
                         distancias[j] = distancia(v[i],v[j]);
                    }
                }
            }
            if(k_vecinos != v.size()) {
                sort(distancias.begin(), distancias.end()); //ordeno de menor a mayor
                for (int k = 0; k < k_vecinos; ++k) {
                    ar.indice = k;
                    ar.desde = v[i];
                    ar.hasta = v[k+1];
                    ar.peso = distancias[k+1];
                    l.agregarArista(ar);
                    //muevo uno (k+1) porque distancias esta ordenado de menor a mayor y la distancia del v[i] es 0 y va a ser la primera
                }
            }
        }
    }
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

void cargarInfo(vector<Nodo>& v, int& cn,float& u, float& var){
    ifstream inFile;
    inFile.open("../s1.txt");
    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    inFile >> cn;
    float x,y, acumValores;
    Nodo nodo;
    int cantidadIng = 0;
    v.resize(cn);
    //ahora leo todos los nodos
    while (inFile >> x >> y) {
        nodo.indice = cantidadIng;
        nodo.x = x/1000;
        nodo.y = y/1000;
        cout <<x<<" "<< y<<endl;
        //acumValores += valNodo;
        v[cantidadIng] = nodo;
        cantidadIng++;
    }
    //u = acumValores / cn;
    //var = variance(v, u);
    inFile.close();
}
float variance ( vector<Nodo>& v , float mean ) {
    float sum = 0.0;
    float temp =0.0;

    for ( int j =0; j < v.size(); j++)
    {
        temp = float(pow((mean),  2));
        sum += temp;
    }

    return (sum/v.size());
}




void init(vector<int>& padre, int cantNodos, vector<int>& altura){
    for (int i = 0; i < cantNodos; ++i) {
        altura[i] = 1;
        padre[i] = i;
    }
}

//O(log n), no recuerdo la justif exacta
int find(int indiceNodo, vector<int>& padre, vector<int>& altura, int subidos) { //devuelvo  el indice del nodo que es rep
    //cout <<"entro a find"<<endl;
    //padre tiene en la pos iesima el nodo "representante" del iesimo nodo
    if(padre[indiceNodo] == indiceNodo) { //si el rep del nodo es él mismo
        altura[indiceNodo] = subidos;
        return indiceNodo;
    }else if(padre[indiceNodo] != indiceNodo){
        padre[indiceNodo] = find(padre[indiceNodo],padre,altura,1+subidos); //path compr
        //return find(padre[indiceNodo],padre,altura,1+subidos);
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