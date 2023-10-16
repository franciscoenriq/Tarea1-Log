#include "../auxiliares.cpp"

int main()
{
    // Pruebas para la función intersectan
    Rectangulo rect1(Punto(0, 0), Punto(4, 4));
    Rectangulo rect2(Punto(2, 2), Punto(6, 6));
    Rectangulo rect3(Punto(5, 5), Punto(8, 8));

    if (intersectan(rect1, rect2))
    {
        std::cout << "rect1 y rect2 se intersectan." << std::endl;
    }
    else
    {
        std::cout << "rect1 y rect2 no se intersectan." << std::endl;
    }

    if (intersectan(rect1, rect3))
    {
        std::cout << "rect1 y rect3 se intersectan." << std::endl;
    }
    else
    {
        std::cout << "rect1 y rect3 no se intersectan." << std::endl;
    }

    // Pruebas para la función calcularMBR
    std::vector<Rectangulo> listaRectangulos;
    listaRectangulos.push_back(Rectangulo(Punto(1, 1), Punto(4, 4)));
    listaRectangulos.push_back(Rectangulo(Punto(3, 3), Punto(6, 6)));
    listaRectangulos.push_back(Rectangulo(Punto(7, 7), Punto(9, 9)));

    Rectangulo mbr = calcularMBR(listaRectangulos);
    std::cout << "MBR calculado: inf_izq=(" << mbr.inf_izq.x << ", " << mbr.inf_izq.y << "), sup_der=(" << mbr.sup_der.x << ", " << mbr.sup_der.y << ")" << std::endl;

    return 0;
}