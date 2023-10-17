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
int M = PAGE_SIZE / (sizeof(int) * 5);

int calc_max_side_length(Rectangulo rect)
{
    return max(rect.sup_der.x - rect.inf_izq.x, rect.sup_der.y - rect.inf_izq.y);
}

vector<Rectangulo> generateLRect(int max_side_length, int max_value, int total)
{
    vector<Rectangulo> result;
    for (int i = 0; i < total; i++)
    {
        Punto inf_izq = Punto(randomNum(0, max_value), randomNum(0, max_value));
        Punto sup_der = Punto(randomNum(inf_izq.x, max_value), randomNum(inf_izq.y, max_value));
        while (inf_izq.x > sup_der.x || inf_izq.y > sup_der.y || calc_max_side_length(Rectangulo(inf_izq, sup_der)) > max_side_length)
        {
            sup_der = Punto(randomNum(0, max_value), randomNum(0, max_value));
        }

        result.push_back(Rectangulo(inf_izq, sup_der));
        cout << Rectangulo(inf_izq, sup_der) << " " << endl;
    }


    return result;
}

vector<int> vRectAvInt(vector<Rectangulo> lista, int cantidadRectangulos, int n)
{

    vector<int> resultado;
    resultado.push_back(n);
    resultado.push_back(cantidadRectangulos);
    for (Rectangulo rect : lista)
    {
        resultado.push_back(rect.inf_izq.x);
        resultado.push_back(rect.inf_izq.y);
        resultado.push_back(rect.sup_der.x);
        resultado.push_back(rect.sup_der.y);
    }
    return resultado;
}

int main()
{
    vector<int> n(16); // Inicializa un vector con 16 elementos
    //vector<int> n(10);           // uno mas chico pa probar
    iota(n.begin(), n.end(), 10); // Llena el vector con valores desde 10 hasta 25


    // Vamos a generar un rectangulos iniciales y consultas por cada n
    // generamos los Q y R por separado
    if (generarQ)
    {
        for (int N : n)
        {
            std::cout << N << std::endl;
            // preparamos nombre de archivo a escribir
            int numero = N;
            char *archivoQbase = "Q_for_n_2_";

            std::stringstream ss;
            ss << archivoQbase << numero;

            char *archivoQ = const_cast<char *>(ss.str().c_str());

            // calculamos los vectores de rectangulos
            vector<Rectangulo> Q = generateLRect(LADOQ, RANGO, 100);

            // pasamos los vectores de rectangulos a vectores de enteros
            vector<int> Qint = vRectAvInt(Q, 100, N);

            // grabamos los vectores en el archivo
            grabarVector(Qint, archivoQ);
        }
    }

    if (generarR)
    {
        for (int N : n)
        {
            std::cout << N << std::endl;

            // preparamos nombre de archivo a escribir
            int numero = N;
            char *archivoRbase = "R_for_n_2_";

            std::stringstream ss;
            ss << archivoRbase << numero;

            char *archivoR = const_cast<char *>(ss.str().c_str());

            // calculamos los vectores de rectangulos
            vector<Rectangulo> R = generateLRect(LADOR, RANGO, int(pow(2, N)));

            // pasamos los vectores de rectangulos a vectores de enteros
            vector<int> Rint = vRectAvInt(R, int(pow(2, N)), N);

            // grabamos los vectores en el archivo
            grabarVector(Rint, archivoR);
        }
    }

    
    return 0;
}
