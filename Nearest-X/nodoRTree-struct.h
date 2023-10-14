#include <utility>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>
#define PGSIZE 4096 /* El tamaño de página. Depende de la máquina */
#define d_int int
using namespace std;

/* Estructura para representar un punto en un plano 2D 
    d_int x: abscisa
    d_int y: ordenada */
struct Punto{
    d_int x;
    d_int y;

    Punto(d_int x, d_int y) : x(x), y(y) {}
};
// Sobrecarga del operador de salida para imprimir un objeto Punto ======================
ostream& operator<<(ostream& os, const Punto& punto){
    os << "(" << punto.x << ", " << punto.y << ")";
    return os;
};


/* Función para calcular el centro de un Rectángulo */
Punto calcularPuntoMedio(const Punto& inferiorIzquierdo, const Punto& superiorDerecho){
    
    double medioX = (inferiorIzquierdo.x + superiorDerecho.x) / 2.0;
    double medioY = (inferiorIzquierdo.y + superiorDerecho.y) / 2.0;

    return Punto(medioX, medioY);
};
// ESTRUCTURA=====================================================
/* Estructura para representar un rectángulo. Contiene un campo extra  para facilitar su manipulación en la construcción de árboles*/
struct Rectangulo{
    Punto p1;
    Punto p2;
    Punto centro;
    Rectangulo(Punto p1, Punto p2) : p1(p1), p2(p2),  centro(calcularPuntoMedio(p1,p2)) {}
};
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