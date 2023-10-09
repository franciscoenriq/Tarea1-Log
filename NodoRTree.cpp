#include <iostream>
#include <vector>
#include <cstdlib> //rand() 
#include <cstdint>

// Estructura para representar un punto en el plano 2D ==========================
struct Punto
{
    uint32_t x;
    uint32_t y;

    Punto(uint32_t x, uint32_t y) : x(x), y(y) {}
};

// Función para calcular el centro de un Rectángulo ======================
Punto calcularPuntoMedio(const Punto& inferiorIzquierdo, const Punto& superiorDerecho)
{
    double medioX = (inferiorIzquierdo.x + superiorDerecho.x) / 2.0;
    double medioY = (inferiorIzquierdo.y + superiorDerecho.y) / 2.0;

    return Punto(medioX, medioY);
};

// Sobrecarga del operador de salida para imprimir un objeto Punto ======================
std::ostream& operator<<(std::ostream& os, const Punto& punto)
{
    os << "(" << punto.x << ", " << punto.y << ")";
    return os;
};

// Estructura para representar un rectangulo según 2 Puntos ======================
struct Rectangulo
{
    Punto inf_izq;
    Punto sup_der;
    Punto centro;

    Rectangulo(Punto inf_izq, Punto sup_der) : inf_izq(inf_izq), sup_der(sup_der), centro(calcularPuntoMedio(inf_izq, sup_der)) {}
};

// Clase NodoRTree que representa un nodo del RTree a partir de una lista de claves(Rectangulos)
// y una lista de hijos(Nodos)
class NodoRTree
{
public:
    // Constructor para un nodo interno
    NodoRTree(bool esHoja, int limite) : esHoja_(esHoja), m(limite) {}

    // Destructor para liberar la memoria de los nodos hijos
    ~NodoRTree();

    // Insertar un rectángulo en el nodo
    bool insertarRectangulo(const Rectangulo &rectangulo);

private:
    bool esHoja_;
    std::vector<Rectangulo> rectangulos_;
    std::vector<NodoRTree *> hijos_;
    bool estaLleno() const;
    int m;
};

NodoRTree::~NodoRTree()
{
    // Liberar la memoria de los nodos hijos (si es necesario)
    for (NodoRTree *hijo : hijos_)
    {
        delete hijo;
    }
}

bool NodoRTree::insertarRectangulo(const Rectangulo &rectangulo)
{
    // Verificar si el nodo está lleno
    if (estaLleno())
    {
        return false; // Indica que no se pudo insertar el rectángulo
    }

    // Si el nodo no está lleno, simplemente agregamos el rectángulo
    rectangulos_.push_back(rectangulo);
    return true; // Indica que el rectángulo se insertó correctamente
}

bool NodoRTree::estaLleno() const
{
    return static_cast<int>(rectangulos_.size()) >= m; // m es el límite máximo de rectángulos
}

uint32_t randomNum(uint32_t minimo = 0, uint32_t maximo = 500000)
{
    // Genera un número aleatorio entre minimo y (maximo - 1)
    uint32_t numeroAleatorio = std::rand() % (maximo - minimo) + minimo;
    return numeroAleatorio;
}

