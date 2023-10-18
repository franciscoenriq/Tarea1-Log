#include <iostream>
#include <fstream>
#include <bitset>
#include <sstream>
#include "../auxiliares.cpp"

using namespace std;

//  funcion para leer por partes el archivo donde se guarda el R-Tree
vector<ull> leerBinPorBits(const char *fileName, ull n_valores_desde_el_inicio, ull n_valores_siguientes)
{
    ull bitInicial = ((n_valores_desde_el_inicio - 1) * sizeof(ull) * 8);
    cout << "bit inicial" << bitInicial << endl;
    ull bitFinal = ((n_valores_desde_el_inicio + n_valores_siguientes - 1) * sizeof(ull) * 8);
    cout << "bit final" << bitFinal << endl;
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
    ull bitOffset = bitInicial % 64;

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
            ull bitsRestantes = 64 - (bitFinal % 64 + 1);
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

void busqueda(ull m, Rectangulo C, ull puntero_buscado, ull puntero_anterior, const char *fileName, ull *accesos, vector<Rectangulo> &resultado)
{
    if (puntero_buscado < puntero_anterior)
    {
        // si llego acá estoy en una hoja
        cout << "llegamos a una hoja" << endl;
        // pedimos el nodo
        vector<ull> nodo = leerBinPorBits(fileName, puntero_buscado, m * 5 - 1);
        // como leimos, sumamos 1 acceso
        (*accesos)++;
        cout << "cantidad de accesos: " << *accesos<< endl;
        // vamos a rescatar los rectangulos en el nodo
        vector<ull> rectangulos(nodo.begin() + m, nodo.begin() + m * 4 + 1);
        // veremos si intersectan
        
        for (ull i = 0; i < m*4; i += 4)
        {
            if (rectangulos[i] != 0) // si algo es 0, entonces es lo que usamos para rellenar y lo ignoramos
            {
                Rectangulo rect(Punto(rectangulos[i], rectangulos[i + 1]), Punto(rectangulos[i + 3], rectangulos[i + 4]));
                // si el rectangulo intersecta con C, lo agregamos al resultado
                if (intersectan(C, rect))
                {
                    resultado.push_back(rect);
                }
            }
        }
    }
    else
    {
        // en este caso estamos en un nodo interno
        cout << "llegamos a un nodo interno... " << endl;
        // pedimos el nodo
        vector<ull> nodo = leerBinPorBits(fileName, puntero_buscado, m * 5 - 1);
        // como leimos, sumamos 1 acceso
        (*accesos)++;
        cout << "accesos: " << *accesos << endl;
        // vamos a pedir las referencias en el nodo
        vector<ull> referencias(nodo.begin(), nodo.begin() + m);
        // vamos a rescatar los rectangulos en el nodo
        vector<ull> rectangulos(nodo.begin() + m, nodo.begin() + m * 4 + 1);

        // veremos si intersectan
        for (ull i = 0; i < m*4; i += 4)
        {
            cout << "entramos a ver si intersectan..." << endl;
            if (rectangulos[i] != 0) // si algo es 0, entonces es lo que usamos para rellenar y lo ignoramos
            {
                Rectangulo rect(Punto(rectangulos[i], rectangulos[i + 1]), Punto(rectangulos[i + 3], rectangulos[i + 4]));
                cout << "rectangulo creado: " << rect << endl;
                // si el rectangulo intersecta con C, seguimos por esa rama
                if (intersectan(C, rect))
                {
                    cout << "intersectaron" << endl;
                    ull nuevo_puntero = referencias[i % m];
                    return busqueda(m, C, nuevo_puntero, puntero_buscado, fileName, accesos, resultado);
                }
                else{
                    cout << "NO intersectaron" << endl;
                }
            }
        }
    }
    return;
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
    // for (ull i = 1; i<20; i++){
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

    // ull puntero_a_raiz = leerBinPorBits("HilbertRTree.bin", 1, 1)[0];
    // ull puntero_actual = puntero_a_raiz;
    // vector<ull> nodoRaiz = leerBinPorBits("HilbertRTree.bin", puntero_a_raiz, m * 5 - 1);

    vector<Rectangulo> resultado;
    ull accesos; // inicia en 1 porque pedimos la raiz
    char *fileName = "HilbertRTree.bin";

    ull puntero_a_raiz = leerBinPorBits(fileName, 1, 1)[0];
    accesos++;

    Rectangulo Q(Punto(257419, 47360), Punto(280047, 46754));
    busqueda(m, Q, puntero_a_raiz, puntero_a_raiz, fileName, &accesos, resultado);

    cout << "resultado:  " << endl;
    for (Rectangulo rect : resultado)
    {
        cout << rect << endl;
    }
    cout << " " << endl;

    // =========================================
    return 0;
}
