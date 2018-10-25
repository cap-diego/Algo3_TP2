#include "cluster.h"
#include <fstream>
#include <chrono>

#define medir_tiempo(K, CODIGO) \
    [&] () -> double {\
        double tiempo_promedio = 0.0;\
        for (int i = 0; i < K; ++i)\
        {\
            auto tiempo_inicio = chrono::steady_clock::now();\
            CODIGO \
            auto tiempo_fin = chrono::steady_clock::now();\
            tiempo_promedio += chrono::duration<double, milli>(tiempo_fin - tiempo_inicio).count();\
        }\
        return tiempo_promedio / (double)K;\
    }();


int main(int argc, char* argv[]){
    int cantidadDeNodos;
    int cantRep = 3;
    string nombreIn = "";
    vector<Nodo> input;
    int cantClusters = 0, forma=1;
    int  profundidadVecindario=1;
    float cantProm = 1.1, cantDesv = 1.8;

    cout <<"Ingrese nombre de archivo a leer (si se quiere ingresar por teclado ingresar -1): "<<endl;
    cin >> nombreIn;
    if(argc > 1 || nombreIn!="-1"){ //si la entrada es por txt
        if(argc >1) nombreIn = argv[1];
        cargarInfo(input,cantidadDeNodos, nombreIn);
    }
    if(argc==1 && nombreIn=="-1"){
        cin>>cantidadDeNodos;
        float x,y;
        Nodo nodo;
        input.resize(cantidadDeNodos);
        int cantIngresados=0;
        for (int i = 0; i < cantidadDeNodos; ++i) {
            cin >> x>>y;
            nodo.indice = cantIngresados;
            nodo.x = x;
            nodo.y = y;
            input[cantIngresados] = nodo;
            cantIngresados++;
        }
    }

    vector<int> padre(cantidadDeNodos);
    vector<int> altura(cantidadDeNodos);
    vector<Nodo> inputK(cantidadDeNodos);

    int gradoNodos=cantidadDeNodos-1;
    long long int cantidadAristasConGradoIndicado = cantidadDeNodos * gradoNodos / 2;


    cout << "Ingrese los valores del coeficiente de promedio, del coeficiente de desviación y del tamaño del vecindario y la forma que se utilizara para determinar el eje inconsistente(1 o 2) (en este orden) "<<endl;
    cin >> cantProm>>cantDesv>>profundidadVecindario>>forma;

    ListaIncidencia l1(cantidadAristasConGradoIndicado); //Creo una lista de incidencia
    MatrizAdyacencias m(cantidadDeNodos); //Creo la matriz de adyacencias
    ListaAdyacencias la(cantidadDeNodos); //Creo la lista de adyacencias
    //Genero lista de incidencia y matriz de adyancencia del grafo completo generado a partir de los puntos ingresados

    convertirNodosAAristas(l1,m, input, gradoNodos);
    m.convertirAListaAdy(la); //Cargo la lista de adyacencia

    ListaIncidencia l2(AGM_Kruskal(l1,padre,altura,cantidadDeNodos));

    ListaIncidencia l3(AGM_Kruskal_SinPathComp(l1,padre,cantidadDeNodos));
    vector<Nodo> primAGM = primSinCola(la);
    float dist;
    vector<int> padreK(cantidadDeNodos,0), padreP(cantidadDeNodos,0), padreK_SPC(cantidadDeNodos,0);
    ListaIncidencia p = ListaIncidencia(l2.cantidad_aristas());
    Arista ar;
    //armo lista de incidencia de prim
    for (int j = 1; j < primAGM.size(); ++j) {
        dist = sqrt(pow((input[j].x - primAGM[input[j].indice].x),2) + pow((input[j].y-primAGM[input[j].indice].y),2));
        ar.peso=dist;
        ar.desde=input[j];
        ar.hasta=primAGM[input[j].indice];
        p.agregarArista(ar);
    }

    //Comienzo a sacar ejes
    //Parametros a modificar
    //forma 1: ambas,forma:2 por por proporcion

    retirarEjesInconsistentes(l2,cantidadDeNodos,cantDesv,padreK,cantClusters, forma,cantProm, profundidadVecindario);
    cantClusters = 0;
    retirarEjesInconsistentes(l3,cantidadDeNodos,cantDesv,padreK_SPC,cantClusters,forma,cantProm,profundidadVecindario);
    vector<bool > c(cantClusters+1,false);
    for (int i = 0; i < padreK_SPC.size(); ++i) {
        c[padreK_SPC[i]] = true;
    }
    int acum = 0;
    for (int k = 0; k < c.size(); ++k) {
        if(c[k]) acum++;
    }
    cout <<"Clusterizacion Kruskal sin path comp"<<endl;
    for(int i = 0; i < padreK_SPC.size();i++){
        padreK_SPC[i] = (cantClusters - padreK_SPC[i]);
        cout << "Nodo: " << i << " cluster: "<<padreK_SPC[i]<<endl;
    }
    c.clear();
    c.resize(cantClusters+1,false);
    for (int i = 0; i < padreK.size(); ++i) {
        c[padreK[i]] = true;
    }
    acum = 0;
    for (int k = 0; k < c.size(); ++k) {
        if(c[k]) acum++;
    }
    cout <<"Clusterización Kruskal"<<endl;
    for(int i = 0; i < padreK.size();i++){
        padreK[i] = (cantClusters - padreK[i]);
        cout << "Nodo: " << i << " cluster: "<<padreK[i]<<endl;
    }
    exportarNodos(input,nombreIn+"_nodosKruskal_SinPathComp",padreK_SPC);
    exportarNodos(input,nombreIn+"_nodosKruskal",padreK);
    cantClusters=0;
    retirarEjesInconsistentes(p,cantidadDeNodos,cantDesv,padreP, cantClusters, forma,cantProm ,profundidadVecindario);
    c.clear();
    c.resize(cantClusters+1, false);
    for (int i = 0; i < padreP.size(); ++i) {
        c[padreP[i]] = true;
    }
    acum = 0;
    for (int k = 0; k < c.size(); ++k) {
        if(c[k]) acum++;
    }
    cout <<"Clusterización Prim"<<endl;
    for(int i = 0; i < padreP.size();i++){
        padreP[i] = (cantClusters - padreP[i]);
        cout << "Nodo: " << i << " cluster: "<<padreP[i]<<endl;
    }
    exportarNodos(input,nombreIn+"_nodosPrim", padreP);


    return 0;
}


bool todosVisitados(vector<bool>& v){
    for(int i = 0; i < v.size(); i++){
        if(!v[i]){
            //Hay uno falso, no se visitaron todos
            return false;
        }
    }

    //Se visitaron todos
    return true;
}
class compare {
public:
    bool operator()(const AristaAd& a, const AristaAd& b) {
        return a.peso > b.peso;
    }
};

template<typename T>
float calcularMedia(vector<T>& vec) {
    T sum= 0;
    if(vec.size()==0)return 0;
    if(vec.size()==1) return float(vec[0]);
    for(auto v : vec) {
        sum+=v;
    }
    return sum/vec.size();
}

float distancia(Nodo& n1, Nodo& n2){
    return(sqrt(pow(n1.x-n2.x,2)+ pow(n1.y- n2.y,2)) );
}

void exportarNodos(vector<Nodo>& vec, string nombre, vector<int>& padre) {
    ofstream myfile, myfile2;
    myfile.open ("../grafos/" + nombre + ".csv");
    myfile2.open ("grafos/" + nombre + ".csv");
    for (int i = 0; i < vec.size(); ++i) {
        if(vec[i].indice!=-1) myfile<<vec[i].indice<<","<<vec[i].x<<","<<vec[i].y<<","<<padre[i] <<endl;
        if(vec[i].indice!=-1) myfile2<<vec[i].indice<<","<<vec[i].x<<","<<vec[i].y<<","<<padre[i] <<endl;
    }
    cout <<"GENERADO lista de nodos: "<<nombre<<endl;
    myfile.close();
    myfile2.close();
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

void retirarEjesInconsistentes(ListaIncidencia& l2, int cantidadDeNodos, float cantDesv, vector<int>& padre, int& cantidadClusters,int forma, float cantProm, int profundidadVecindario){
    //necesito recorrer los ejes y verificar cuales cumplen la nocion de eje inconsistente
    //1) nocion es : cuantas desviaciones estandar hay entre el peso de un eje y la media en el vecindario de sus nodos
    //2) nocion es: calcular la proporcion entre el peso de un eje y las medias en los vecindarios de sus nodos
    //param importantes: media, desv_standar, vecindario de un nodo,
    ListaAdyacencias la(cantidadDeNodos);
    l2.convertirALista(la);
    float media = 0, std_desv = 0, mediaExt1, mediaExt2;
    vector<Peso> pesosDeVecinos;
    float proporcionEntreEjeYProm = cantDespuesDeComa(cantProm,profundidadVecindario), proporcionExt1, proporcionExt2;
    int d = profundidadVecindario;
    for (int e = 0; e < padre.size(); ++e) {
        padre[e]=0;
    }
    int ca=l2.cantidad_aristas();
    int digDspDeComa = 1;
    float factorDsvExt1, factorDsvExt2;
    for (int j = 0; j < ca; ++j) {//O(M)
        //Recorro las aristas y analizo si hay inconsistentes
        Arista ar= l2.getArista(j);
        if(ar.indice!=-1){
        //if (l2.getArista(j).indice != -1 && ar.desde.x <= 15 && ar.hasta.x<=15 && ar.desde.y <= 10 && ar.hasta.y <=10 && ar.desde.x >0 && ar.hasta.x>=0 &&ar.desde.y>=0 && ar.hasta.y>=0 ){ //Si la arista existe (cuando la eliminamos, en realidad la deshabilitamos)
            //Consigo los pesos de los vecinos a 'd' pasos de aristas desde l2.getArista(j).desde y que NO pasen por l2.getArista(j), es decir, consigo el vecindario de uno de los extremos de la arista
            //cout << "arista "<< ar.indice<<" de ("<<ar.desde.x<<","<<ar.desde.y<<")-("<<ar.hasta.x<<","<<ar.hasta.y<<")"<<endl;
            if(forma== 1) {
                float media1, media2;
                pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).desde, l2.getArista(j).hasta, d);//extremo 1
                media1 = cantDespuesDeComa(calcularMedia(pesosDeVecinos),digDspDeComa);
                factorDsvExt1 = cantDespuesDeComa((media1+(cantDesv * desviacion_std(pesosDeVecinos, media1))),digDspDeComa);
                mediaExt1 = cantDespuesDeComa(l2.getArista(j).peso/media1,digDspDeComa);
                pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).hasta, l2.getArista(j).desde, d); //extremo 2
                media2 = cantDespuesDeComa(calcularMedia(pesosDeVecinos),digDspDeComa);
                mediaExt2 = cantDespuesDeComa(l2.getArista(j).peso/media2,digDspDeComa);
                factorDsvExt2 = cantDespuesDeComa(media2+((cantDesv * desviacion_std(pesosDeVecinos,media2))),digDspDeComa);

                if(media1!=0 && media2 != 0 && l2.getArista(j).peso > factorDsvExt1  &&  l2.getArista(j).peso > factorDsvExt2 && mediaExt1 > cantProm && mediaExt2 > cantProm){
                    l2.sacarArista(l2.getArista(j));
                    la.sacarArista(l2.getArista(j));
                    //modificar el padre de todos los nodos que son alcanzables desde uno de los extremos de la arista sacada.
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
                            if(padre[(*it).adyacente.indice] != cantidadClusters) colaNodos.push((*it).adyacente);
                            it++;
                        }
                        colaNodos.pop();
                    }
                    cantidadClusters++;
                    colaNodos.push(l2.getArista(j).hasta);
                    while (colaNodos.size() > 0) {
                        padre[colaNodos.front().indice] = cantidadClusters;
                        //meto en la cola los vecinos del nodo actual
                        list<AristaAd> vecinos = la[colaNodos.front()];
                        list<AristaAd>::iterator it = vecinos.begin();
                        while (it != vecinos.end()) {
                            if (padre[(*it).adyacente.indice] != cantidadClusters) colaNodos.push((*it).adyacente);
                            it++;
                        }
                        colaNodos.pop();
                    }
                    cantidadClusters++;
                }


            }else {
                float pesoArista = 0.0;
                pesoArista = cantDespuesDeComa(l2.getArista(j).peso, digDspDeComa);
                pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).desde, l2.getArista(j).hasta, d);
                mediaExt1 = cantDespuesDeComa(calcularMedia(pesosDeVecinos), digDspDeComa);
                proporcionExt1 = cantDespuesDeComa(pesoArista / mediaExt1,digDspDeComa);
                pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).hasta, l2.getArista(j).desde, d);
                mediaExt2 = cantDespuesDeComa(calcularMedia(pesosDeVecinos), digDspDeComa);
                proporcionExt2 = cantDespuesDeComa(pesoArista / mediaExt2,
                                                   digDspDeComa);//media*100/l2.getArista(j).peso;
                if (forma == 2) {

                    if ((mediaExt1 != 0 && mediaExt2 != 0) && (proporcionExt1 > proporcionEntreEjeYProm &&
                                                               proporcionExt2 >
                                                               proporcionEntreEjeYProm)) { //si es eje inconsistente
                       /* cout << "ENCONTREEJE INCONSISTENTE, saco arista: " << l2.getArista(j).indice << " de nodos:"
                             << l2.getArista(j).desde.indice << "(" << l2.getArista(j).desde.x << ","
                             << l2.getArista(j).desde.y << ")" << "--"
                             << l2.getArista(j).hasta.indice << "(" << l2.getArista(j).hasta.x << ","
                             << l2.getArista(j).hasta.y << ")" << endl;*/


                        //MODIFICO REPRESENTANTES DE LAS COMP CONEXAS
                        l2.sacarArista(l2.getArista(j));
                        la.sacarArista(l2.getArista(j));
                        //modificar el padre de todos los nodos que son alcanzables desde uno de los extremos de la arista sacada.
                        queue<Nodo> colaNodos;
                        colaNodos.push(l2.getArista(j).desde);
                        while (colaNodos.size() > 0) {
                            padre[colaNodos.front().indice] = cantidadClusters;
                            //meto en la cola los vecinos del nodo actual
                            list<AristaAd> vecinos = la[colaNodos.front()];
                            list<AristaAd>::iterator it = vecinos.begin();
                            while (it != vecinos.end()) {
                                if (padre[(*it).adyacente.indice] != cantidadClusters) colaNodos.push((*it).adyacente);
                                it++;
                            }
                            colaNodos.pop();
                        }
                        cantidadClusters++;
                        colaNodos.push(l2.getArista(j).hasta);
                        while (colaNodos.size() > 0) {
                            padre[colaNodos.front().indice] = cantidadClusters;
                            //meto en la cola los vecinos del nodo actual
                            list<AristaAd> vecinos = la[colaNodos.front()];
                            list<AristaAd>::iterator it = vecinos.begin();
                            while (it != vecinos.end()) {
                                if (padre[(*it).adyacente.indice] != cantidadClusters) colaNodos.push((*it).adyacente);
                                it++;
                            }
                            colaNodos.pop();
                        }
                        cantidadClusters++;
                    }
                }
            }

        }
    }
}


void convertirNodosAAristas(ListaIncidencia& l, MatrizAdyacencias& m, vector<Nodo>& v,int k_vecinos) {
    //armo arbol completo
    if(k_vecinos == v.size()-1) {
        int cantAg = 0;
        for (int i = 0; i < v.size(); ++i) {
            Arista ar;
            //cout <<"nodo desde indice: "<<v[i].indice<<endl;
            ar.desde = v[i];
            for (int j = i+1; j < v.size(); ++j) {

                ar.hasta = v[j];
                ar.peso = distancia(v[i], v[j]);
                ar.indice = cantAg;
                //cout <<"agrego arista: "<<ar.desde.indice<<","<<ar.hasta.indice<<endl;
                l.agregarArista(ar);
                m.agregarArista(ar);
                //cout <<"indice nodo desde de la arista agregada: "<<l.getArista(cantAg).desde.indice<<"vs indice nodo desde ag: "<<v[i].indice<<endl;
                cantAg++;

            }
        }
    } else{ //no se usa al final
        //armo arbol de k_vecinos c/a nodo
        Arista ar;
        for (int i = 0; i < v.size(); ++i) {
            //conecto con el anterior
            if(i!=0) {
                ar.desde = v[i-1];
                ar.desde.indice=v[i-1].indice;
                ar.peso = distancia(v[i-1], v[i]);
                ar.hasta = v[i];
                ar.hasta.indice = v[i].indice;
                m.agregarArista(ar);
                //l.agregarArista(ar);
            }
            ar.desde = v[i];
            ar.desde.indice = v[i].indice;
            for (int j = 0; j < k_vecinos-1 && j+i+1 <v.size(); ++j) {
                ar.peso = distancia(v[j+i+1], v[i]);
                ar.hasta = v[j+i+1];
                ar.hasta.indice = v[j+i+1].indice;
                m.agregarArista(ar);
                //l.agregarArista(ar);
            }
        }

    }
}

ListaIncidencia AGM_Kruskal(ListaIncidencia& grafo, vector<int>& padre, vector<int>& altura , int cantNodos){
    //Como es agm, m = n-1
    ListaIncidencia agm;//O(n)
    init(padre,cantNodos,altura); //O(n)
    grafo.ordenarPorPeso();//O( m * Log(m)) + O(m)
    for (int i = 0; i < grafo.cantidad_aristas(); ++i) { //O(m)*O(log n)
        //cout <<"USANDO ARISTA: "<< i <<" de: "<<grafo.cantidad_aristas()<<endl;
        if(not( find(grafo.getArista(i).desde.indice,padre,altura,0) == find(grafo.getArista(i).hasta.indice,padre,altura,0) ) ) {  //si pertenecen a diferentes comp conexas
            //ahora uno las comp conexas
            agm.agregarArista(grafo.getArista(i));
            unir_componentes(grafo.getArista(i).desde.indice,grafo.getArista(i).hasta.indice,padre,cantNodos,altura);
        }
    }
    return agm;
}

ListaIncidencia AGM_Kruskal_SinPathComp(ListaIncidencia& grafo, vector<int>& padre, int cantNodos){
    //Como es agm, m = n-1
    ListaIncidencia agm;//O(n)
    init_Sin_PathComp(padre); //O(n)
    grafo.ordenarPorPeso();//O( m * Log(m)) + O(m)
    for (int i = 0; i < grafo.cantidad_aristas(); ++i) { //O(m)*O(log n)
        if(not(find_Sin_PathCompression(grafo.getArista(i).desde.indice,padre) == find_Sin_PathCompression(grafo.getArista(i).hasta.indice,padre))) {
            agm.agregarArista(grafo.getArista(i));
            unir_componentes_Sin_PathComp(grafo.getArista(i).desde.indice,grafo.getArista(i).hasta.indice,padre);
        }
    }
    return agm;
}

void cargarInfo(vector<Nodo>& v, int& cn,string nombre){
    ifstream inFile;
    inFile.open("../in/"+ nombre+".txt");//si se usa clion
    if (!inFile) {
        inFile.open("in/"+ nombre+".txt");//si es de linea de comando
        if(!inFile) {
            cerr << "ERROR ABRIENDO EL ARCHIVO DE TEXTO INGRESADO";
            exit(1);   // call system to stop
        }
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
float desviacion_std(vector<float> &v, float mean) {
    float sum = 0;
    float temp = 0;
    if(v.size()==0) return 0;
    for ( int j =0; j < v.size(); j++)
    {
        //cout <<v[j]<<"-"<<mean<<endl;
        temp = pow((v[j]-mean),  2);
        sum += temp;
    }

    return sqrt(sum/v.size());
}

void init(vector<int>& padre, int cantNodos, vector<int>& altura){
    for (int i = 0; i < cantNodos; ++i) {
        altura[i] = 1;
        padre[i] = i;
    }
}
void init_Sin_PathComp(vector<int>& padre){
    for (int i = 0; i < padre.size(); ++i) {
        padre[i] = i;
    }
}

int find(int indiceNodo, vector<int>& padre, vector<int>& altura, int subidos) { //devuelvo  el indice del nodo que es rep
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

int find_Sin_PathCompression(int indiceNodo, vector<int>& padre) {
    if(padre[indiceNodo] == indiceNodo) { //si el rep del nodo es él mismo
        return indiceNodo;
    }else if(padre[indiceNodo] != indiceNodo){
        return find_Sin_PathCompression(padre[indiceNodo],padre);//devolvemos el representante sin actualizar el padre, de modo que siempre busqueemos este rep desde alguna parte que no sea el mismo vamos a hacer este recorrido
    }
}

void unir_componentes_Sin_PathComp(int indiceNodo1, int indiceNodo2, vector<int>& padre){
    int indicePadreNodo1 = find_Sin_PathCompression(indiceNodo1,padre);
    int indicePadreNodo2 = find_Sin_PathCompression(indiceNodo2,padre);
    padre[indicePadreNodo1] = indicePadreNodo2;
}

vector<Nodo> primSinCola(ListaAdyacencias& G){
    vector<bool> visitados = vector<bool>(G.size(), false);
    vector<float> distacia = vector<float>(G.size(), std::numeric_limits<float>::max());
    vector<Nodo> padres = vector<Nodo>(G.size()); //Si el padre es -1 entonces el nodo no tiene padre

    //srand (time(NULL));
    //Agarro un nodo al azar y actualizo las distancias con las de sus ejes
    Nodo first = G.getNodo(0);

    for(auto elem : G[first]){
        distacia[elem.adyacente.indice] = elem.peso;
        padres[elem.adyacente.indice] = first;
    }

    //Ahora se hace el resto del cálculo
    distacia[first.indice] = 0;
    visitados[first.indice] = true;

    //
    while(!todosVisitados(visitados)){
        //Elijo el menor que no esté visitado
        Nodo v = menorNoVisitado(G.getNodos(), distacia, visitados);
        visitados[v.indice] = true;

        for(auto elem : G[v]){
            if(!visitados[elem.adyacente.indice]){
                if(distacia[elem.adyacente.indice] > elem.peso){
                    distacia[elem.adyacente.indice] = elem.peso;
                    padres[elem.adyacente.indice] = v;
                }
            }
        }
    }

    return padres;
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

Nodo menorNoVisitado(vector<Nodo> nodos, vector<float>& distancia, vector<bool>& visitados){

    Nodo min = Nodo(0, 0, -1);
    float minDist = -1;

    for(int i = 0; i < distancia.size(); i++){
        if(!visitados[i]){
            if(min.indice == -1){
                minDist = distancia[i];
                min = nodos[i];
            }
            else{
                if(minDist > distancia[i]){
                    minDist = distancia[i];
                    min = nodos[i];
                }
            }
        }
    }

    return min;
}

vector<Nodo> prim2(ListaAdyacencias& G){
    // Inicializacion
    vector<Peso> res(G.size(), INT_MAX);
    vector<Nodo> padres(G.size());
    vector<bool> visitados(G.size(), false);

    printf("\nPASADO TEST 1\n");

    priority_queue<AristaAd, vector<AristaAd>, compare> cola;

    printf("\nPASADO TEST 2\n");

    Nodo pN = G.getNodo(0);

    printf("\nPASADO TEST 2.5\n");

    AristaAd primerNodo(pN, 0);

    cola.push(primerNodo);

    printf("\nPASADO TEST 3\n");

    while (!cola.empty()){
        AristaAd u = cola.top();
        cola.pop();
        visitados[u.adyacente.indice] = true;
        for(AristaAd ad : G[u.adyacente]){
            if(!visitados[ad.adyacente.indice]){
                cola.push(ad);
                if(res[ad.adyacente.indice] > ad.peso){
                    res[ad.adyacente.indice] = ad.peso;
                    padres[ad.adyacente.indice] = u.adyacente;
                }
            }
        }
    }

    return padres;
}