
//#include "floyd-warshall.h"
//#include "kruskal.h"
#include "prim.h"

int main() {
    /*/
    printf("PRUEBA DE ALGO FLOYD\n");
    MatrizAdyacencias inicial(6);
    inicial[0][0] = 0;
    inicial[1][0] = 3;
    inicial[2][0] = 5;
    inicial[3][0] = 1;
    //inicial[4][0] = 0;
    //inicial[5][0] = 0;
    inicial[0][1] = 3;
    inicial[1][1] = 0;
    //inicial[2][1] = 0;
    //inicial[3][1] = 0;
    inicial[4][1] = 9;
    //inicial[5][1] = 0;
    inicial[0][2] = 5;
    //inicial[1][2] = 0;
    inicial[2][2] = 0;
    inicial[3][2] = 7;
    inicial[4][2] = 7;
    inicial[5][2] = 1;
    inicial[0][3] = 1;
    //inicial[1][3] = 0;
    inicial[2][3] = 7;
    inicial[3][3] = 0;
    //inicial[4][3] = 0;
    inicial[5][3] = 4;
    //inicial[0][4] = 0;
    inicial[1][4] = 9;
    inicial[2][4] = 7;
    //inicial[3][4] = 0;
    inicial[4][4] = 0;
    //inicial[5][4] = 0;
    //inicial[0][5] = 0;
    //inicial[1][5] = 0;
    inicial[2][5] = 1;
    inicial[3][5] = 4;
    //inicial[4][5] = 0;
    inicial[5][5] = 0;
    printf("Inicial:\n");
    inicial.print();
    MatrizAdyacencias m(floydWarshall(inicial));
    printf("\nResultado:\n");
    m.print();
    //*/

    printf("\nPRUEBA DE ALGO PRIM\n");
    ListaAdyacencias l(6);

    Nodo n0(10, 10, 0);
    Nodo n1(20, 20, 1);
    Nodo n2(40, 20, 2);
    Nodo n3(60, 10, 3);
    Nodo n4(50, 5, 4);
    Nodo n5(4, 6, 5);

    printf("\nCREADOS NODOS\n");

    Arista a1(n0, n1, 4);
    Arista a2(n0, n2, 7);
    Arista a3(n0, n5, 3);
    Arista a4(n1, n2, 3);
    Arista a5(n1, n4, 1);
    Arista a6(n2, n3, 1);
    Arista a7(n4, n2, 1);
    Arista a8(n4, n3, 4);
    Arista a9(n5, n4, 3);

    printf("\nCREADAS ARISTAS\n");

    l.agregarArista(a1);
    l.agregarArista(a2);
    l.agregarArista(a3);
    l.agregarArista(a4);
    l.agregarArista(a5);
    l.agregarArista(a6);
    l.agregarArista(a7);
    l.agregarArista(a8);
    l.agregarArista(a9);

    printf("\nCARGADAS ARISTAS\n");

//    l.agregarArista(0, 1, 4);
//    l.agregarArista(0, 2, 7);
//    l.agregarArista(0, 5, 3);
//    l.agregarArista(1, 2, 3);
//    l.agregarArista(1, 4, 1);
//    l.agregarArista(2, 3, 1);
//    l.agregarArista(4, 2, 1);
//    l.agregarArista(4, 3, 4);
//    l.agregarArista(5, 4, 3);

    //printf("LA LISTA ES: \n");

    //l.print();

    //*/
    //vector<Nodo> agm = prim(l);
    vector<Nodo> agm = primSinCola(l);
    //vector<Nodo> agm2 = prim2(l);

    printf("\nRESULTADO: \n");

    std::cout << "[";

    for(int i = 0; i < agm.size(); i++){
        std::cout << agm[i].indice  << ((i == agm.size() - 1)? ("]") : (", "));
    }

    std::cout << std::endl;
//    std::cout << "[";
//
//    for(int i = 0; i < agm2.size(); i++){
//        std::cout << agm2[i]  << ((i == agm2.size() - 1)? ("]") : (", "));
//    }
//
//    std::cout << std::endl;
    //*/
}