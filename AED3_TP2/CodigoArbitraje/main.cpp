#include "floyd-warshall.h"
#include "bellman-ford.h"
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

void execute(const char* filename, bool verbose = true, char* only = "all") {
    int n;
    vector<int> S;
    // Leo los datos de archivo o de consola
    if (filename != NULL) {
        if (verbose)
            cout << "Procesando datos del test " << filename << endl;
        freopen(filename, "r", stdin);
    }
    cin >> n;
    MatrizAdyacencias m(n);
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; ++y) {
            Peso temp; cin >> temp;
            m[x][y] = temp;
        }
    }
    if (filename != NULL) {
        fclose(stdin);
    }
    // Calculo los tiempos
    double t1, t2;
    if (strcmp(only,"BellmanFord") == 0 || strcmp(only,"all") == 0)
        t1 = medir_tiempo(2, problemaArbitrajeBF(m););
    if (strcmp(only,"FloydWarshall") == 0 || strcmp(only,"all") == 0)
        t2 = medir_tiempo(2, problemaArbitrajeFW(m););
    if (verbose) {
        if (strcmp(only,"BellmanFord") == 0 || strcmp(only,"all") == 0) {
            cout << "Tiempo de BellmanFord: " << t1 << " milisegundos" << endl;
            vector<int> r = problemaArbitrajeBF(m);
            cout << "Resultado de BellmanFord: ";
            if (r.size() == 0) cout << "No" << endl;
            else {
                cout << "Si";
                for (int x : r) {
                    cout << " " << x;
                }
                cout << endl;
            }
        }
        if (strcmp(only,"FloydWarshall") == 0 || strcmp(only,"all") == 0) {
            cout << "Tiempo de FloydWarshall: " << t2 << " milisegundos" << endl;
            vector<int> r = problemaArbitrajeFW(m);
            cout << "Resultado de FloydWarshall: ";
            if (r.size() == 0) cout << "No" << endl;
            else {
                cout << "Si";
                for (int x : r) {
                    cout << " " << x;
                }
                cout << endl;
            }
        }
    } else {
        // Guardo los tiempos en archivos
        FILE* f1 = fopen("TiemposBellmanFord", "a");
        FILE* f2 = fopen("TiemposFloydWarshall", "a");
        if (strcmp(only,"BellmanFord") == 0 || strcmp(only,"all") == 0)
            fprintf(f1, "%f\n", t1);
        if (strcmp(only,"FloydWarshall") == 0 || strcmp(only,"all") == 0)
            fprintf(f2, "%f\n", t2);
        fclose(f1); fclose(f2);
    }
}


void mostrarSolucion(vector<int> s, vector<int> expected, int case_number) {
    cout << "<<<<<<< CASO " << case_number << " <<<<<<<" << endl;
    bool correct = s.size() == expected.size();
    if(s.size()== 0){
        cout << "vacio" << endl;
    }else {
        for (int i = 0; i < s.size(); i++) {
            cout << s[i] ;
            if (i != s.size()-1) cout << " ";
            if (s[i] != expected[i]) correct = false;
        }
        cout << endl;
    }
    cout << "Resultado " << (correct ? "correcto" : "incorrecto") << endl;
}

void test1(bool mode) {
    MatrizAdyacencias matriz(4);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[3][3] = 1;
    matriz[0][1] = 36.64;
    matriz[0][2] = 0.863;
    matriz[0][3] = 0.760;
    matriz[1][0] = 0.027;
    matriz[1][2] = 0.023;
    matriz[1][3] = 0.020;
    matriz[2][0] = 1.159;
    matriz[2][1] = 42.42;
    matriz[2][3] = 0.88;
    matriz[3][0] = 1.315;
    matriz[3][1] = 48.20;
    matriz[3][2] = 1.136;
    
    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);
    
    mostrarSolucion(solucion, {0, 2, 0}, 1);
}

void test2(bool mode) {
    MatrizAdyacencias matriz(4);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[3][3] = 1;
    matriz[0][1] = 1;
    matriz[0][2] = 1;
    matriz[0][3] = 1;
    matriz[1][0] = 1;
    matriz[1][2] = 1;
    matriz[1][3] = 1;
    matriz[2][0] = 1;
    matriz[2][1] = 1;
    matriz[2][3] = 1;
    matriz[3][0] = 1;
    matriz[3][1] = 1;
    matriz[3][2] = 1;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {}, 2);
}

void test3(bool mode) {
    MatrizAdyacencias matriz(3);
    matriz[0][0] = (float) 1;
    matriz[1][1] = (float) 1;
    matriz[2][2] = (float) 1;
    matriz[0][1] = (float) 2/3;
    matriz[0][2] = (float) 1/3;
    matriz[1][0] = (float) 1/3;
    matriz[1][2] = (float) 5/2;
    matriz[2][0] = (float) 3/2;
    matriz[2][1] = (float) 1/6;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {0, 1, 2, 0}, 3);
}

void test4(bool mode) {
    MatrizAdyacencias matriz(4);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[3][3] = 1;
    matriz[0][1] = 0.10;
    matriz[0][2] = 0.10;
    matriz[0][3] = 0.10;
    matriz[1][0] = 0.10;
    matriz[1][2] = 0.10;
    matriz[1][3] = 0.10;
    matriz[2][0] = 0.10;
    matriz[2][1] = 0.10;
    matriz[2][3] = 0.10;
    matriz[3][0] = 0.10;
    matriz[3][1] = 0.10;
    matriz[3][2] = 0.10;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {}, 4);
}

void test5(bool mode) {
    MatrizAdyacencias matriz(3);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[0][1] = 5;
    matriz[0][2] = 1;
    matriz[1][0] = 1;
    matriz[1][2] = 1;
    matriz[2][0] = 1;
    matriz[2][1] = 1;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {0, 1, 0}, 5);
}

void test6(bool mode) {
    MatrizAdyacencias matriz(3);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[0][1] = 5;
    matriz[0][2] = (float) 1/5;
    matriz[1][0] = 1;
    matriz[1][2] = (float) 1/5;
    matriz[2][0] = (float) 1/5;
    matriz[2][1] = (float) 1/5;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {0, 1, 0}, 6);
}

void test7(bool mode) {
    MatrizAdyacencias matriz(7);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[3][3] = 1;
    matriz[4][4] = 1;
    matriz[5][5] = 1;
    matriz[6][6] = 1;

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            if(i != j){
                matriz[i][j] = 0.10;
            }
        }
    }

    matriz[0][5] = (float) 2/3;
    matriz[5][3] = (float) 1/6;
    matriz[3][4] = (float) 2;
    matriz[4][2] = (float) 5/2;
    matriz[2][1] = (float) 3;
    matriz[1][0] = (float) 1;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {0, 5, 3, 4, 2, 1, 0}, 7);
}

void test8(bool mode) {
    MatrizAdyacencias matriz(10);
    matriz[0][0] = 1;
    matriz[1][1] = 1;
    matriz[2][2] = 1;
    matriz[3][3] = 1;
    matriz[4][4] = 1;
    matriz[5][5] = 1;
    matriz[6][6] = 1;
    matriz[7][7] = 1;
    matriz[8][8] = 1;
    matriz[9][9] = 1;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if(i != j){
                matriz[i][j] = 0.010;
            }
        }
    }

    matriz[0][3] = (float) 4/7;
    matriz[3][5] = (float) 3/2;
    matriz[5][1] = (float) 5/3;
    matriz[1][7] = (float) 14/15;
    matriz[7][9] = (float) 2/3;
    matriz[9][0] = (float) 3;

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {0, 3, 5, 1, 7, 9, 0}, 8);
}

void test9(bool mode) {
    MatrizAdyacencias matriz(10);

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if(i != j){
                matriz[i][j] = 1;
            }
        }
    }

    vector<int> solucion;
    if (mode)
        solucion = problemaArbitrajeFW(matriz);
    else
        solucion = problemaArbitrajeBF(matriz);

    mostrarSolucion(solucion, {}, 9);
}


enum {
    BELLMAN_FORD,
    FLOYD_WARSHALL
};

int main(int argc, char* argv[]) {
    char* file = "";
    char* only = "all";
    bool test = false;
    bool no_params = true;
    for (int x = 0; x < argc; ++x) {
        if (strcmp(argv[x],"--file") == 0) {
            file = argv[x+1];
            no_params = false;
        }
        if (strcmp(argv[x],"--only") == 0) {
            only = argv[x+1];
            no_params = false;
        }
        if (strcmp(argv[x],"--test") == 0) {
            test = true;
            no_params = false;
        }
    }

    if (test) {
        bool general_mode = FLOYD_WARSHALL;
        test1(general_mode);
        test2(general_mode);
        test3(general_mode);
        test4(general_mode);
        test5(general_mode);
        test6(general_mode);
        test7(general_mode);
        test8(general_mode);
        test9(general_mode);
    } else if (no_params) {
        execute(NULL, true);
    } else {
        execute(file, false, only);
    }
}