#include <iostream>
#include <vector>
#include <cstdlib> // Necesario para std::rand() y std::srand()

struct Punto
{
    int x;
    int y;

    Punto(int x, int y) : x(x), y(y) {}
};

struct Rectangulo
{
    int x1;
    int y1;
    int x2;
    int y2;

    Rectangulo(int x1, int y1, int x2, int y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
};

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

int randomNum(int minimo = 0, int maximo = 100)
{
    // Genera un número aleatorio entre minimo y (maximo - 1)
    int numeroAleatorio = std::rand() % (maximo - minimo) + minimo;
    return numeroAleatorio;
}

int main()
{
    int m = 5;

    NodoRTree nodo(false, m); // Crear un nodo interno con límite de 5 rectángulos

    for (int i = 1; i <= 6; i++)
    {
        std::cout << "Iteración " << i << std::endl;
        Rectangulo rect(randomNum(), randomNum(), randomNum(), randomNum());
        if (nodo.insertarRectangulo(rect))
        {
            std::cout << "Rectángulo insertado con éxito." << std::endl;
        }
        else
        {
            std::cout << "No se pudo insertar el rectángulo." << std::endl;
        }
    }

    return 0;
}
