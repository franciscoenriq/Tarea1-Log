#include <iostream>
#include <thread> 
#include <chrono> 
#include <cmath>
#include "../algoritmo_2/Hilbert.cpp"

int main()
{   
    uint32_t n = std::pow(2,10);
    std::vector<std::tuple<Punto, uint32_t>> puntosConInt;
    for (uint32_t i = 0; i < n; ++i) {
        Punto punto(randomNum(), randomNum());
        puntosConInt.push_back(std::make_tuple(punto, i));
    }

    std::cout << "Puntos originales:" << std::endl;
    for (const auto &tupla : puntosConInt)
    {
        const Punto &punto = std::get<0>(tupla);
        uint32_t indice = std::get<1>(tupla);
        std::cout << "(" << punto.x << ", " << punto.y << ") - Indice: " << indice << std::endl;
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


    // Pausa durante 5 segundos
    std::this_thread::sleep_for(std::chrono::seconds(5));

    std::cout << "Puntos en orden de la curva de Hilbert:" << std::endl;
    for (const auto &tupla : puntosConInt)
    {
        const Punto &punto = std::get<0>(tupla);
        uint32_t indice = std::get<1>(tupla);
        std::cout << "(" << punto.x << ", " << punto.y << ") - Index: " << indice << std::endl;
    }

    return 0;
}
