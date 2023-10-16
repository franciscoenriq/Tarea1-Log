#include <iostream>
#include <cstdlib> //rand()
#include "NodoRTree.cpp"

using namespace std;

int randomNum(int minimo = 0, int maximo = 500000)
{
    // Genera un número aleatorio entre minimo y (maximo - 1)
    int numeroAleatorio = std::rand() % (maximo - minimo) + minimo;
    return numeroAleatorio;
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
Rectangulo calcularMBR(vector<Rectangulo> &listaRectangulos, int start, int end)
{
    int lsize = listaRectangulos.size();
    if (listaRectangulos.empty())
    {
        return Rectangulo{{0, 0}, {0, 0}};
    }

    if (lsize == 1)
    {
        return listaRectangulos[0];
    }

    int minX = listaRectangulos[start].inf_izq.x;
    int maxX = listaRectangulos[start].sup_der.x;
    int minY = listaRectangulos[start].inf_izq.y;
    int maxY = listaRectangulos[start].sup_der.y;
    if (lsize < end)
        end = lsize;
    for (int i = start; i < end; i++)
    {
        const Rectangulo &rect = listaRectangulos[i];
        minX = min(minX, (int)rect.inf_izq.x);
        maxX = max(maxX, (int)rect.sup_der.x);
        minY = min(minY, (int)rect.inf_izq.y);
        maxY = max(maxY, (int)rect.sup_der.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}

/*
Función que  genera un array que linealmente representa a un  RTree
La estructura final se disponde de la siguiente forma en memoria:

0  rectangulos     R    claves   3k   k
|------------------|----|----|----|----|
|__________________|____ ___________h__|....

---cada rectángulo contiene 4 enteros: 4*R enteros
---cada nodo contiene 4k + k enteros;
            4k para rectangulos  MBR
            k  para punteros (offset) a hijos.
--- se tienen R/m nodos en total

--- el offset unitario que más sentido tiene usar es sizeof(int) por simplicidad

*/
int offset = sizeof(int);
vector<int> vectorRTree(vector<Rectangulo> &r_vect, int m)
{

    // suponemos que el vector de rectangulos viene ordenado

    // vector que contendrá los datos a insertar en la interación actual
    vector<int> arr(1, 0);
    // se calcula la cantidad de rectángulos totales
    int R_totales = sizeof(r_vect) / sizeof(r_vect[0]);

    // se escribe r_vect en memoria como un nodo hijo (anteponiendo un offset a nulo)
    for (Rectangulo r : r_vect)
    {
        arr.push_back(0);
        arr.push_back(r.inf_izq.x);
        arr.push_back(r.inf_izq.y);
        arr.push_back(r.sup_der.x);
        arr.push_back(r.sup_der.y);
    } // con esto, se tienen R rectángulos compuestos por 5 enteros.

    // se calcula la cantidad de mbrs a calcular
    // Notar que esto también corresponde a cuantos nodos se crearán en esta iteración
    int n_mbr = R_totales / m;

    // se calculan los mbrs
    vector<Rectangulo> mbrs(n_mbr, Rectangulo(Punto(0, 0), Punto(0, 0)));
    // Se recorre el vector de rectángulos para generar los MBR
    for (int rect = 0; rect < n_mbr; rect++)
    {
        mbrs[rect] = calcularMBR(r_vect, rect * m, rect * (m + 1));
    }

    // se setablece la posición del primer nodo hijo
    int offset = 1;
    // Se van insertando los nodos padre de a m datos
    for (int padres = 0; padres < n_mbr; padres++)
    {
        int ds = m * padres; // desplazamiento para los mbrs
        if (ds > n_mbr)
            ds = n_mbr; // si ds > n_mbr pasan cosas feas :s
        for (int k = 0; k < m; k++)
        { // primero los "punteros" a hijos
            arr.push_back(offset);
            offset += 5; // se avanza a la primera posición de la siguiente hoja
        }
        for (int k = 0; k < m; k++)
        { // luego las claves (rectángulos mbr)
            arr.push_back(mbrs[k + ds].inf_izq.x);
            arr.push_back(mbrs[k + ds].inf_izq.y);
            arr.push_back(mbrs[k + ds].sup_der.x);
            arr.push_back(mbrs[k + ds].sup_der.y);
        }
    }
    // si la cantidad de mbrs fué mayor a la cantidad de datos guardables en un nodo,
    // se repite una variación del algoritmo anterior.
    if (n_mbr > m)
    {
        int data = n_mbr;
        /* se crean vectores para contener los mbrs anteriores y a los actuales.
          Notar que el tamañano efectivamente utilizado nunca puede ser mayor a n_mbr original */
        vector<Rectangulo> ans_mbrs = mbrs;
        vector<Rectangulo> new_mbrs(n_mbr / m, Rectangulo(Punto(0, 0), Punto(0, 0)));

        while (data > n_mbr)
        {
            /*se (re)calcula la cantidad de mbrs a calcular. Notar que esto también corresponde a
             cuantos nodos se crearán en esta iteración, y hasta que nivel se usarán los vectores
             anteriores */
            int nmbr = data / m;
            // Se recorre el vector de rectángulos para generar los MBR
            for (int rect = 0; rect < nmbr; rect++)
            {
                new_mbrs[rect] = calcularMBR(ans_mbrs, rect * m, rect * (m + 1));
            }
            // Se van insertando datos a los  nodos ascendentes de a m datos (hasta llenar los)
            for (int padres = 0; padres < nmbr; padres++)
            {
                for (int k = 0; k < m; k++)
                { // primero los "punteros" a hijos
                    arr.push_back(offset);
                    offset += 5 * m; // se avanza a la primera posición del siguiente nodo
                }
                int K = m; // cantidad de datos a insertar
                if (nmbr < n_mbr)
                    K = n_mbr; // si ya se ha logrado que data = nmbr < n_mbrm, K = nmbr
                for (int k = 0; k < K; k++)
                { // luego las claves (rectángulos mbr)
                    arr.push_back(new_mbrs[k].inf_izq.x);
                    arr.push_back(new_mbrs[k].inf_izq.y);
                    arr.push_back(new_mbrs[k].sup_der.x);
                    arr.push_back(new_mbrs[k].sup_der.y);
                }
            }
            // ahora se new_mbr debe ser ans_mbr
            for (int i = 0; i < nmbr; i++)
            {
                ans_mbrs[i] = new_mbrs[i];
            }
            // Se actualiza el valor de data
            data = data / m;
        }
    }
    // retornamos el vector que representa al arbol
    return arr;
}
