#include "kruskal.h"
#include <fstream>


int main() {

    int cantidadDeNodos;
    vector<Nodo> input;
    cargarInfo(input,cantidadDeNodos, "g3000");
    vector<int> padre(cantidadDeNodos);//O(n)
    vector<int> altura(cantidadDeNodos); //O(n)
    vector<Nodo> inputK(cantidadDeNodos);

    int gradoNodos=cantidadDeNodos-1;
    cout <<"prepara para generar grafo con el que despues voy a trabajar"<<endl;

    long long int cantidadAristasConGradoIndicado = cantidadDeNodos * gradoNodos / 2;
    cout <<"Cantidad de aristas indicado: "<< cantidadAristasConGradoIndicado<<endl;
    ListaIncidencia l1(cantidadAristasConGradoIndicado); //Creo una lista de incidencia
    cout <<"lista lista"<<endl;
    MatrizAdyacencias m(cantidadDeNodos); //Creo la matriz de adyacencias
    ListaAdyacencias la(cantidadDeNodos); //Creo la lista de adyacencias
    convertirNodosAAristas(l1,m, input, gradoNodos);
    if(gradoNodos!=cantidadDeNodos-1) { //si el grafo que genere no era completo, entonces use la matriz para crearlo, lo paso a lista inc
        m.convertirAListaInc(l1);
    } //si hice un grafo completo entonces ambas estan act


    cout <<"Se ha generado el grafo completo de: "<<l1.cantidad_aristas()<<" aristas" <<endl;

    //Generamos el AGM
    ListaIncidencia l2 =AGM_Kruskal(l1,padre,altura,cantidadDeNodos);
    cout <<"Se genera el agm de kruskal de "<<l2.cantidad_aristas()<<" aristas"<<endl;
    l2.mostrar();


    m.convertirAListaAdy(la); //Creo lista ady
    vector<Nodo> primAGM = primSinCola(la);
    cout <<"Generado prim de "<< primAGM.size()-1<<" aristas" <<endl;
    float dist;
    vector<int> padreK(cantidadDeNodos,0), padreP(cantidadDeNodos,0);
    ListaIncidencia p = ListaIncidencia(l2.cantidad_aristas());
    Arista ar;
    //armo lista de incidencia de prim
    for (int j = 1; j < primAGM.size(); ++j) {
        dist = sqrt(pow((input[j].x - primAGM[input[j].indice].x),2) + pow((input[j].y-primAGM[input[j].indice].y),2));
        ar.peso=dist;
        ar.desde=input[j];
        ar.hasta=primAGM[input[j].indice];
        p.agregarArista(ar);
        //cout <<"AGREGO ARISTA: "<<cantAg<< " a PRIM  ("<<ar.desde.indice<<","<<ar.hasta.indice<<")"<<endl;
    }


    //Comienzo a sacar ejes
    cout<<"Se comienza a sacar aristas inconsistentes "<<endl;

    //Parametros a modificar
    int cantClusters = 0, forma =1;
    int  profundidadVecindario=2;
    float cantProm =2.15,cantDesv = 2.8;

    retirarEjesInconsistentes(l2,cantidadDeNodos,cantDesv,padreK,cantClusters, forma,cantProm, profundidadVecindario);


    cout <<"clusters sin mover de indice: "<<cantClusters<<endl;

    vector<bool > c(cantClusters+1,false);
    for (int i = 0; i < padreK.size(); ++i) {
       c[padreK[i]] = true;
    }
    int acum = 0;
    for (int k = 0; k < c.size(); ++k) {
        if(c[k]) acum++;
    }
    for(int i = 0; i < padreK.size();i++){
        padreK[i] = acum - padreK[i];
    }
    exportarNodos(input,"nodosK",padreK);
    cout <<"hay "<<acum<< " clusters de AGM de kruskal"<<endl;
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
    for(int i = 0; i < padreP.size();i++){
        padreP[i] = acum - padreP[i];
    }
    cout <<"hay "<<acum<< " clusters de AGM de prim"<<endl;
    exportarNodos(input,"nodosP", padreP);


    cout <<"fin"<<endl;
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
    int ca=l2.cantidad_aristas();
    int digDspDeComa = 2;
    float factorDsvExt1, factorDsvExt2;
    for (int j = 0; j < ca; ++j) {//O(M)
        //Recorro las aristas y analizo si hay inconsistentes

        if (l2.getArista(j).indice != -1){ //Si la arista existe (cuando la eliminamos, en realidad la deshabilitamos)
            cout << "-------------------------ARISTA: " << l2.getArista(j).indice <<" de nodos:" << l2.getArista(j).desde.indice <<"("<<l2.getArista(j).desde.x<<","<<l2.getArista(j).desde.y<<")" << "--"
                 << l2.getArista(j).hasta.indice <<"("<<l2.getArista(j).hasta.x<<","<<l2.getArista(j).hasta.y<<") peso: " <<l2.getArista(j).peso<<endl;
            //Consigo los pesos de los vecinos a 'd' pasos de aristas desde l2.getArista(j).desde y que NO pasen por l2.getArista(j), es decir, consigo el vecindario de uno de los extremos de la arista

            if(l2.getArista(j).desde.x > 20){
           //     cout << "-------------------------ARISTA: " << l2.getArista(j).indice <<" de nodos:" << l2.getArista(j).desde.indice <<"("<<l2.getArista(j).desde.x<<","<<l2.getArista(j).desde.y<<")" << "--"
                    // << l2.getArista(j).hasta.indice <<"("<<l2.getArista(j).hasta.x<<","<<l2.getArista(j).hasta.y<<") peso: " <<l2.getArista(j).peso<<endl;
            }
            if(forma== 1) {
                float media1, media2;
                pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).desde, l2.getArista(j).hasta, d);//extremo 1
                media1 = cantDespuesDeComa(calcularMedia(pesosDeVecinos),digDspDeComa);
                factorDsvExt1 = cantDespuesDeComa((media1+(cantDesv * desviacion_std(pesosDeVecinos, media1))),digDspDeComa);
                mediaExt1 = cantDespuesDeComa(l2.getArista(j).peso/media1,digDspDeComa);

                /*for (int i = 0; i < pesosDeVecinos.size(); ++i) {
                    cout <<"peso "<<i<<"  "<<pesosDeVecinos[i]<<endl;
                }*/
                pesosDeVecinos = la.pesosVecinosDeKPasosDesdeNodo(l2.getArista(j).hasta, l2.getArista(j).desde, d); //extremo 2
                media2 = cantDespuesDeComa(calcularMedia(pesosDeVecinos),digDspDeComa);
                mediaExt2 = cantDespuesDeComa(l2.getArista(j).peso/media2,digDspDeComa);
                factorDsvExt2 = cantDespuesDeComa(media2+((cantDesv * desviacion_std(pesosDeVecinos,media2))),digDspDeComa);

                //cout <<"peso candidato: "<< l2.getArista(j).peso <<", factordsv1: "<<media<<"*"<<cantDesv<<"*"<< desviacion_std(pesosDeVecinos,media)<<"   PROM1: "<< mediaExt1<<endl;
                if(media1!=0 && media2 != 0 && l2.getArista(j).peso > factorDsvExt1  &&  l2.getArista(j).peso > factorDsvExt2 && mediaExt1 > cantProm && mediaExt2 > cantProm){
                    //ARISTA INCONSISTENTE CON DETERMINACION FORMA 1
                    cout<<"inconsistente"<<endl;
                    cout << "-------------------------ARISTA: " << l2.getArista(j).indice <<" de nodos:" << l2.getArista(j).desde.indice <<"("<<l2.getArista(j).desde.x<<","<<l2.getArista(j).desde.y<<")" << "--"
                         << l2.getArista(j).hasta.indice <<"("<<l2.getArista(j).hasta.x<<","<<l2.getArista(j).hasta.y<<") peso: " <<l2.getArista(j).peso<<endl;
                    cout <<"peso candidato: "<< l2.getArista(j).peso <<", factordsv1: "<<media1<<"+"<<cantDesv<<"*"<< desviacion_std(pesosDeVecinos,media)<<"   PROM1: "<< mediaExt1<<endl;

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
                        cout << "ENCONTREEJE INCONSISTENTE, saco arista: " << l2.getArista(j).indice << " de nodos:"
                             << l2.getArista(j).desde.indice << "(" << l2.getArista(j).desde.x << ","
                             << l2.getArista(j).desde.y << ")" << "--"
                             << l2.getArista(j).hasta.indice << "(" << l2.getArista(j).hasta.x << ","
                             << l2.getArista(j).hasta.y << ")" << endl;


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

                    }
                }
            }

        }
    }
}


void convertirNodosAAristas(ListaIncidencia& l, MatrizAdyacencias& m, vector<Nodo>& v,int k_vecinos) {
    //armo arbol completo
    if(k_vecinos == v.size()-1) {
        //cout <<"entra aca"<<endl;
        int cantAg = 0;
        for (int i = 0; i < v.size(); ++i) {
            Arista ar;
            //cout <<"nodo desde indice: "<<v[i].indice<<endl;
            ar.desde = v[i];
            for (int j = i+1; j < v.size(); ++j) {
                //cout <<"a"<<endl;
                ar.hasta = v[j];
                ar.peso = distancia(v[i],v[j]);
                ar.indice = cantAg;
                //cout <<"agrego arista: "<<ar.desde.indice<<","<<ar.hasta.indice<<endl;
                l.agregarArista(ar);
                m.agregarArista(ar);
                //cout <<"indice nodo desde de la arista agregada: "<<l.getArista(cantAg).desde.indice<<"vs indice nodo desde ag: "<<v[i].indice<<endl;
                cantAg++;
            }
        }
    } else{
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
    for (int i = 0; i < grafo.cantidad_aristas(); ++i) { //O(n)*O(log n)
        //cout <<"USANDO ARISTA: "<< i <<" de: "<<grafo.cantidad_aristas()<<endl;
        if(not( find(grafo.getArista(i).desde.indice,padre,altura,0) == find(grafo.getArista(i).hasta.indice,padre,altura,0) ) ) {  //si pertenecen a diferentes comp conexas
            //agrego la arista i al agm
            //cout <<"agrego arista: "<< grafo.getArista(i).indice<<"de nodos: "<< grafo.getArista(i).desde.indice<<"-"<<grafo.getArista(i).hasta.indice<<endl;
            agm.agregarArista(grafo.getArista(i));
            //ahora uno las comp conexas
            unir_componentes(grafo.getArista(i).desde.indice,grafo.getArista(i).hasta.indice,padre,cantNodos,altura);
        }
    }
    return agm;
}

void cargarInfo(vector<Nodo>& v, int& cn,string nombre){
    ifstream inFile;
    inFile.open("../in/"+ nombre+".txt");
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
    float ax;
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


int find_Sin_Path_Compression(int indiceNodo, vector<int>& padre, vector<int>& altura, int subidos) {
    if(padre[indiceNodo] == indiceNodo) { //si el rep del nodo es él mismo
        altura[indiceNodo] = subidos;
        return indiceNodo;
    }else if(padre[indiceNodo] != indiceNodo){
        return find(padre[indiceNodo],padre,altura,1+subidos);
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