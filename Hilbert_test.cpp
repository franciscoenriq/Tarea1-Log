#include <iostream>
#include <cmath>
#include "Hilbert.cpp"

int main()
{
    std::vector<std::tuple<Punto, int>> puntosConInt = {
        std::make_tuple(Punto(randomNum(), randomNum()), 0),
        std::make_tuple(Punto(randomNum(), randomNum()), 1),
        std::make_tuple(Punto(randomNum(), randomNum()), 2),
        std::make_tuple(Punto(randomNum(), randomNum()), 3)};

    std::cout << "Puntos originales:" << std::endl;
    for (const auto &tupla : puntosConInt)
    {
        const Punto &punto = std::get<0>(tupla);
        int valor = std::get<1>(tupla);
        std::cout << "(" << punto.x << ", " << punto.y << ") - Valor: " << valor << std::endl;
    }

    uint32_t minX = std::numeric_limits<uint32_t>::max();
    uint32_t minY = std::numeric_limits<uint32_t>::max();
    uint32_t maxX = std::numeric_limits<uint32_t>::min();
    uint32_t maxY = std::numeric_limits<uint32_t>::min();

    for (const auto &tupla : puntosConInt)
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

    curvaDeHilbert(puntosConInt, minX, minY, maxX, maxY, nivel);

    std::cout << "Puntos en orden de la curva de Hilbert:" << std::endl;
    for (const auto &tupla : puntosConInt)
    {
        const Punto &punto = std::get<0>(tupla);
        int indice = std::get<1>(tupla);
        std::cout << "(" << punto.x << ", " << punto.y << ") - Index: " << indice << std::endl;
    }

    return 0;
}
