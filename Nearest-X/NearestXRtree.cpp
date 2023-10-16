#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <set>
#include <string>
#include <cmath>
#include <malloc.h>
#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#include "bits/stdc++.h"
#include "valarray"
#include "../auxiliares.cpp"
using namespace std;

/* función auxiliar que genera un vector de rectángulos en base a N puntos */
vector <Rectangulo> makeRectArray(int arr[], int N){
    int L = N >> 2;
    vector<Rectangulo>vec(L, Rectangulo(Punto(0,0),Punto(0,0)));
    for (int i = 0; i  <L; i++){
        int x1 =arr[i*4];
        int y1 =arr[i*4+1];
        int x2 =arr[i*4+2];
        int y2 =arr[i*4+3];
        Rectangulo rect = Rectangulo(Punto(x1,y1),Punto(x2,y2)); 
        vec[i] = rect;
    }
   return vec;
}

 /*algoritmo quicksort para ordenar un vector rectángulos de menor a mayor respecto a la abscisa*/
void xquickSort(vector <Rectangulo> &arr, int start, int end){ 
 
    //caso base
    if (start >= end)
        return;

    // se particiona el arreglo
    int pivot = arr[start].centro.x;
    int count = 0;
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
void nX_RTree(vector <Rectangulo> &r_vect, int m){
    // se calcula la cantidad de rectángulos totales
    int R_totales = r_vect.size();
    //Se ordenan los rectángulos del vector de entrada.Notar que esta función puede ser reemplazada por otras 
    xquickSort(r_vect,0,R_totales - 1);
    // se obtiene un vector que representa al rTree
    vector<int> arr = vectorRTree(r_vect, m);
    // Se escribe el RTree en un binario
    grabarVector(arr);
}
