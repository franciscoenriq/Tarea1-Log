#include <iostream>
#include "NodoRTree.cpp"

using namespace std;

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
Rectangulo calcularMBR(vector<Rectangulo>& listaRectangulos,int start, int end) {
    int lsize = listaRectangulos.size();
    if (listaRectangulos.empty()) {
        return Rectangulo{{0, 0}, {0, 0}};
    }

    if (lsize ==1){
        return listaRectangulos[0];
    }

    uint32_t minX = listaRectangulos[start].p1.x;
    uint32_t maxX = listaRectangulos[start].p2.x;
    uint32_t minY = listaRectangulos[start].p1.y;
    uint32_t maxY = listaRectangulos[start].p2.y;
    if (lsize < end)
        end = lsize;
    for (size_t i = start; i < end; i++) {
        const Rectangulo& rect = listaRectangulos[i];
        minX = min(minX, (uint32_t)rect.p1.x);
        maxX = max(maxX, (uint32_t)rect.p2.x);
        minY = min(minY, (uint32_t)rect.p1.y);
        maxY = max(maxY, (uint32_t)rect.p2.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}