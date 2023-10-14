#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include <iostream>
#include <vector>
#include <tuple>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include "../../NodoRTree.cpp"

// ===================== version 2 ================================

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

bool compararPorD(std::tuple<Punto, int> a, std::tuple<Punto, int> b, int n)
{
    const Punto &puntoA = std::get<0>(a);
    const Punto &puntoB = std::get<0>(b);
    int valorA = xy2d(n, puntoA.x, puntoA.y);
    int valorB = xy2d(n, puntoB.x, puntoB.y);

    // // Imprimir los puntos
    // std::cout << "Punto A: " << puntoA << std::endl;
    // std::cout << "Punto B: " << puntoB << std::endl;

    // // Imprimir el valor de valorA y valorB
    // std::cout << "Valor de valorA: " << valorA << std::endl;
    // std::cout << "Valor de valorB: " << valorB << std::endl;

    return valorA < valorB;
}

void ordenarHilbert(std::vector<std::tuple<Punto, int>> &puntos)
{

    // calcular n necesario
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();

    for (auto &tupla : puntos)
    {
        Punto &punto = std::get<0>(tupla);
        minX = std::min(minX, punto.x);
        minY = std::min(minY, punto.y);
        maxX = std::max(maxX, punto.x);
        maxY = std::max(maxY, punto.y);
    }
    int ancho_espacio = maxX - minX;
    int alto_espacio = maxY - minY;
    int nivel = log2(std::max(ancho_espacio, alto_espacio));
    int n = pow(2, nivel);

    std::sort(puntos.begin(), puntos.end(), [n](std::tuple<Punto, int> a, std::tuple<Punto, int> b)
              { return compararPorD(a, b, n); }); // ordenamos ds según d
}