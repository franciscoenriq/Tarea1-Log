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
#include "nodoRTree-struct.h"
#include "valarray"
using namespace std;

int l=0;

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
/*Función auxiliar para grabar un vector en memoria */
void grabarVector(vector<int> vect){
    int size = sizeof(vect)/sizeof(vect[0]);
    FILE *arch;
    arch = fopen("xRTree.bin","wt");
    for (int data : vect){
        fwrite(&data,sizeof(int),1,arch);
    }
    fclose(arch);
}

/*
Función que  genera un array que linealmente representa a un  RTree
La estructura final se disponde de la siguiente forma en memoria:

0  rectangulos     R    claves   3k   k
|------------------|----|----|----|----|
|__________________|____ ___________h__|....
                    
---cada rectángulo contiene 4 enteros: 4*R enteros
---cada nodo contiene 4k + k enteros; 
            4k para rectangulos  MBR
            k  para punteros (offset) a hijos.
--- se tienen R/m nodos en total

--- el offset unitario que más sentido tiene usar es sizeof(int) por simplicidad

*/
int offset=sizeof(int);
void nX_RTree(vector <Rectangulo> &r_vect, int m){
    //vector que contendrá los datos a insertar en la interación actual
    vector <int> arr(1,0);
    // se calcula la cantidad de rectángulos totales
    int R_totales = sizeof(r_vect) / sizeof(r_vect[0]);
    //Se ordenan los rectángulos del vector de entrada
    xquickSort(r_vect,0,R_totales - 1);
    //se escribe r_vect en memoria como un nodo hijo (anteponiendo un offset a nulo)
    for (Rectangulo r : r_vect){
        arr.push_back(0);
        arr.push_back(r.p1.x);
        arr.push_back(r.p1.y);
        arr.push_back(r.p2.x);
        arr.push_back(r.p2.y);
    }//con esto, se tienen R rectángulos compuestos por 5 enteros.

    //se calcula la cantidad de mbrs a calcular
    //Notar que esto también corresponde a cuantos nodos se crearán en esta iteración
    int n_mbr = R_totales/m;

    // se calculan los mbrs
    vector <Rectangulo> mbrs(n_mbr,Rectangulo(Punto(0,0),Punto(0,0)));
    // Se recorre el vector de rectángulos para generar los MBR
    for (int rect = 0; rect < n_mbr; rect++){  
        mbrs[rect] = calcularMBR(r_vect,rect*m,rect*(m+1)); 
    }

    // se setablece la posición del primer nodo hijo
    int offset = 1;
    //Se van insertando los nodos padre de a m datos
    for (int padres = 0; padres < n_mbr; padres++){
        int ds = m*padres;              //desplazamiento para los mbrs  
        if (ds > n_mbr) ds = n_mbr;     // si ds > n_mbr pasan cosas feas :s
        for (int k = 0; k<m; k++){      //primero los "punteros" a hijos
            arr.push_back(offset);
            offset +=5;                 //se avanza a la primera posición de la siguiente hoja
        }
        for (int k = 0; k<m; k++){      //luego las claves (rectángulos mbr)
            arr.push_back(mbrs[k+ds].p1.x);
            arr.push_back(mbrs[k+ds].p1.y);
            arr.push_back(mbrs[k+ds].p2.x);
            arr.push_back(mbrs[k+ds].p2.y);
            
        }
    }
    // si la cantidad de mbrs fué mayor a la cantidad de datos guardables en un nodo,
    // se repite una variación del algoritmo anterior.
    if (n_mbr> m){
        int data = n_mbr;
        /* se crean vectores para contener los mbrs anteriores y a los actuales. 
          Notar que el tamañano efectivamente utilizado nunca puede ser mayor a n_mbr original */
        vector <Rectangulo> ans_mbrs = mbrs;
        vector <Rectangulo> new_mbrs(n_mbr/m,Rectangulo(Punto(0,0),Punto(0,0)));


        while (data > n_mbr){
            /*se (re)calcula la cantidad de mbrs a calcular. Notar que esto también corresponde a 
             cuantos nodos se crearán en esta iteración, y hasta que nivel se usarán los vectores
             anteriores */
            int nmbr = data/m;
            // Se recorre el vector de rectángulos para generar los MBR
            for (int rect = 0; rect < nmbr; rect++){  
                new_mbrs[rect] = calcularMBR(ans_mbrs,rect*m,rect*(m+1)); 
            }
            //Se van insertando datos a los  nodos ascendentes de a m datos (hasta llenar los)
            for (int padres = 0; padres < nmbr; padres++){
                for (int k = 0; k<m; k++){      //primero los "punteros" a hijos
                    arr.push_back(offset);
                    offset +=5*m;               //se avanza a la primera posición del siguiente nodo
                }
                int K = m;                      //cantidad de datos a insertar 
                if (nmbr < n_mbr) K = n_mbr;    // si ya se ha logrado que data = nmbr < n_mbrm, K = nmbr
                for (int k = 0; k<K; k++){      //luego las claves (rectángulos mbr)
                    arr.push_back(new_mbrs[k].p1.x);
                    arr.push_back(new_mbrs[k].p1.y);
                    arr.push_back(new_mbrs[k].p2.x);
                    arr.push_back(new_mbrs[k].p2.y);
                    
                } 
            }
            // ahora se new_mbr debe ser ans_mbr
            for (int i = 0; i<nmbr; i++){
                ans_mbrs[i]=new_mbrs[i];
            }
            // Se actualiza el valor de data
            data = data/m;

        }
    }
    // Se escribe el RTree en un binario
    grabarVector(arr);
}
