#include "HilbertRTree.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

ull m = 3;



int main()
{
    // Inicializa la semilla aleatoria para obtener números aleatorios diferentes en cada ejecución
    srand(static_cast<unsigned int>(time(nullptr)));

    vector<Rectangulo> vRects;
    for (int i = 0; i < 10; i++)
    {
        Rectangulo rectttt = Rectangulo(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum()));
        vRects.push_back(rectttt);
        cout << rectttt << endl;
    }
    vector<Rectangulo> copia;
    copia.assign(vRects.begin(), vRects.end());

    //cout << "\nOrden Original:" << endl;
    // for (const auto &rect : copia)
    // {
    //     cout << rect.centro << " ";
    // }
    // cout << endl;

    ordenarHilbert(vRects);

    // cout << "Ordenado con Hilbert:" << endl;
    // for (const auto &rect : vRects)
    // {
    //     cout << rect.centro << " ";
    // }
    // cout << endl;

    vector<ull> rTree = vectorRTree(vRects, m, ordenarHilbert);
    cout << "vector RTree_";
    ull pos = 0;
    // for (const auto &val : rTree)
    // {
    //     if (pos == 2 || (pos - 2) & (m * 5) == 0)
    //         cout << '\n'
    //              << "Nodo: ";
    //     if (pos == 1)
    //     {
    //         cout << '\n'
    //              << "puntero: ";
    //     }
    //     cout << val << " ";
    //     pos++;
    // }
    // cout << endl;

    char *fileName = "HilbertRTree.bin";
    grabarVector(rTree, fileName);
    leerBinFile(fileName);


   
    return 0;
}
