#include "../Nearest-X/xquicksort.cpp"
#include "bits/stdc++.h"



//para ahorrar un par de escrituras de teclado
#define p1 inf_izq
#define p2 sup_der
using namespace std;

//vcontador global
int counter = 0;
 /*
Método auxiliar de ordenamiento de ordenamiento para STR basado en un algoritmo quicksort.
Ordenar un vector rectángulos de menor a mayor respecto a la ordenada*/
void yquickSort(vector <Rectangulo> &arr, int start, int end){

    //caso base
    if (start >= end)
        return;

    // se particiona el arreglo
    d_int pivot = arr[start].centro.y;
    d_int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i].centro.y <= pivot)
            count++;
    }
    // asignando una posición al pivote
    int pivotIndex = start + count;


    swap(arr[pivotIndex], arr[start]);


    // ordenando particiones izquierdas y derechas del pivote
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i].centro.y <= pivot) {
            i++;
        }

        while (arr[j].centro.y > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
    // se ordena la mitad izquierda
    yquickSort(arr, start, pivotIndex - 1);
    // Sorting the right part
    yquickSort(arr, pivotIndex + 1, end);
}
//Método de ordenamiento para Sort-Tile-Recursive. Counter 1, se ordena en x. Counter 2, en y
void ordenSTR(vector<Rectangulo> &vect){
    int start = 0;
    int end = vect.size()-1;
    if (counter== 0){
        xquickSort(vect,start,end);
        counter++;
    }
    else{
        cout<<"Y-sorteadno..."<<endl;
        yquickSort(vect,start,end);
        counter--;
    }
}
/*
Función de interfaz que  genera un array que linealmente representa a un  RTree
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
vector<ull> str_RTree(vector <Rectangulo> &r_vect, int m){
    //vector que contendrá los rectángulos en el orden Nearest-X
    vector<ull>arr = vectorRTree(r_vect,m,ordenSTR);
    return arr;
}
