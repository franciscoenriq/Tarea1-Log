#include <iostream>
#include <vector>
#include <tuple> 
#include <iostream>
#include <thread> 
#include <chrono> 
#include <cmath>
#include "../NodoRTree.cpp"
#include <algorithm>
#include "../auxiliares.cpp"

using namespace std;


bool compararPorX(const Rectangulo& r1, const Rectangulo& r2){
    return r1.centro.x < r2.centro.x;
}

// Comparador personalizado para ordenar por coordenada Y del centro
bool compararPorY(const Rectangulo& r1, const Rectangulo& r2){
    return r1.centro.y < r2.centro.y;
}




vector<vector<Rectangulo>> crearVectores(int n){
    vector<vector<Rectangulo>> vectorDeVectores;
    for (int i = 0; i < n; i++){
        vector<Rectangulo> vectorDeRectangulos;
        vectorDeVectores.push_back(vectorDeRectangulos);
    }
    return vectorDeVectores;
}

void strALgorithm(vector<rectangulo>& rectangulos,int M){

    int cantidadRectangulos = rectangulos.size();
    //tengo ordenado los centros, ahora necesito hacer S grupos 
    //Calculamos S
    
    double resultado = static_cast<double>(cantidadRectangulos) / M;
    if(resultado <=1){
        return;
    }
    double raizCuadrada = sqrt(resultado);

    // Redondea hacia arriba y convierte a entero
    int S = static_cast<int>(ceil(raizCuadrada));
    //Cantidad de elemetnos que tendrá cada grupo S 
    int nodosX = S*M; 
    //ordenamos todos los puntos segun su eje x , de esta manera recorremos el arreglo de forma ordenada 
    //y vamos introduciendoen cada grupo S hasta que se cumple el maximo que es S*M

    sort(rectangulos.begin(),rectangulos.end(),compararPorX);
    vector<vector<Rectangulo>> vectoresCreados = crearVectores(S);
    int vectorActual = 0;

    for (int i = 0; i < cantidadRectangulos; i++){
        // Agrega el Rectángulo actual al vector actual y actualiza el índice del vector
        vectoresCreados[vectorActual].push_back(rectangulos[i]);
        vectorActual = (vectorActual + 1) % nodosX; // Avanza al siguiente vector (cíclicamente)
    }

    rectangulos.clear();

    for(int i=0;i<S;i++){
        //ordenamos cada vector ahora segun el eje y para poder ingresarlos al verdadero MBR

        sort(vectoresCreados[i].begin(),vectoresCreados[i].end(),compararPorY);
        vector<vector<Rectangulo>> vectoresCreados2 = crearVectores(S);

        //tomamos el i-esimo vector ordenado anteriormente por el eje X 
        int cantidadElementos = vectoresCreados[i].size();
        int vectorActualY = 0;
        const vector<Rectangulo> &a = vectoresCreados[i]; 
        //para cada uno de estos vecotres creamos S vectores nuevos , los cuales seran rellenados con M elementos cada un 
        //recorremos todo el arreglo y vamos insertando , cuando llegan a haber M elementos , rellenamos el siguiente vector 
        for(int j=0;j<cantidadElementos;j++){
            vectoresCreados2[vectorActualY].push_back(a[j]);
            vectorActualY = (vectorActualY+1) % M ;
        }
        //luego recorremos estos nuevos S vectores creados , sacados de vectoresCreados[i] y los insertamos nuevamente en rectangulos 
        //despues hacemos la recursion 
        for(int k=0;k<S;k++){
            const vector<Rectangulo> &b = vectoresCreados2[k];
            int tamaño = b.size();
            for(int o=0;o<tamaño;o++){
                rectangulos.push_back(b[o]);
            }
            strALgorithm(vectoresCreados2[k],M);
        }

    }
}
