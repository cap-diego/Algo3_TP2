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
    cargarInfo(input,cantidadDeNodos, "g3000");
    vector<int> padre(cantidadDeNodos);//O(n)
    vector<int> altura(cantidadDeNodos); //O(n)

    int gradoNodos=cantidadDeNodos-1;
    cout <<"prepara para generar grafo con el que despues voy a trabajar"<<endl;
    int cantidadAristasConGradoIndicado = cantidadDeNodos * gradoNodos / 2;
    ListaIncidencia l1(cantidadAristasConGradoIndicado); //l1 grafo completo
    convertirNodosAAristas(l1, input, gradoNodos);
    cout <<"cantidad: "<<cantidadAristasConGradoIndicado<<endl;
    cout << "cant aristas completo: "<< l1.cantidad_aristas() <<endl;
    std_desv= sqrt(varianza);
    cout <<"Se ha generado el grafo completo"<<endl;
    //exportarGrafo(l1, "grafo");
    //l1.mostrar();

    //Generamos el AGM
    ListaIncidencia l2 =(AGM_Kruskal(l1,padre,altura,cantidadDeNodos));
    cout <<"Se genera el agm"<<endl;
    //l2.mostrar();
    exportarGrafo(l2, "agm");

    //para graficar los nodos en el plano

    exportarNodos(input,"nodos",padre);


    //Ahora quiero sacar ejes




        //COMO ESTO LO HAGO CON UN AGM, NO ME PREOCUPO POR HACER ESTO CON UN MISMO NODO PORQUE ESO ES IMPOSIBLE EN ESTE CASO
    int cantidadClusters=0;
    retirarEjesInconsistentes(l2,cantidadDeNodos,2,padre, cantidadClusters);
    //exportarGrafo(l2,"sacandoAristask2");
    exportarNodos(input,"nodos", padre);
    for(int i = 0; i < padre.size(); i++){
        cout <<"NODO: "<<i<<" REP: "<< padre[i]<<endl;
    }
/*
    for (int j = 0; j < l2.cantidad_aristas(); ++j) {
        if(l2.getArista(j).indice!=-1) cout <<"rep de nodo: "<<  l2.getPrimerNodo(j).indice << " es: "<< find(l2.getArista(j).indice,padre,altura,0)<<endl;
    }*/
    //AHORA DEBERIA TENER DIFERENTES COMPONENTES CONEXAS
    cout <<"CANTIDAD clusters: "<<cantidadClusters<<endl;

    return 0;
}


template<typename T>
float calcularMedia(vector<T>& vec, int n) {
    T sum= 0;
    if(vec.size()==0)return 0;
    for(auto v : vec) {
        sum+=v;
    }
    return sum/vec.size();
}

float distancia(Nodo& n1, Nodo& n2){
    return(sqrt(pow(n1.x-n2.x,2)+ pow(n1.y- n2.y,2)) );
}

void exportarNodos(vector<Nodo>& vec, string nombre, vector<int>& padre) {
    ofstream myfile;
    myfile.open ("../grafos/" + nombre + ".csv");
    for (int i = 0; i < vec.size(); ++i) {
        if(vec[i].indice!=-1) myfile<<vec[i].indice<<","<<vec[i].x<<","<<vec[i].y<<","<<padre[i] <<endl;
        //if(vec[i].indice!=-1) cout <<vec[i].indice<<","<<vec[i].x<<","<<vec[i].y<<","<<padre[i] <<endl;
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

void retirarEjesInconsistentes(ListaIncidencia& l2, int cantidadDeNodos, int kPasosVecindario, vector<int>& padre, int& cantidadClusters){
    //necesito recorrer los ejes y verificar cuales cumplen la nocion de eje inconsistente
    //1) nocion es : cuantas desviaciones estandar hay entre el peso de un eje y la media en el vecindario de sus nodos
    //2) nocion es: calcular la proporcion entre el peso de un eje y las medias en los vecindarios de sus nodos
    //param importantes: media, desv_standar, vecindario de un nodo,

    ListaAdyacencias la(cantidadDeNodos);
    l2.convertirALista(la);
    float media = 0, std_desv = 0;
    vector<Peso> pesosDeVecinos;
    int cantVecinos;
    int k = 1;
    int rep;
    int ca=l2.cantidad_aristas();
    for (int j = 0; j < ca; ++j) {
        if (l2.getArista(j).indice != -1){
            //recorro los nodos y veo si alguno es inconsistenteç
            cout << "-------------------------ARISTA: " << l2.getArista(j).indice << " peso: " << l2.getArista(j).peso << endl;

            //COMPARO CON PRIMER EXTREMO DE ARISTA ACTUAL
            pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).desde, l2.getArista(j).hasta, k);
            cantVecinos = pesosDeVecinos.size();
            media = calcularMedia(pesosDeVecinos, cantVecinos);
            cout << "media: " << media << endl;
            std_desv = sqrt(variance(pesosDeVecinos, media));
            cout << "std_desv: " << std_desv << endl;
            if (l2.getArista(j).peso > (200 * std_desv) && (std_desv != 0 && media!=0)) { //si es eje inconsistente
                cout << "ENCONTRE EJE INCONSISTENTE, saco arista: " <<l2.getArista(j).indice <<" de nodos:" << l2.getArista(j).desde.indice <<"("<<l2.getArista(j).desde.x<<","<<l2.getArista(j).desde.y<<")" << "--"
                     << l2.getArista(j).hasta.indice <<"("<<l2.getArista(j).hasta.x<<","<<l2.getArista(j).hasta.y<<")" <<endl;
                cantidadClusters++;

                //MODIFICO REPRESENTANTES DE LAS COMP CONEXAS
                l2.sacarArista(l2.getArista(j));
                la.sacarArista(l2.getArista(j));
                //modificar el padre de todos los nodos que son alcanzables desde uno de los extremos de la arista sacada.
                //cout <<"saque aristas"<<endl;
                padre[l2.getArista(j).desde.indice] = cantidadClusters;
                queue<Nodo> colaNodos;
                colaNodos.push(l2.getArista(j).desde);
                while (colaNodos.size() > 0 ){
                    //cout <<"colanodos.szE: "<<colaNodos.size()<<endl;
                    padre[colaNodos.front().indice] = cantidadClusters;
                    //meto en la cola los vecinos del nodo actual
                    list<AristaAd> vecinos = la[colaNodos.front()];
                    //cout <<"obtengo vecinos del nodo: "<<colaNodos.front().indice<<endl;
                    list<AristaAd>::iterator it = vecinos.begin();
                    while(it!=vecinos.end()) {
                        if(padre[(*it).adyacente.indice] != cantidadClusters && (*it).adyacente!=l2.getArista(j).hasta) colaNodos.push((*it).adyacente);
                        it++;
                    }
                    colaNodos.pop();
                }

            }
        }
/* else {
            //COMPARO CON EL OTRO EXTREMO DE LA ARISTA ACTUAL (si no hubo arista adyacente por el lado contrario)
            pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).hasta, l2.getArista(j).desde, k);
            cantVecinos = pesosDeVecinos.size();
            media = calcularMedia(pesosDeVecinos, cantVecinos);
            std_desv = variance(pesosDeVecinos, media);
            //cout << "media: " << media << endl;
            //cout << "std_desv : " << std_desv << endl;

            if (l2.getArista(j).peso > 2 * std_desv && std_desv != 0) { //si es eje inconsistente
                l2.sacarArista(l2.getArista(j));
                la.sacarArista(l2.getArista(j));
                cout << "ENCONTRE EJE INCONSISTENTE" << endl;
                cantidadClusters++;
                padre[l2.getArista(j).desde.indice] = cantidadClusters;
                queue<Nodo> colaNodos;
                colaNodos.push(l2.getArista(j).desde);
                while (colaNodos.size() > 0 ){
                    padre[colaNodos.front().indice] = cantidadClusters;
                    //meto en la cola los vecinos del nodo actual
                    list<AristaAd> vecinos = la[colaNodos.front()];
                    list<AristaAd>::iterator it = vecinos.begin();
                    while(it!=vecinos.end()) {
                        if(padre[(*it).adyacente.indice] != cantidadClusters) colaNodos.push((*it).adyacente);
                        it++;
                    }
                    colaNodos.pop();
                }

            }
        }*/
    }
}

vector<int> clusterizarDatos(ListaIncidencia& listIn, ListaAdyacencias& listAd, int kPasosVec, int cantDesv){
    vector<int> clusterDeNodo(listAd.size(), 0);
    //Pasar por todas las aristas
    for(int idArista = 0; idArista < listIn.cantidad_aristas(); idArista++){
        //Hay que ver si la arista es inconsistente o no
        //Hay que comparar el peso de la arista con las medias de pesos de los ejes
        Arista aristaActual = listIn.getArista(idArista);

        //Calculo los pesos
        float pesoAristaARevisar = aristaActual.peso;
        vector<Peso> pesosDeVecinos  = listAd.pesosVecinosDeKPasosDesdeNodo(aristaActual.hasta, aristaActual.desde, kPasosVec);
        float pesoBorde1 = calcularMedia(pesosDeVecinos, pesosDeVecinos.size());
        float desvStandar1 = sqrt(variance(pesosDeVecinos, pesoBorde1));

        pesosDeVecinos.clear();
        pesosDeVecinos = listAd.pesosVecinosDeKPasosDesdeNodo(aristaActual.hasta, aristaActual.desde, kPasosVec);
        float pesoBorde2 = calcularMedia(pesosDeVecinos, pesosDeVecinos.size());
        float desvStandar2 = sqrt(variance(pesosDeVecinos, pesoBorde2));

        //Ahora se checkea si es o no inconsistente

    }
    return  clusterDeNodo;
}

void convertirNodosAAristas(ListaIncidencia& l, vector<Nodo>& v,int k_vecinos) {


    //armo arbol completo
    int cantAg = 0;
    for (int i = 0; i < v.size(); ++i) {
        Arista ar;
        ar.desde = v[i];
        for (int j = i+1; j < v.size(); ++j) {
            ar.hasta = v[j];
            ar.peso = distancia(v[i],v[j]);
            ar.indice = cantAg;
            l.agregarArista(ar);
            cantAg++;
        }
    }
}


ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, vector<int>& padre, vector<int>& altura , int cantNodos){
    //Como es agm, m = n-1
    ListaIncidencia agm;//O(n)

    init(padre,cantNodos,altura); //O(n)
    grafo.ordenarPorPeso();//O( n * Log(n)) + O(n)
    for (int i = 0; i < grafo.cantidad_aristas(); ++i) { //O(n)*O(log n)
        //cout <<"USANDO ARISTA: "<< i <<" de: "<<grafo.cantidad_aristas()<<endl;
        if(not( find(grafo.getArista(i).desde.indice,padre,altura,0) == find(grafo.getArista(i).hasta.indice,padre,altura,0) ) ) {  //si pertenecen a diferentes comp conexas
            //agrego la arista i al agm
            agm.agregarArista(grafo.getArista(i));
            //ahora uno las comp conexas
            unir_componentes(grafo.getArista(i).desde.indice,grafo.getArista(i).hasta.indice,padre,cantNodos,altura);
        }
    }
    return agm;
}

void cargarInfo(vector<Nodo>& v, int& cn,string nombre){
    ifstream inFile;
    inFile.open("../"+ nombre+".txt");
    if (!inFile) {
        cerr << "Unable to open file datafile.txt";
        exit(1);   // call system to stop
    }
    inFile >> cn;
    cout << "CANTIDAD DE NODOS: "<<cn<<endl;
    float x,y;
    Nodo nodo;
    int cantidadIng = 0;
    v.resize(cn);
    //ahora leo todos los nodos
    while (inFile >> x >> y && cantidadIng<cn) {
        nodo.indice = cantidadIng;
        nodo.x = x;
        nodo.y = y;
        //cout <<nodo.x<<" "<< nodo.y<<endl;
        v[cantidadIng] = nodo;
        cantidadIng++;
    }
    inFile.close();
}
float variance ( vector<float>& v , float mean) {
    float sum = 0.0;
    float temp =0.0;
    if(v.size()==0) return 0;
    if(v.size()==1) return mean;
    for ( int j =0; j < v.size(); j++)
    {
        //cout <<v[j]<<"-"<<mean<<endl;
        temp = pow((v[j]-mean),  2);
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