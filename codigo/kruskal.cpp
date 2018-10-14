#include "kruskal.h"
#include <fstream>
#include <string>
#include <math.h>


template<typename T>
float calcularMedia(vector<T>&, int);

int main() {

    int cantidadDeNodos;
    vector<Nodo> input;
    float media=0, varianza=0, std_desv;
    cargarInfo(input,cantidadDeNodos,media,varianza, "grafo1");

    ListaIncidencia l1((cantidadDeNodos*(cantidadDeNodos-1))/2); //l1 grafo completo

    cout <<"prepara para generar grafo con el que despues voy a trabajar"<<endl;
    convertirNodosAAristas(l1, input, media,varianza, input.size()-1);

    std_desv= sqrt(varianza);
    cout <<"Se ha generado el grafo completo"<<endl;
    exportarGrafo(l1, "grafo");
    l1.mostrar();

    //Generamos el AGM
    ListaIncidencia l2 =(AGM_Kruskal(l1, cantidadDeNodos));
    cout <<"Se genera el agm"<<endl;
    l2.mostrar();
    exportarGrafo(l2, "agm");

    //para graficar los nodos en el plano
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


    //Ahora quiero sacar ejes

    ListaAdyacencias la(cantidadDeNodos);
    l2.convertirALista(la);
    media = 0, std_desv = 0;
    vector<Peso> pesosDeVecinos;
    int cantVecinos;
    int k = 1;
    for (int j = 0; j < l2.cantidad_aristas(); ++j) {
        //recorro los nodos y veo si alguno es inconsistenteç
        cout <<"ARISTA: "<<j<<" peso: "<<l2.getArista(j).peso<<endl;

        //COMPARO CON PRIMER EXTREMO DE ARISTA ACTUAL
        pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).desde, l2.getArista(j).hasta,k);
        cantVecinos = pesosDeVecinos.size();
        cout <<"cant de vecinos: "<<cantVecinos<<endl;
        cout <<"vecinos de arista: "<<j << " = "<< cantVecinos<<endl;

        media = calcularMedia(pesosDeVecinos,cantVecinos);
        cout <<"media: "<<media<<endl;
        std_desv = variance(pesosDeVecinos,media,cantVecinos);
        cout <<"std_desv: "<<std_desv<<endl;
        if(l2.getArista(j).peso > 2*std_desv) { //si es eje inconsistente
            l2.sacarArista(l2.getArista(j));
            la.sacarArista(l2.getArista(j));
            cout <<"ENCONTRE EJE INCONSISTENTE"<<endl;
        }
/*
        //COMPARO CON EL OTRO EXTREMO DE LA ARISTA ACTUAL
        pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).desde, l2.getArista(j).hasta,k);
        cantVecinos = pesosDeVecinos.size();
        media = calcularMedia(pesosDeVecinos,cantVecinos);
        std_desv = variance(pesosDeVecinos,media,cantVecinos);
        cout <<"std_desv : "<<std_desv<<endl;
        if(l2.getArista(j).peso > 2*std_desv) { //si es eje inconsistente
            l2.sacarArista(l2.getArista(j));
            la.sacarArista(l2.getArista(j));
            cout <<"ENCONTRE EJE INCONSISTENTE"<<endl;
        }
        //COMO ESTO LO HAGO CON UN AGM, NO ME PREOCUPO POR HACER ESTO CON UN MISMO NODO PORQUE ESO ES IMPOSIBLE EN ESTE CASO
*/
    }


    return 0;
}


template<typename T>
float calcularMedia(vector<T>& vec, int n) {
    T sum= 0;
    for(auto v : vec) {
        sum+=v;
    }
    return sum/n;
}

float distancia(Nodo& n1, Nodo& n2){
    return(sqrt(pow(n1.x-n2.x,2)+ pow(n1.y- n2.y,2)) );
}

void exportarNodos(vector<Nodo>& vec, string nombre) {
    ofstream myfile;
    myfile.open ("../grafos/" + nombre + ".csv");
    for (int i = 0; i < vec.size(); ++i) {
        if(vec[i].indice!=-1) myfile<<vec[i].indice<<","<<vec[i].x<<","<<vec[i].y<<endl;
    }
    cout <<"GENERADO lista de nodos: "<<nombre<<endl;
    myfile.close();
}

void exportarGrafo(ListaIncidencia& l, string nombre) {
    ofstream myfile;
    myfile.open ("../grafos/" + nombre + ".csv");
    for (int i = 0; i < l.cantidad_aristas(); ++i) {
        if(l.getArista(i).indice!=-1) myfile<<l.getArista(i).desde.indice<<","<<l.getArista(i).hasta.indice<<","<<-1<<","<< l.getArista(i).peso<<endl;
    }
    cout <<"GENERADO GRAFO: "<<nombre<<endl;
    myfile.close();
}

void retirarEjesInconsistentes(ListaIncidencia& l, float media, float varianza, float std_desv){
    //necesito recorrer los ejes y verificar cuales cumplen la nocion de eje inconsistente
    //1) nocion es : cuantas desviaciones estandar hay entre el peso de un eje y la media en el vecindario de sus nodos
    //2) nocion es: calcular la proporcion entre el peso de un eje y las medias en los vecindarios de sus nodos
    //param importantes: media, desv_standar, vecindario de un nodo,
}


void convertirNodosAAristas(ListaIncidencia& l, vector<Nodo>& v, float& u,float& var,int k_vecinos) {
    Arista ar;
    float sumaDeDistanciasNodoAct=0, varAux=0;
    if(k_vecinos != -1) {
        vector<float> distancias(v.size(),0);
        int cantAg = 0;
        float mediaNodoAct = 0;
        for (int i = 0; i < v.size(); ++i) { //para cada nodo
            distancias[i] = 0;
            sumaDeDistanciasNodoAct=0;
            for (int j = 0; j < v.size(); ++j) {//para cada nodo dif del anterior
                if(v[i] != v[j])  { //si no son el mismo nodo
                    if(k_vecinos == v.size()-1) {
                        ar.indice = cantAg;
                        ar.desde = v[i];
                        ar.hasta = v[j];
                        ar.peso = distancia(v[i],v[j]);
                        l.agregarArista(ar);
                        cantAg++;
                        distancias[j] = distancia(v[i],v[j]);
                    }else{
                        distancias[j] = distancia(v[i],v[j]);
                    }
                }
                sumaDeDistanciasNodoAct +=distancia(v[i],v[j]);
            }
            mediaNodoAct = sumaDeDistanciasNodoAct / (v.size()-1);
            varAux = variance(distancias,mediaNodoAct, v.size()-1);
            u += mediaNodoAct;
            var+=varAux;
            if(k_vecinos != v.size()-1) {
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
    u = u/v.size(); //promedio de las distancias medias para vecinos entre c/a nodo
    cout <<"var antes de div: "<<var<<endl;
    var = var/v.size();//promedio de las variancias de cada nodo
    cout <<"La media de las distancias es: "<< u<<" la varianza promedio es: "<< var<<endl;
}


ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, int cantNodos){
    //Como es agm, m = n-1
    ListaIncidencia agm;//O(n)
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

void cargarInfo(vector<Nodo>& v, int& cn,float& u, float& var, string nombre){
    ifstream inFile;
    inFile.open("../"+ nombre+".txt");
    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    inFile >> cn;
    float x,y;
    Nodo nodo;
    int cantidadIng = 0;
    v.resize(cn);
    //ahora leo todos los nodos
    while (inFile >> x >> y) {
        nodo.indice = cantidadIng;
        nodo.x = x;
        nodo.y = y;
        cout <<nodo.x<<" "<< nodo.y<<endl;
        v[cantidadIng] = nodo;
        cantidadIng++;
    }
    //u = acumValores / cn;
    //var = variance(v, u);

    inFile.close();
}
float variance ( vector<float>& v , float mean , int c) {
    float sum = 0.0;
    float temp =0.0;

    for ( int j =0; j < v.size(); j++)
    {
        //cout <<v[j]<<"-"<<mean<<endl;

        temp = float(pow((v[j]-mean),  2));
        sum += temp;
    }

    return (sum/c);
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