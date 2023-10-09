#include <iostream>
#include "NodoRTree.cpp"

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

Rectangulo calcularMBR(const std::vector<Rectangulo> &listaRectangulos)
{

    if (listaRectangulos.empty())
    {
        return Rectangulo{{0, 0}, {0, 0}};
    }

    if (listaRectangulos.size() == 1)
    {
        return listaRectangulos[0];
    }

    int minX = listaRectangulos[0].inf_izq.x;
    int maxX = listaRectangulos[0].sup_der.x;
    int minY = listaRectangulos[0].inf_izq.y;
    int maxY = listaRectangulos[0].sup_der.y;

    for (size_t i = 1; i < listaRectangulos.size(); i++)
    {
        const Rectangulo &rect = listaRectangulos[i];
        minX = std::min(minX, rect.inf_izq.x);
        maxX = std::max(maxX, rect.sup_der.x);
        minY = std::min(minY, rect.inf_izq.y);
        maxY = std::max(maxY, rect.sup_der.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}
