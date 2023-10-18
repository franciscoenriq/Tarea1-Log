#include "bits/stdc++.h"
#include "xquicksort.cpp"


//para ahorrar un par de escrituras de teclado
#define p1 inf_izq
#define p2 sup_der
using namespace std;


//función auxiliar de ordenamiento según nearestX
void ordenNearestX(vector<Rectangulo> &vect){
    int start = 0;
    int end = vect.size()-1;
    xquickSort(vect,start,end);
}
/*
Función que  genera un array que linealmente representa a un  RTree
La estructura final dispone de los datos en nodos de la siguiente forma en memoria

0 1    k+1  claves (rects)    5k+1 otro nodo...
|-|-----|-----|-----|-----|-----|
|_|hijos|_______________________|.......

---cada rectángulo contiene 4 enteros + 0: 5*R enteros
---cada nodo contiene 4k + k enteros;
            4k para rectangulos  MBR
            k  para punteros (offset) a hijos.
--- se tienen R/m nodos en total
*/
vector<ull> nX_RTree(vector <Rectangulo> &r_vect, int m){
    //vector que contendrá los rectángulos en el orden Nearest-X
    vector<ull>arr = vectorRTree(r_vect,m,ordenNearestX);
    return arr;
}
