#include <iostream>
#include <vector>
#include <tuple> 
#include <iostream>
#include <thread> 
#include <chrono> 
#include <cmath>
#include "Hilbert.cpp"
#include "NodoRTree.cpp"

NodoRTree HilbertRTree(std::vector<Rectangulo>& listaRectangulos, int m){

    // 1. extraer centros,indice
    std::vector<std::tuple<Punto, uint32_t>> centros;

    int cantidadRectangulos = sizeof(listaRectangulos) / sizeof(listaRectangulos[0]);

    // iteramos sobre los rectangulos
    for (int i = 0; i < cantidadRectangulos; i++) {
        // Acceder al elemento en la posición i
        Rectangulo rect = listaRectangulos[i];
        std::tuple<Punto, uint32_t> tupla(rect.centro, i);
        centros.push_back(tupla);
    };

    // 2. ordenar los centros segun la curva de hilbert
    uint32_t minX = std::numeric_limits<uint32_t>::max();
    uint32_t minY = std::numeric_limits<uint32_t>::max();
    uint32_t maxX = std::numeric_limits<uint32_t>::min();
    uint32_t maxY = std::numeric_limits<uint32_t>::min();

    for (const auto &tupla : centros)
    {
        const Punto &punto = std::get<0>(tupla);
        minX = std::min(minX, punto.x);
        minY = std::min(minY, punto.y);
        maxX = std::max(maxX, punto.x);
        maxY = std::max(maxY, punto.y);
    }
    uint32_t ancho_espacio = maxX-minX;
    uint32_t alto_espacio = maxY-minY;
    uint32_t lado_cuadrado=std::max(ancho_espacio, alto_espacio);
    uint32_t lado_potencia_2 = siguientePotenciaDeDos(lado_cuadrado);

    uint32_t segmento = 128; // TODO: dejaremos segmentos de 128 pero debe ser el mínimo que cubra el plano (pendiente)
    uint32_t nivel = log2(lado_potencia_2/segmento);

    curvaDeHilbert(centros, minX, minY, maxX, maxY, nivel);

    // 3. insertar en nodos de tamaño M y crear el árbol
    

}

