#include "HilbertRTree.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int tamanho_pagina = 4096; // bytes

int main()
{
    // Inicializa la semilla aleatoria para obtener números aleatorios diferentes en cada ejecución
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Rectangulo> rectangulos1;
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));
    rectangulos1.push_back(Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum())));

    vector<Rectangulo> copia;
    copia.assign(rectangulos1.begin(), rectangulos1.end());

    cout << "Orden Original:" << endl;
    for (const auto &rect : copia)
    {
        cout << rect.centro << " ";
    }
    cout << endl;

    ordenarHilbert(rectangulos1);

    cout << "Ordenado con Hilbert:" << endl;
    for (const auto &rect : rectangulos1)
    {
        cout << rect.centro << " ";
    }
    cout << endl;

    vector<int> rTree = vectorRTree(rectangulos1, 2);
    cout << "vector RTree:" << endl;
    for (const auto &val : rTree)
    {
        cout << val << " ";
    }
    cout << endl;

    const char* fileName = "HilbertRTree.bin";
    grabarVector(rTree, fileName);

    return 0;
}
