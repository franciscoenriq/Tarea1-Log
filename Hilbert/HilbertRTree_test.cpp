#include "HilbertRTree.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

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

    vector<ull> rTree = vectorRTree(rectangulos1, 3);
    cout << "vector RTree:" << endl;
    for (const auto &val : rTree)
    {
        cout << val << " ";
    }
    cout << endl;

    char *fileName = "HilbertRTree.bin";
    grabarVector(rTree, fileName);
    char *file2 = "HilbertRTree.bin";
    leerBinFile(file2);

    return 0;
}
