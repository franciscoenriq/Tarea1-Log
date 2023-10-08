#include "../algoritmo_2/NodoRTree.cpp"

int main()
{
    int m = 5;

    NodoRTree nodo(false, m); // Crear un nodo interno con límite de 5 rectángulos

    for (int i = 1; i <= 6; i++)
    {
        std::cout << "Iteración " << i << std::endl;
        Rectangulo rect(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum()));
        std::cout << "(" << rect.inf_izq << ", " << rect.sup_der << ", " << rect.centro << " )" << std::endl;

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
