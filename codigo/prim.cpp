#include "prim.h"
#include <vector>
#include <random>
#include <utility>
#include <time.h>
#include <queue>

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

Nodo menorNoVisitado(vector<int>& distancia, vector<bool>& visitados){
    Nodo min = -1;
    int minDist = -1;

    for(int i = 0; i < distancia.size(); i++){
        if(!visitados[i]){
            if(min == -1){
                minDist = distancia[i];
                min = i;
            }
            else{
                if(minDist > distancia[i]){
                    minDist = distancia[i];
                    min = i;
                }
            }
        }
    }

    return min;
}

/*/
//Realiza el algoritmo de prim
//Devuelve el vector de padres que representa uno de los AGM del grafo
vector<Nodo> prim(ListaAdyacencias G){
    vector<bool> visitados = vector<bool>(G.size(), false);
    vector<int> distacia = vector<int>(G.size(), std::numeric_limits<int>::max());
    vector<Nodo> padres = vector<Nodo>(G.size(), -1); //Si el padre es -1 entonces el nodo no tiene padre

    //srand (time(NULL));
    //Agarro un nodo al azar y actualizo las distancias con las de sus ejes
    Nodo first = 0;
    for(int i = 0; i < G[first].size(); i++){
        //G[first][i]
        distacia[G[first][i].adyacente] = G[first][i].peso;
        padres[G[first][i].adyacente] = first;
    }

    //Ahora se hace el resto del cálculo
    distacia[first] = 0;
    visitados[first] = true;

    //
    while(!todosVisitados(visitados)){
        //Elijo el menor que no esté visitado
        Nodo v = menorNoVisitado(distacia, visitados);
        visitados[v] = true;

        //Actualizo las distancias
        for(int i = 0; i < G[v].size(); i++){
            if(!visitados[G[v][i].adyacente]){
                if(distacia[G[v][i].adyacente] > G[v][i].peso){
                    distacia[G[v][i].adyacente] = G[v][i].peso;
                    padres[G[v][i].adyacente] = v;
                }
            }
        }
    }

    return padres;
}
//*/

class compare {
public:
    bool operator()(const AristaAd& a, const AristaAd& b) {
        return a.peso > b.peso;
    }
};

/*/
vector<Nodo> prim2(ListaAdyacencias G){
    // Inicializacion
    vector<Peso> res(G.size(), INT_MAX);
    vector<Nodo> padres(G.size(), -1);
    vector<bool> visitados(G.size(), false);

    priority_queue<AristaAd, vector<AristaAd>, compare> cola;

    cola.push(AristaAd(0,0));

    while (!cola.empty()){
        AristaAd u = cola.top();
        cola.pop();
        visitados[u.adyacente] = true;
        for(AristaAd ad : G[u.adyacente]){
            if(!visitados[ad.adyacente]){
                cola.push(ad);
                if(res[ad.adyacente] > ad.peso){
                    res[ad.adyacente] = ad.peso;
                    padres[ad.adyacente] = u.adyacente;
                }
            }
        }
    }

    return padres;
}
//*/

vector<Nodo> prim(ListaAdyacencias G){
    // Inicializacion
    vector<Peso> res(G.size(), INT_MAX);
    vector<Nodo> padres(G.size(), -1);
    vector<bool> visitados(G.size(), false);

    priority_queue<AristaAd, vector<AristaAd>, compare> cola;

    cola.push(AristaAd(G.getNodo(0),0));

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