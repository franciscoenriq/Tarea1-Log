#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include "../auxiliares.cpp"
using namespace std;
#define PAGE_SIZE 4096 // bytes
#define RANGO 500000
#define LADOR 100
#define LADOQ 100000
#define NConsultas 100

// m es los rectangulos que caben en un nodo
// cada rectangulo usa 5 enteros
int M = PAGE_SIZE / (sizeof(int) * 5);

int calc_max_side_length(Rectangulo rect)
{
    return max(rect.sup_der.x - rect.inf_izq.x, rect.sup_der.y - rect.inf_izq.y);
}

vector<Rectangulo> generateLRect(int max_side_length, int max_value, int total)
{
    vector<Rectangulo> result;
    for (int i; i < total; i++)
    {
        Punto inf_izq = Punto(randomNum(0, max_value), randomNum(0, max_value));
        Punto sup_der = Punto(randomNum(0, max_value), randomNum(0, max_value));
        while (inf_izq.x > sup_der.x || inf_izq.y > sup_der.y || calc_max_side_length(Rectangulo(inf_izq, sup_der)) > max_side_length)
        {
            sup_der = Punto(randomNum(0, max_value), randomNum(0, max_value));
        }

        result.push_back(Rectangulo(inf_izq, sup_der));
    }

    return result;
}

int main()
{
    //vector<int> n(16);            // Inicializa un vector con 16 elementos
    vector<int> n(10);    // uno mas chico pa probar
    iota(n.begin(), n.end(), 6); // Llena el vector con valores desde 10 hasta 25

    vector<vector<Rectangulo>> allR;
    // vector<vector<Rectangulo>> allQ;
    vector<Rectangulo> Q = generateLRect(LADOQ, RANGO, 100); // unicas consultas para todo n

for (const auto &rect : Q)
    {
        cout << rect << " ";
    }
    cout << endl;

    // Vamos a generar un rectangulos iniciales y consultas por cada n
    for (int N : n)
    {
        std::cout << N << std::endl;
        vector<Rectangulo> R = generateLRect(LADOR, RANGO, pow(2, N));
        // vector<Rectangulo> Q = generateLRect(LADOQ, RANGO, 100);
        allR.push_back(R);
        // allQ.push_back(Q);
    }

    

    return 0;
}
