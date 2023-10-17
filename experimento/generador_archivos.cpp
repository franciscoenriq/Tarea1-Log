#include <iostream>
#include <vector>
#include <numeric>
#include <cmath>
#include "../auxiliares.cpp"

#include <sstream>
using namespace std;
#define PAGE_SIZE 4096 // bytes
#define RANGO 500000
#define LADOR 100
#define LADOQ 100000
#define NConsultas 100

// variables para generar los archivos de pruebas
bool generarQ = 0;
bool generarR = 1;

// m es los rectangulos que caben en un nodo
// cada rectangulo usa 5 enteros
ull M = PAGE_SIZE / (sizeof(ull) * 5);

ull calc_max_side_length(Rectangulo rect)
{
    return max(rect.sup_der.x - rect.inf_izq.x, rect.sup_der.y - rect.inf_izq.y);
}

vector<ull> generateLRect(ull max_side_length, ull max_value, ull total)
{
    vector<ull> result;
    for (ull i = 0; i < total; i++)
    {
        std::cout << i << std::endl;
        ull inf_izq_x = randomNum(0, max_value);
        ull inf_izq_y = randomNum(0, max_value);
        ull sup_der_x = randomNum(inf_izq_x, inf_izq_x + max_side_length);
        ull sup_der_y = randomNum(inf_izq_y, inf_izq_y + max_side_length);

        result.push_back(inf_izq_x);
        result.push_back(inf_izq_y);
        result.push_back(sup_der_x);
        result.push_back(sup_der_y);

        cout << Rectangulo(Punto(inf_izq_x, inf_izq_y), Punto(sup_der_x, sup_der_y)) << " " << endl;
    }

    return result;
}

int main()
{
    vector<ull> n = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25}; // Inicializa un vector con 16 elementos
    //vector<ull> n = {25};
    // Vamos a generar un rectangulos iniciales y consultas por cada n
    // generamos los Q y R por separado
    if (generarQ)
    {
        for (ull N : n)
        {
            std::cout << N << std::endl;
            // preparamos nombre de archivo a escribir
            ull numero = N;
            char *archivoQbase = "Q_for_n_2_";

            std::stringstream ss;
            ss << archivoQbase << numero;

            char *archivoQ = const_cast<char *>(ss.str().c_str());

            // calculamos los vectores de rectangulos
            vector<ull> Q = generateLRect(LADOQ, RANGO, 100);

            // grabamos los vectores en el archivo
            grabarVector(Q, archivoQ);
        }
    }

    if (generarR)
    {
        for (ull N : n)
        {
            std::cout << N << std::endl;

            // preparamos nombre de archivo a escribir
            ull numero = N;
            char *archivoRbase = "R_for_n_2_";

            std::stringstream ss;
            ss << archivoRbase << numero;

            char *archivoR = const_cast<char *>(ss.str().c_str());

            // calculamos los vectores de rectangulos
            vector<ull> R;
            if (N == 25)
            {
                R = generateLRect(LADOR, RANGO, ull(pow(2, 23)));
                for (int i = 1; i < 4; i++)
                {
                    vector<ull> R2 = generateLRect(LADOR, RANGO, ull(pow(2, 23)));
                    R.insert(R.end(), R2.begin(), R2.end());
                }
            }
            else
            {
                R = generateLRect(LADOR, RANGO, ull(pow(2, N)));
            }

            // grabamos los vectores en el archivo
            grabarVector(R, archivoR);
        }
    }

    return 0;
}
