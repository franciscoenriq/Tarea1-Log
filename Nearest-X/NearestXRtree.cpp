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
#include "../NodoRTree.cpp"
#include "valarray"
//para ahorrar un par de escrituras de teclado
#define p1 inf_izq
#define p2 sup_der
#define d_int int
using namespace std;

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

 /*algoritmo quicksort para ordenar un vector rectángulos de menor a mayor respecto a la abscisa*/
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

    d_int minX = listaRectangulos[start].p1.x;
    d_int maxX = listaRectangulos[start].p2.x;
    d_int minY = listaRectangulos[start].p1.y;
    d_int maxY = listaRectangulos[start].p2.y;
    if (lsize < end)
        end = lsize;
    for (size_t i = start+1; i < end; i++) {
        const Rectangulo& rect = listaRectangulos[i];
        minX = min(minX, (d_int)rect.p1.x);
        maxX = max(maxX, (d_int)rect.p2.x);
        minY = min(minY, (d_int)rect.p1.y);
        maxY = max(maxY, (d_int)rect.p2.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}
/*Función auxiliar para grabar un vector en memoria */
void grabarVector(vector<int> vect){
    int size = sizeof(vect)/sizeof(vect[0]);
    FILE *arch;
    arch = fopen("xRTree","wt");
    for (int data : vect){
        fwrite(&data,sizeof(int),1,arch);
    }
    fclose(arch);
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
    //vector que contendrá los datos a insertar en la interación actual
    vector <int> arr(1,0);
    // se calcula la cantidad de rectángulos totales
    int R_totales = r_vect.size();
    cout<<"Size of vector: "<< R_totales <<endl;
    //Se ordenan los rectángulos del vector de entrada.Notar que esta función puede ser reemplazada por otras 
    xquickSort(r_vect,0,R_totales - 1);

    /* ESTO NO ES NECESARIO SI LOS RECTANGULOS HIJOS VAN EN  LOS PADRES
    //se escribe r_vect en memoria como un nodo hijo (anteponiendo un offset a nulo)
    for (Rectangulo r : r_vect){
        arr.push_back(0);
        arr.push_back(r.p1.x);
        arr.push_back(r.p1.y);
        arr.push_back(r.p2.x);
        arr.push_back(r.p2.y);
    }//con esto, se tienen R rectángulos compuestos por 5 enteros.
    */
    // PRIMERA ITERACIÓN
    //se calcula la cantidad de mbrs a calcular
    //Notar que esto también corresponde a cuantos nodos se crearán en esta iteración
    int n_mbr = R_totales/m;
    if (R_totales%m != 0) n_mbr++;     //si existe algun resto, se debe calcular un mbr más
    cout<<"Starting..."<<"MBRs to calculate: "<< n_mbr<<endl;

    // se calculan los mbrs
    vector <Rectangulo> mbrs(n_mbr,Rectangulo(Punto(0,0),Punto(0,0)));
    // Se recorre el vector de rectángulos para generar los MBR
    for (int rect = 0; rect < n_mbr; rect++){  
        mbrs[rect] = calcularMBR(r_vect,rect*m,(rect+1)*m); 
    }
    //DEBUG
    for (Rectangulo x :mbrs){
        cout<<"MBR: "<<x.p1.x<<','<<x.p1.y<<';'<<x.p2.x<<','<<x.p2.y<<endl;
    }
    //contador de inserciones
    int data = 0;
    // algoritmo de construcción
    int offset = 1 + m;   // se establece la posición del primer nodo hijo
    //Se van insertando los nodos padre de a m datos
    for (int padres = 0; padres < n_mbr; padres++){
        int ds = m*padres;              //desplazamiento de los padres por los mbrs  
        for (int k = 0; k<m; k++){      //primero los "punteros" a hijos
            if (k >= R_totales) arr.push_back(0);//Hay menos de k hijos reales para el nodo
            else arr.push_back(offset);
            offset +=5;                 //se avanza a la primera posición de la siguiente hoja
        }
        for (int k = 0; k<m; k++){      //luego las claves (rectángulos de hijos)
            if (k+ds >= R_totales){       //Hay menos de k mbr reales para el nodo
                arr.push_back(0);
                arr.push_back(0);
                arr.push_back(0);
                arr.push_back(0);
            } 
            else {
                arr.push_back(r_vect[k+ds].p1.x);
                arr.push_back(r_vect[k+ds].p1.y);
                arr.push_back(r_vect[k+ds].p2.x);
                arr.push_back(r_vect[k+ds].p2.y);
                data++;
            }
        }
    }
    offset = 1; //se retrocede el offset al primer nodo hijo
    // si la cantidad de mbrs fué mayor a la cantidad de datos guardables en un nodo,
    // se repite una variación del algoritmo anterior.
 
        int last = 0; //marcador que indica si es la última iteración
        int rects = n_mbr;
        while (true){
            int ds = 0;
            for (int padres = 0; padres < rects; padres++){
                //inserción de punteros a hijos
                for (int k = 0; k<m; k++){          
                    if (k +ds>=n_mbr) {
                        arr.push_back(0);        //Hay menos de k hijos reales para el nodo
                        
                    }
                    else {
                        arr.push_back(offset);
                        offset +=5*m;           //se avanza a la primera posición de la siguiente hoja
                    }                 
                }
                
                //linserción de claves (rectángulos de hijos)
                for (int k = 0; k < m; k++){            
                    if (k  +ds>= n_mbr){           //Hay menos de k mbr reales para el nodo
                        arr.push_back(0);
                        arr.push_back(0);
                        arr.push_back(0);
                        arr.push_back(0);
                        rects--;
                    } 
                    else { 
                        arr.push_back(mbrs[k+ds].p1.x);
                        arr.push_back(mbrs[k+ds].p1.y);
                        arr.push_back(mbrs[k+ds].p2.x);
                        arr.push_back(mbrs[k+ds].p2.y);
                        data++; 

                    }
                }ds +=m;
            
                
            }
            ds = 0;
            if (last)       //si la siguiente es la última iteración, se rompe el bucle
                break;
            //se re-calculan los MBR
            int br = n_mbr;
            n_mbr = br/m;
            rects = n_mbr;
            // Se guarda cuantos elemntos será necesario quitar del vector;
            if (br != 1 && br%m != 0) n_mbr++;
            // Se recorre el vector de mbrs para generar otros MBR
            if (n_mbr>0){
                cout<<"MBRs to calculate: "<< n_mbr<<endl;
                vector <Rectangulo>  newmbr(n_mbr,Rectangulo(Punto(0,0),Punto(0,0)));
                for (int rect = 0; rect < n_mbr; rect++){  
                    newmbr[rect] = calcularMBR(mbrs,rect*m,(rect+1)*m); 
                }
                // se quitan los espacios innecesarios de mbrs
                mbrs = vector <Rectangulo> (n_mbr,Rectangulo(Punto(0,0),Punto(0,0)));
                for (int i = 0; i<n_mbr; i++){
                    mbrs[i]= newmbr[i];
                }
                //DEBUG
                for (Rectangulo x :mbrs){
                    cout<<"MBR*: "<<x.p1.x<<','<<x.p1.y<<';'<<x.p2.x<<','<<x.p2.y<<endl;
                }
            }
            if (n_mbr < m) 
                last = 1;   //se marca como ultima la siguiente iteración    
        }
    
    // Se escribe el RTree en un binario
    grabarVector(arr);
}
