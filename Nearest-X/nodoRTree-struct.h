#include <utility>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>
#define PGSIZE 4096 /* El tamaño de página. Depende de la máquina */
#define d_int int
using namespace std;

/* Estructura para representar un punto en un plano 2D 
    d_int x: abscisa
    d_int y: ordenada */
struct Punto{
    d_int x;
    d_int y;

    Punto(d_int x, d_int y) : x(x), y(y) {}
};
// Sobrecarga del operador de salida para imprimir un objeto Punto ======================
ostream& operator<<(ostream& os, const Punto& punto){
    os << "(" << punto.x << ", " << punto.y << ")";
    return os;
};


/* Función para calcular el centro de un Rectángulo */
Punto calcularPuntoMedio(const Punto& inferiorIzquierdo, const Punto& superiorDerecho){
    
    double medioX = (inferiorIzquierdo.x + superiorDerecho.x) / 2.0;
    double medioY = (inferiorIzquierdo.y + superiorDerecho.y) / 2.0;

    return Punto(medioX, medioY);
};
// ESTRUCTURA=====================================================
/* Estructura para representar un rectángulo. Contiene un campo extra  para facilitar su manipulación en la construcción de árboles*/
struct Rectangulo{
    Punto p1;
    Punto p2;
    Punto centro;
    Rectangulo(Punto p1, Punto p2) : p1(p1), p2(p2),  centro(calcularPuntoMedio(p1,p2)) {}
};

/* Clase para representar un nodoRtree: contiene una lista de claves (rectángulos) y una lista de hijos(nodos) */
class NodoRTree{
    public:
    /* Constructor para un nodo interno */
    NodoRTree(bool esHoja, int limite) : esHoja_(esHoja), m(limite) {}
    
    /* Destructor  para liberar la memoria de todos los nodos hijos*/
    ~NodoRTree();
    /* función para insertar un rectángulo en el nodo */
    bool insertarRectangulo(const Rectangulo &rectangulo);
    void insertarHijo(const NodoRTree hijo);
    /*Función para rescatar los rectángulos de un nodo*/
    void *rescatarRectangulos();
    private:
        bool esHoja_;
        /* cada nodo contiene k rectángulos de los MBR de sus hijos*/
        vector<Rectangulo> rectangulos_;
        /* cada nodo contiene k punteros a los hijos de los MBR anteriores*/
        vector<NodoRTree *> hijos_;
        bool estaLleno() const;
        int m;
};

// OPERADORES CLASE ==============================================================
/* función para liberar memoria de los hijos de un nodo*/
NodoRTree::~NodoRTree(){
    // Liberar la memoria de los nodos hijos (si es necesario)
    for (void *hijo : hijos_){
        delete hijo;
    }
}
/* Función para insertar hijos en un nodo */
void NodoRTree::insertarHijo(NodoRTree hijo){
    hijos_.push_back(&hijo);

}
/* Función para rescatar los rectángulos de un nodo */
void* NodoRTree::rescatarRectangulos(){
    if (esHoja_){
        return (&rectangulos_[0]);
    }
    return &rectangulos_;
}
/* Función para insertar rectángulos en un nodo */
bool NodoRTree::insertarRectangulo(const Rectangulo &rectangulo)
{
    // Verificar si el nodo está lleno
    if (estaLleno()){
        return false; // Indica que no se pudo insertar el rectángulo
    }

    // Si el nodo no está lleno, simplemente agregamos el rectángulo
    rectangulos_.push_back(rectangulo);
    return true; // Indica que el rectángulo se insertó correctamente

}

bool NodoRTree::estaLleno() const{
    return static_cast<int>(rectangulos_.size()) >= m; // m es el límite máximo de rectángulos
}


