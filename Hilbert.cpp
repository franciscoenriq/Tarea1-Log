#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
#include <limits>
#include "NodoRTree.cpp"

// Función auxiliar para comparar tuplas por su componente de tipo Punto
bool compararTuplas(const std::tuple<Punto, int> &a, const std::tuple<Punto, int> &b)
{
    const Punto &puntoA = std::get<0>(a);
    const Punto &puntoB = std::get<0>(b);

    if (puntoA.x != puntoB.x)
        return puntoA.x < puntoB.x;
    return puntoA.y < puntoB.y;
}

// Función principal que ordena una lista de tuplas (Punto,int) segun su posición en la curva de Hilbert
void curvaDeHilbert(std::vector<std::tuple<Punto, int>> &puntos, uint32_t minX, uint32_t minY, uint32_t maxX, uint32_t maxY, uint32_t nivel)
{   
    //std::cout << "Nivel:" << nivel<< std::endl; // (debug)
    if (nivel <= 0)
        return;

    uint32_t midX = (minX + maxX) / 2;
    uint32_t midY = (minY + maxY) / 2;

    // Separamos por cuadrantes
    std::vector<std::tuple<Punto, int>> cuadrante1, cuadrante2, cuadrante3, cuadrante4;

    for (std::tuple<Punto, int> &tupla : puntos)
    {
        const Punto &punto = std::get<0>(tupla);
        // Dividimos la lista según el cuadrante que les corresponde
        if (punto.x <= midX && punto.y <= midY)
            cuadrante1.push_back(tupla);
        else if (punto.x <= midX && punto.y > midY)
            cuadrante2.push_back(tupla);
        else if (punto.x > midX && punto.y <= midY)
            cuadrante3.push_back(tupla);
        else
            cuadrante4.push_back(tupla);
    }

    std::sort(cuadrante1.begin(), cuadrante1.end(), compararTuplas);
    std::sort(cuadrante2.begin(), cuadrante2.end(), compararTuplas);
    std::sort(cuadrante3.begin(), cuadrante3.end(), compararTuplas);
    std::sort(cuadrante4.begin(), cuadrante4.end(), compararTuplas);

    puntos.clear();
    puntos.insert(puntos.end(), cuadrante1.begin(), cuadrante1.end());
    puntos.insert(puntos.end(), cuadrante2.begin(), cuadrante2.end());
    puntos.insert(puntos.end(), cuadrante3.begin(), cuadrante3.end());
    puntos.insert(puntos.end(), cuadrante4.begin(), cuadrante4.end());

    curvaDeHilbert(cuadrante1, minX, minY, midX, midY, nivel - 1);
    curvaDeHilbert(cuadrante2, minX, midY + 1, midX, maxY, nivel - 1);
    curvaDeHilbert(cuadrante3, midX + 1, minY, maxX, midY, nivel - 1);
    curvaDeHilbert(cuadrante4, midX + 1, midY + 1, maxX, maxY, nivel - 1);
}


uint32_t siguientePotenciaDeDos(uint32_t numero) {
    // Si el número es 0, devuelve 1 como la potencia de 2 más cercana
    if (numero == 0) {
        return 1;
    }
    
    // Encuentra el bit más significativo
    uint32_t msb = 0;
    while (numero > 1) {
        numero >>= 1;
        msb++;
    }
    
    // Si el número original es una potencia de 2, devuelve el mismo número
    if ((1u << msb) == numero) {
        return numero;
    }
    
    // Calcula la siguiente potencia de 2 usando desplazamiento a la izquierda
    return 1u << (msb + 1);
}