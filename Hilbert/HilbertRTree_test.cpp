#include "HilbertRTree.cpp"

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int m = 2;

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

    vector<int> rTree = vectorRTree(copia, m, ordenarHilbert);
    cout << "vector RTree:" << endl;
    cout << "Nodo:"<<endl;
    int pos = 0;
    for (const auto &val : rTree)
    {   if (pos == 2 || (pos-2)&(m*5)==0){
            cout<<'\n'<<"Nodo: ";}
        if (pos==1){
            cout<<'\n'<<"puntero: ";
        }
        cout << val << " ";
        pos++;
    }
    cout << endl;

    const char* fileName = "HilbertRTree.bin";
    grabarVector(rTree, fileName);

    return 0;
}
