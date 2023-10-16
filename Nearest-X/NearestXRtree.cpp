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
#include "../auxiliares.cpp"
#include "../NodoRTree.cpp"
using namespace std;

int l = 0;

int offset = sizeof(int);
void nX_RTree(vector<Rectangulo> &r_vect, int m)
{

    // se calcula la cantidad de rectángulos totales
    int R_totales = sizeof(r_vect) / sizeof(r_vect[0]);
    // Se ordenan los rectángulos del vector de entrada
    xquickSort(r_vect, 0, R_totales - 1);

    vector<int> arbol = vectorRTree(r_vect, m);
}
