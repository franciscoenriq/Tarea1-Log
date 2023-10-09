#include <iostream>
#include <vector>
#include <tuple>
#include <iostream>
#include <thread>
#include <chrono>
#include <cmath>
#include "Hilbert.cpp"
#include "../NodoRTree.cpp"

NodoRTree HilbertRTree(std::vector<Rectangulo> &listaRectangulos, int m)
{

    // 1. extraer centros,indice
    std::vector<std::tuple<Punto, int>> centros;

    int cantidadRectangulos = sizeof(listaRectangulos) / sizeof(listaRectangulos[0]);

    // iteramos sobre los rectangulos
    for (int i = 0; i < cantidadRectangulos; i++)
    {
        // Acceder al elemento en la posición i
        Rectangulo rect = listaRectangulos[i];
        const std::tuple<Punto, int> tupla(rect.centro, i);
        centros.push_back(tupla);
    };

    // 2. ordenar los centros segun la curva de hilbert
    // calculamos el límite del espacio

    ordenarHilbert(centros);

    // 3. insertar en nodos de tamaño M y crear el árbol
    int contador = 0;

    // iteramos sobre los rectangulos

    for (int i = 0; i < cantidadRectangulos; i++)
    {
        // Acceder al elemento en la posición i
        Rectangulo rect = listaRectangulos[i];
        const std::tuple<Punto, int> tupla(rect.centro, i);
        centros.push_back(tupla);
    };
}
