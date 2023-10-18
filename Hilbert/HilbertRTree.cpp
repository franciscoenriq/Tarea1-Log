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
typedef unsigned long long ull;


// Código desde wikipedia para pasar de un punto x,y a d

// Declaración anticipada de la función rot
void rot(ull n, ull &x, ull &y, ull rx, ull ry);

// Convierte (x, y) a d
ull xy2d(ull n, ull x, ull y)
{
    ull rx, ry, s, d = 0;
    for (s = n / 2; s > 0; s /= 2)
    {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, x, y, rx, ry);
    }

    //cout << d << " " << endl;
    return d;
}

// Convierte d a (x, y)
void d2xy(ull n, ull d, ull &x, ull &y)
{
    ull rx, ry, s, t = d;
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
void rot(ull n, ull &x, ull &y, ull rx, ull ry)
{
    ull t;
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

bool compararPorD(Rectangulo a, Rectangulo b, ull n)
{
    const Punto centroA = a.centro;
    const Punto centroB = b.centro;
    ull valorA = xy2d(n, centroA.x, centroA.y);
    ull valorB = xy2d(n, centroB.x, centroB.y);


    return valorA < valorB;
}

void ordenarHilbert(vector<Rectangulo> &lRect)
{

    // calcular n necesario
    ull minX = numeric_limits<ull>::max();
    ull minY = numeric_limits<ull>::max();
    ull maxX = numeric_limits<ull>::min();
    ull maxY = numeric_limits<ull>::min();

    for (const auto &rect : lRect)
    {
        minX = min(minX, rect.centro.x);
        minY = min(minY, rect.centro.y);
        maxX = max(maxX, rect.centro.x);
        maxY = max(maxY, rect.centro.y);
    };

    ull ancho_espacio = maxX - minX;
    ull alto_espacio = maxY - minY;
    ull nivel = log2(max(ancho_espacio, alto_espacio));
    ull n = pow(2, nivel);
    //cout << n << " " << endl;



    sort(lRect.begin(), lRect.end(), [n](Rectangulo a, Rectangulo b)
         { return compararPorD(a, b, n); }); // ordenamos ds según d
}

void HilbertRTree(vector<Rectangulo> &lRect, ull m)
{
    //pasar la lista a un vector que representa al RTree junto con el algoritmo de ordenamiento
    vector<ull> vArbol = vectorRTree(lRect, m,ordenarHilbert);

    // finalmente, escribir el vector en un archivo binario
    char* fileName = "HilbertRTree.bin";
    grabarVector(vArbol, fileName);
}
