#include <iostream>
#include <vector>
#include <tuple>
#include <thread>
#include <chrono>
#include <cmath>
#include <algorithm>
#include <limits>
#include "../auxiliares.cpp"

using namespace std;

// Código desde wikipedia para pasar de un punto x,y a d

// Declaración anticipada de la función rot
void rot(int n, int &x, int &y, int rx, int ry);

// Convierte (x, y) a d
int xy2d(int n, int x, int y)
{
    int rx, ry, s, d = 0;
    for (s = n / 2; s > 0; s /= 2)
    {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, x, y, rx, ry);
    }
    return d;
}

// Convierte d a (x, y)
void d2xy(int n, int d, int &x, int &y)
{
    int rx, ry, s, t = d;
    x = y = 0;
    for (s = 1; s < n; s *= 2)
    {
        rx = 1 & (t / 2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        x += s * rx;
        y += s * ry;
        t /= 4;
    }
}

// Rotar/voltear un cuadrante apropiadamente
void rot(int n, int &x, int &y, int rx, int ry)
{
    int t;
    if (ry == 0)
    {
        if (rx == 1)
        {
            x = n - 1 - x;
            y = n - 1 - y;
        }
        t = x;
        x = y;
        y = t;
    }
}

bool compararPorD(Rectangulo a, Rectangulo b, int n)
{
    const Punto centroA = a.centro;
    const Punto centroB = b.centro;
    int valorA = xy2d(n, centroA.x, centroA.y);
    int valorB = xy2d(n, centroB.x, centroB.y);

    // Imprimir los puntos
    cout << "Punto A: " << centroA << endl;
    cout << "Punto B: " << centroB << endl;

    // Imprimir el valor de valorA y valorB
    cout << "Valor de valorA: " << valorA << endl;
    cout << "Valor de valorB: " << valorB << endl;

    return valorA < valorB;
}

void ordenarHilbert(vector<Rectangulo> &lRect)
{

    // calcular n necesario
    int minX = numeric_limits<int>::max();
    int minY = numeric_limits<int>::max();
    int maxX = numeric_limits<int>::min();
    int maxY = numeric_limits<int>::min();

    for (const auto &rect : lRect)
    {
        minX = min(minX, rect.centro.x);
        minY = min(minY, rect.centro.y);
        maxX = max(maxX, rect.centro.x);
        maxY = max(maxY, rect.centro.y);
    };

    int ancho_espacio = maxX - minX;
    int alto_espacio = maxY - minY;
    int nivel = log2(max(ancho_espacio, alto_espacio));
    int n = pow(2, nivel);

    sort(lRect.begin(), lRect.end(), [n](Rectangulo a, Rectangulo b)
         { return compararPorD(a, b, n); }); // ordenamos ds según d
}

void HilbertRTree(vector<Rectangulo> &lRect, int m)
{
    //pasar la lista a un vector que representa al RTree junto con el algoritmo de ordenamiento
    vector<int> vArbol = vectorRTree(lRect, m,ordenarHilbert);

    // finalmente, escribir el vector en un archivo binario
      const char* fileName = "HilbertRTree.bin";
    grabarVector(vArbol, fileName);
}
