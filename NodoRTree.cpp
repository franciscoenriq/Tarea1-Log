#include <iostream>
#include <vector>
#include <cstdint>

// Estructura para representar un punto en el plano 2D ==========================
struct Punto
{
    int x;
    int y;

    Punto(int x, int y) : x(x), y(y) {}
};

// Función para calcular el centro de un Rectángulo ======================
Punto calcularPuntoMedio(const Punto &inferiorIzquierdo, const Punto &superiorDerecho)
{
    double medioX = (inferiorIzquierdo.x + superiorDerecho.x) / 2.0;
    double medioY = (inferiorIzquierdo.y + superiorDerecho.y) / 2.0;

    return Punto(medioX, medioY);
};

// Sobrecarga del operador de salida para imprimir un objeto Punto ======================
std::ostream &operator<<(std::ostream &os, const Punto &punto)
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
