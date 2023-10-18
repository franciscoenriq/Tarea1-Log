#include <iterator>
#include <functional>
#include <cstdlib> //rand()
#include <iostream>
#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include "NodoRTree.cpp"

#define d_int int
using namespace std;
typedef unsigned long long ull;

ull leerBinFile(char *fileName)
{

    std::ifstream archivo(fileName, std::ios::binary);

    if (!archivo)
    {
        std::cerr << "No se pudo abrir el archivo." << std::endl;
        return 1;
    }

    std::vector<ull> enteros;
    ull entero;

    while (archivo.read(reinterpret_cast<char *>(&entero), sizeof(ull)))
    {
        enteros.push_back(entero);
    }

    archivo.close();

    // mostramos lo que habia
    std::cout << "   archivo:     " << endl;
    for (ull num : enteros)
    {
        std::cout << num << " ";
    }
    return 0;
}

ull randomNum(ull minimo = 0, ull maximo = 500000)
{
    // Crear un generador de números aleatorios
    random_device rd;  // Usar un dispositivo de hardware para generar semillas aleatorias
    mt19937 gen(rd()); // Utilizar el algoritmo Mersenne Twister

    // Definir el rango de números aleatorios
    ull min = minimo;
    ull max = maximo;

    // Crear una distribución uniforme en el rango deseado
    std::uniform_int_distribution<> distribution(min, max);

    // Generar un número aleatorio
    ull numeroAleatorio = distribution(gen);

    return numeroAleatorio;


}

/* función auxiliar que genera un vector de rectángulos en base a N puntos */
vector<Rectangulo> makeRectArray(ull arr[], int N)
{
    int L = N >> 2;
    vector<Rectangulo> vec(L, Rectangulo(Punto(0, 0), Punto(0, 0)));
    for (int i = 0; i < L; i++)
    {
        d_int x1 = arr[i * 4];
        d_int y1 = arr[i * 4 + 1];
        d_int x2 = arr[i * 4 + 2];
        d_int y2 = arr[i * 4 + 3];
        Rectangulo rect = Rectangulo(Punto(x1, y1), Punto(x2, y2));
        vec[i] = rect;
    }
    return vec;
}

bool intersectan(Rectangulo rect1, Rectangulo rect2)
{
    // vemos los casos donde no se intersectan
    if (rect1.sup_der.x < rect2.inf_izq.x || // rect1 está a la izquierda de rect2
        rect2.sup_der.x < rect1.inf_izq.x || // rect2 está a la izquierda de rect1
        rect1.sup_der.y < rect2.inf_izq.y || // rect1 está por encima de rect2
        rect2.sup_der.y < rect1.inf_izq.y)   // rect2 está por encima de rect1
    {
        return false;
    }

    // si no se cumplen las condiciones anteriores, se intersectan
    return true;
}

/*Función auxiliar que encuentra un MBR en el rango [start,end] de una lista de rectangulos*/
Rectangulo calcularMBR(vector<Rectangulo> &listaRectangulos, ull start, ull end)
{
    ull lsize = listaRectangulos.size();
    if (listaRectangulos.empty())
    {
        return Rectangulo{{0, 0}, {0, 0}};
    }

    if (lsize == 1)
    {
        return listaRectangulos[0];
    }

    ull minX = listaRectangulos[start].inf_izq.x;
    ull maxX = listaRectangulos[start].sup_der.x;
    ull minY = listaRectangulos[start].inf_izq.y;
    ull maxY = listaRectangulos[start].sup_der.y;
    if (lsize < end)
        end = lsize;
    for (ull i = start; i < end; i++)
    {
        const Rectangulo &rect = listaRectangulos[i];
        minX = min(minX, (ull)rect.inf_izq.x);
        maxX = max(maxX, (ull)rect.sup_der.x);
        minY = min(minY, (ull)rect.inf_izq.y);
        maxY = max(maxY, (ull)rect.sup_der.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}

/*Función auxiliar para grabar un vector en memoria */
void grabarVector(vector<ull> vect, char *fileName)
{
    FILE *arch;
    arch = fopen(fileName, "wt");
    if (arch)
    {
        std::cout << "Iniciando escritura ..." << std::endl;
        for (ull data : vect)
        {
            fwrite(&data, sizeof(ull), 1, arch);
        }

        std::cout << "Archivo '" << fileName << "' creado y datos escritos con éxito." << std::endl;
    }
    else
    {
        std::cerr << "No se pudo abrir el archivo '" << fileName << "'." << std::endl;
    }
    fclose(arch);
}
/*
Función que  genera un array que linealmente representa a un  RTree
La estructura final dispone de los datos en nodos de la siguiente forma en memoria

0 1    k+1  claves (rects)    5k+1 otro nodo...
|-|-----|-----|-----|-----|-----|
|_|hijos|_______________________|.......

---cada rectángulo contiene 4 enteros + 0: 5*R enteros
---cada nodo contiene 4k + k enteros;
            4k para rectangulos  MBR
            k  para punteros (offset) a hijos.
--- se tienen R/m nodos en total
*/
vector<ull> vectorRTree(vector<Rectangulo> &r_vect, ull m,
                        function<void(vector<Rectangulo> &)> funcOrden)
{
    // suponemos que los rectangulos vienen ordenados

    //vector que contendrá los datos a insertar en la iteración actual
    vector <ull> arr(1,0);
    // se calcula la cantidad de rectángulos totales
    ull R_totales = r_vect.size();
    cout << "Size of vector: " << R_totales << endl;
    // Se ordenan los rectángulos del vector de entrada según el método
    funcOrden(r_vect);
    // PRIMERA ITERACIÓN
    // se calcula la cantidad de mbrs a calcular. Esto puede variar dependiendo del método
    // Notar que esto también corresponde a cuantos nodos se crearán en esta iteración
    ull n_mbr = R_totales / m;
    if (R_totales % m != 0 && R_totales != 1)
        n_mbr++; // si existe algun resto, se debe calcular un mbr más
    // contador de inserciones
    ull data = 0;
    // algoritmo de construcción
    ull offset = 2 + m; // se establece la posición del primer nodo hijo
    // Se van insertando los nodos padre de a m datos
    for (ull padres = 0; padres < n_mbr; padres++)
    {
        ull ds = m * padres; // desplazamiento de los padres por los mbrs
        for (ull k = 0; k < m; k++)
        { // primero los "punteros" a hijos
            if (k >= R_totales)
                arr.push_back(0); // Hay menos de k hijos reales para el nodo
            else
                arr.push_back(offset);
            offset += 5; // se avanza a la primera posición de la siguiente hoja
        }
        for (ull k = 0; k < m; k++)
        { // luego las claves (rectángulos de hijos)
            if (k + ds >= R_totales)
            { // Hay menos de k mbr reales para el nodo
                arr.push_back(0);
                arr.push_back(0);
                arr.push_back(0);
                arr.push_back(0);
            }
            else
            {
                arr.push_back(r_vect[k + ds].inf_izq.x);
                arr.push_back(r_vect[k + ds].inf_izq.y);
                arr.push_back(r_vect[k + ds].sup_der.x);
                arr.push_back(r_vect[k + ds].sup_der.y);
                data++;
            }
        }
        ds += 1;
    }
    offset = 2; // se retrocede el offset al primer nodo hijo
    // Se calculan los MBR
    cout << "Starting..."
         << "MBRs to calculate: " << n_mbr << endl;
    // se calculan los mbrs
    vector<Rectangulo> mbrs(n_mbr, Rectangulo(Punto(0, 0), Punto(0, 0)));
    // Se recorre el vector de rectángulos para generar los MBR.
    for (ull rect = 0; rect < n_mbr; rect++)
    {
        mbrs[rect] = calcularMBR(r_vect, rect * m, (rect + 1) * m);
    }
    // DEBUG
    for (Rectangulo x : mbrs)
    {
        cout << "MBR: " << x.inf_izq.x << ',' << x.inf_izq.y << ';' << x.sup_der.x << ',' << x.sup_der.y << endl;
    }
    // si la cantidad de mbrs fué mayor a la cantidad de datos guardables en un nodo,
    // se repite una variación del algoritmo anterior.
    if (R_totales > m)
    {
        ull last = 0;
        ull rects = n_mbr;
        while (true)
        {
            // se ordenan los MBR según el método
            funcOrden(mbrs);
            ull ds = 0;
            for (ull padres = 0; padres < rects; padres++)
            {
                // inserción punteros a hijos
                for (ull k = 0; k < m; k++)
                {
                    if (k + ds >= n_mbr)
                        arr.push_back(0); // Hay menos de k hijos reales para el nodo
                    else
                    {
                        arr.push_back(offset);
                        offset += 5 * m; // se avanza a la primera posición de la siguiente hoja
                    }
                }
                // inserción de claves (rectángulos de hijos)
                for (ull k = 0; k < m; k++)
                {
                    if (k + ds >= n_mbr)
                    { // Hay menos de k mbr reales para el nodo
                        arr.push_back(0);
                        arr.push_back(0);
                        arr.push_back(0);
                        arr.push_back(0);
                        rects--;
                    }
                    else
                    {
                        arr.push_back(mbrs[k + ds].inf_izq.x);
                        arr.push_back(mbrs[k + ds].inf_izq.y);
                        arr.push_back(mbrs[k + ds].sup_der.x);
                        arr.push_back(mbrs[k + ds].sup_der.y);
                        data++;
                    }
                }
                ds += m;
            }
            if (rects == 0)
                break;
            if (last) // si la siguiente es la última iteración, se rompe el bucle
                break;
            // se re-calculan los MBR
            ull br = n_mbr;
            n_mbr = n_mbr / m;
            rects = n_mbr;
            // Se guarda cuantos elemntos será necesario quitar del vector;
            if (br != 1 && br % m != 0)
                n_mbr++;
            // Se recorre el vector de mbrs para generar otros MBR
            if (n_mbr > 0)
            {
                cout << "MBRs to calculate: " << n_mbr << endl;
                vector<Rectangulo> newmbr(n_mbr, Rectangulo(Punto(0, 0), Punto(0, 0)));
                for (ull rect = 0; rect < n_mbr; rect++)
                {
                    newmbr[rect] = calcularMBR(mbrs, rect * m, (rect + 1) * m);
                }
                // se quitan los espacios innecesarios de mbrs
                mbrs = vector<Rectangulo>(n_mbr, Rectangulo(Punto(0, 0), Punto(0, 0)));
                for (ull i = 0; i < n_mbr; i++)
                {
                    mbrs[i] = newmbr[i];
                }
                // DEBUG
                for (Rectangulo x : mbrs)
                {
                    cout << "MBR*: " << x.inf_izq.x << ',' << x.inf_izq.y << ';' << x.sup_der.x << ',' << x.sup_der.y << endl;
                }
            }
            if (n_mbr - 1 == 0)
                last = 1; // se marca como ultima la siguiente iteración
        }
    }
    // Se escribe el puntero al nodo raíz
    arr[0] = offset;
    // Se retorna el vector creado
    return arr;
}
