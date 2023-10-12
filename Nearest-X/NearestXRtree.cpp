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
/* Función que construye un grupo de nodos hoja en base a una lista de rectángulos*/
vector <NodoRTree> makeLeafs(vector<Rectangulo> &rects){
    //tamaño del vector
    int n = sizeof(rects)/sizeof(rects[0]);
    //se crea el vector de nodos
    vector<NodoRTree> nodos(n,NodoRTree(true,1));
    int i = 0;
    for(Rectangulo r: rects){
        nodos[i].insertarRectangulo(r);
        i++;
    }
    return nodos;
} 
/*
Función que  genera un único nodo raíz en base a una lista de rectángulos. 
Recibe:
  r_vect:  un vector de rectángulos, m: la cantidad máxima de nodos 
Entrega: 
   un  NodoRTree 
*/
NodoRTree nX_RTree(vector <Rectangulo> &r_vect, int m){
    // creación del nodo a retornar
    NodoRTree Rtree = NodoRTree(false,m);
    // se calcula la cantidad total  de rectángulos  a calcular
    int R_totales = sizeof(r_vect) / sizeof(r_vect[0]);
    //Se ordenan los rectángulos del vector de entrada
    xquickSort(r_vect,0,R_totales - 1);
    int i = 0;

    // Si la cantidad de rectángulos total cabe en el nodo, se añaden los rectángulos como nodos hoja
    if (R_totales<=m){
        vector <NodoRTree> hijos = makeLeafs(r_vect);
        for (NodoRTree h : hijos){
            Rtree.insertarHijo(h);
            Rtree.insertarRectangulo(r_vect[i]);
            i++;
        }
    }
    else{
        /* se calcula la cantidad de iteraciones que serán necesarias para a los rectangulos.
           O iteraciones = lo del if:  
                Se tiene un único nodo no hoja que contiene a lo  más m rectángulos y m punteros a 
                nodos hoja que  contienen cada uno uno de los rectángulos anteriores.
             
           1 iteración = bastan nr<=m MBRs para cubrir todos los rectángulos:
              => el nodo Rtree contiene nr MBRs y nr punteros a los nodos, los cuales cumplen lo 
              anterior
            ... etc ...
        */
        int rt = R_totales;
        while(rt > m){
            rt = rt/m;
            i++;
        }
        // se crea un vector de punteros de arreglos que contendrán los resultados de cada iteración
        vector <NodoRTree**> ptrs[3];
        //se calcula el tamaño de la partición de  rectángulos
        int part = (R_totales/m);
        for (rt; rt>0; rt--){
            vector<NodoRTree**> 
            for(){

            }
                ptrs->insert(part,NodoRTree(false,m));
            // Se genera un vector de  MBR  rectángulos para el grupo de m rectángulos, 
            vector <Rectangulo> mbrs(R_totales/m,Rectangulo(Punto(0,0),Punto(0,0)));
            int counter = 0;
            for (int rect = 0; rect < R_totales/m; rect++){  
                mbrs[rect] = calcularMBR(r_vect,counter,counter+m);
                }
                counter+=m;
                nodos->
            }
            
        }
    }

return Rtree;
}
