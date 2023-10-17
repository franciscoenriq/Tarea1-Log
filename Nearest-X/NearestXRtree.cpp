#include "../auxiliares.cpp"
#include "bits/stdc++.h"


//para ahorrar un par de escrituras de teclado
#define p1 inf_izq
#define p2 sup_der
using namespace std;


 /*
Método de ordenamiento para Nearest-X basado en un algoritmo quicksort.
Ordenar un vector rectángulos de menor a mayor respecto a la abscisa*/
void xquickSort(vector <Rectangulo> &arr, int start, int end){ 
 
    //caso base
    if (start >= end)
        return;

    // se particiona el arreglo
    d_int pivot = arr[start].centro.x;
    d_int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i].centro.x <= pivot)
            count++;
    }
    // asignando una posición al pivote
    int pivotIndex = start + count;


    swap(arr[pivotIndex], arr[start]);

 
    // ordenando particiones izquierdas y derechas del pivote
    int i = start, j = end;
 
    while (i < pivotIndex && j > pivotIndex) {
 
        while (arr[i].centro.x <= pivot) {
            i++;
        }
 
        while (arr[j].centro.x > pivot) {
            j--;
        }
 
        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
    // se ordena la mitad izquierda
    xquickSort(arr, start, pivotIndex - 1);
    // Sorting the right part
    xquickSort(arr, pivotIndex + 1, end);
}
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
vector<int> nX_RTree(vector <Rectangulo> &r_vect, int m){
    //vector que contendrá los rectángulos en el orden Nearest-X
    vector<int>arr = vectorRTree(r_vect,m,ordenNearestX);
    return arr;
}
