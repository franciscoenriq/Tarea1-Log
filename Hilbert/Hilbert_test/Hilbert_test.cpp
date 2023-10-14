#include "../algoritmo_2/Hilbert.cpp"

int main()
{
    // Prueba 1
    std::vector<std::tuple<Punto, int>> puntos1;
    puntos1.push_back(std::make_tuple(Punto(3, 2), 1)); // d=11
    puntos1.push_back(std::make_tuple(Punto(5, 1), 2)); // d=2
    puntos1.push_back(std::make_tuple(Punto(2, 4), 3)); // d=14
    puntos1.push_back(std::make_tuple(Punto(1, 5), 4)); // d=2

    ordenarHilbert(puntos1);

    std::cout << "Prueba 1:" << std::endl;
    for (const auto &punto : puntos1)
    {
        std::cout << std::get<1>(punto) << " ";
    }
    std::cout << std::endl;

    // Prueba 2
    std::vector<std::tuple<Punto, int>> puntos2;
    puntos2.push_back(std::make_tuple(Punto(5, 5), 1)); // d=2
    puntos2.push_back(std::make_tuple(Punto(3, 4), 2)); // d=15
    puntos2.push_back(std::make_tuple(Punto(2, 3), 3)); // d= 9
    puntos2.push_back(std::make_tuple(Punto(1, 2), 4)); // d=7

    ordenarHilbert(puntos2);

    std::cout << "Prueba 2:" << std::endl;
    for (const auto &punto : puntos2)
    {
        std::cout << std::get<1>(punto) << " ";
    }
    std::cout << std::endl;

    return 0;
}