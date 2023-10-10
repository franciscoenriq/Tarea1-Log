#include <iostream>
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
#include "nodoRTree-struct.h"
#include "valarray"
using namespace std;

int l=0;


/* función auxiliar que genera un vector de rectángulos en base a N puntos */
vector <Rectangulo> makeRectArray(d_int arr[], d_int N){
    int L = N >> 2;
    vector<Rectangulo>vec(L, Rectangulo(Punto(0,0),Punto(0,0)));
    for (int i = 0; i  <L; i++){
        d_int x1 =arr[i*4];
        d_int y1 =arr[i*4+1];
        d_int x2 =arr[i*4+2];
        d_int y2 =arr[i*4+3];
        Rectangulo rect = Rectangulo(Punto(x1,y1),Punto(x2,y2)); 
        vec[i] = rect;
    }
   return vec;
}

 /*algoritmo quicksort para ordenar un rectángulos de menor a mayor respecto a la abscisa*/
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
/*Función auxiliar que encuentra un MBR en el rango [start,end] de una lista de rectangulos*/
Rectangulo calcularMBR(vector<Rectangulo>& listaRectangulos,int start, int end) {
    int lsize = listaRectangulos.size();
    if (listaRectangulos.empty()) {
        return Rectangulo{{0, 0}, {0, 0}};
    }

    if (lsize ==1){
        return listaRectangulos[0];
    }

    uint32_t minX = listaRectangulos[start].p1.x;
    uint32_t maxX = listaRectangulos[start].p2.x;
    uint32_t minY = listaRectangulos[start].p1.y;
    uint32_t maxY = listaRectangulos[start].p2.y;
    if (lsize < end)
        end = lsize;
    for (size_t i = start; i < end; i++) {
        const Rectangulo& rect = listaRectangulos[i];
        minX = min(minX, (uint32_t)rect.p1.x);
        maxX = max(maxX, (uint32_t)rect.p2.x);
        minY = min(minY, (uint32_t)rect.p1.y);
        maxY = max(maxY, (uint32_t)rect.p2.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}
/*
Función que genera una lista de nodos en base a una lista de rectángulos. 
Recibe:
  r_vect:  un vector de rectángulos, m: la cantidad máxima de nodos 
Entrega: 
   un NodoRTree */
vector <NodoRTree> nX_RTree(vector <Rectangulo> &r_vect, int m){
    // cantidad de rectángulos a considerar
    int R_totales = sizeof(r_vect) / sizeof(r_vect[0]);
    //se inicializa un vector con todos los nodos necesarios para cubrir los rectángulos
    vector <NodoRTree> nodos(R_totales/m,NodoRTree(false,m));

    //1. Se ordenan los rectángulos. Esta funcion los ordena con respecto a la abscisa de su centro de menor a mayor
    xquickSort(r_vect,0,R_totales-1);

    //2. Se crean MBR que  cubran un máximo de m rectángulos cada uno
    vector <Rectangulo> mbrs(R_totales/m,Rectangulo(Punto(0,0),Punto(0,0)));
    int counter = 0;
    int index = 0;
    int n_nodo = 0;
    // 3. Se generan los mbr para grupos de m rectángulos y se insertan tanto en el arreglo como en los nodos
    for (int rect = 0; rect < R_totales/m;rect++){  
        mbrs[index] = calcularMBR(r_vect,counter,counter+m);
        //se inserta en el nodo mientras no esté lleno
        if (!nodos[n_nodo].insertarRectangulo(mbrs[index]))
            //si está lleno, se inserta en el siguiente
            nodos[n_nodo++].insertarRectangulo(mbrs[index]);
        index++;
        counter+=m;
    }

return nodos;
}
