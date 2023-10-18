#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
#include "../auxiliares.cpp"

using namespace std;

vector<ull> leerBinPorBits(const char *fileName, ull n_valores_desde_el_inicio, ull n_valores_siguientes)
{
    ull bitInicial = ((n_valores_desde_el_inicio-1) * sizeof(ull) * 8);
    //cout << "bit inicial" << bitInicial << endl;
    ull bitFinal = ((n_valores_desde_el_inicio+n_valores_siguientes-1) * sizeof(ull) * 8);
    //cout << "bit final" << bitFinal << endl;
    vector<ull> resultado;
    ifstream archivo(fileName, ios::binary);

    if (!archivo)
    {
        cerr << "No se pudo abrir el archivo." << endl;
        return resultado;
    }

    // Calcular la posición de inicio y fin de los valores
    ull startValue = bitInicial / 64; // Valor de inicio en el arreglo de valores
    ull endValue = bitFinal / 64;     // Valor de fin en el arreglo de valores

    // Calcular el desplazamiento dentro del primer valor
    int bitOffset = bitInicial % 64;

    // Calcular la cantidad de valores que se leerán
    ull numValues = endValue - startValue + 1;

    archivo.seekg(startValue * sizeof(ull), ios::beg);

    for (ull i = 0; i < numValues; i++)
    {
        ull value;
        archivo.read(reinterpret_cast<char *>(&value), sizeof(ull));

        if (i == 0)
        {
            // Ajustar el valor inicial si hay un desplazamiento
            value >>= bitOffset;
        }

        if (i == numValues - 1)
        {
            // Limpiar los bits adicionales al final si es necesario
            int bitsRestantes = 64 - (bitFinal % 64 + 1);
            value &= (1ULL << bitsRestantes) - 1;
        }

        resultado.push_back(value);
    }

    archivo.close();

    return resultado;
}

// funcion para leer los binarios de q y R, pasar true si quieres Q, contrario false
vector<Rectangulo> recuperarRectangulosFromBin(ull n, bool Q)
{
    ull numero = n;
    char *archivoBase;
    if (Q)
    {
        archivoBase = "Q_for_n_2_";
    }
    else
    {
        archivoBase = "R_for_n_2_";
    }

    stringstream ss;
    ss << archivoBase << numero;

    char *archivo = const_cast<char *>(ss.str().c_str());

    vector<ull> valores = recuperarVectorFromBinFile(archivo);

    cout << "  " << endl;
    cout << "cantidad de valores: " << valores.size() << endl;

    // for (ull val : valores)
    // {
    //     cout << val << endl;
    // }

    vector<Rectangulo> rectangulos;
    for (ull i = 0; i < valores.size(); i += 4)
    {
        Punto inf_izq = Punto(valores[i], valores[i + 1]);
        Punto sup_der = Punto(valores[i + 2], valores[i + 3]);

        Rectangulo rect = Rectangulo(inf_izq, sup_der);
        // cout << rect << endl;
        rectangulos.push_back(rect);
    }

    return rectangulos;
}

int main()
{
    // vector con los valores de n a usar
    vector<ull> n = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25};

    // vector<Rectangulo> R, Q;
    // cout << "  " << endl;
    // cout << "R" << endl;
    // R = recuperarRectangulosFromBin(10, 0);
    // cout << "  " << endl;
    // cout << "Q" << endl;
    // Q = recuperarRectangulosFromBin(10, 1);

    // =========================================
    // for (int i = 1; i<20; i++){
    // ull n_valores = i;

    // vector<ull> resultado = leerBinPorBits("HilbertRTree.bin", 0, n_valores);

    // for (ull valor : resultado)
    // {
    //     cout << "valor: " << valor << endl;

    // }
    // cout << " " << endl;
    // }
    // =========================================
    // suponemos un m, que va a ser el que se uso para hilbert para pribar:
    ull m = 3;
    ull puntero_a_raiz = leerBinPorBits("HilbertRTree.bin", 1, 1)[0];
    vector<ull> nodoRaiz =  leerBinPorBits("HilbertRTree.bin", puntero_a_raiz, m*5-1 );

// for (ull valor : nodoRaiz)
//     {
//         cout << "valor: " << valor << endl;

//     }
//     cout << " " << endl;
    

    // =========================================
    return 0;
}
