#include "NearestXRtree.cpp"
using namespace std;

//se definen los rangos en x e y para generar una nube de puntos
#define xrange  100
#define yrange  100


uint32_t randomNum(uint32_t minimo = 0, uint32_t maximo = 500000){
    // Genera un número aleatorio entre minimo y (maximo - 1)
    uint32_t numeroAleatorio = rand() % (maximo - minimo) + minimo;
    return numeroAleatorio;
}

int main(){
    int m = 5;

    NodoRTree nodo(false, m); // Crear un nodo interno con límite de 5 rectángulos

    for (int i = 1; i <= 6; i++)
    {
        std::cout << "Iteracion " << i << std::endl;
        Rectangulo rect(Punto(randomNum(), randomNum()), Punto(randomNum(), randomNum()));
        std::cout << "(" << rect.p1 << ", " << rect.p2 << ", " << rect.centro << " )" << std::endl;

        if (nodo.insertarRectangulo(rect))
        {
            std::cout << "Rectángulo insertado con exito." << std::endl;
        }
        else
        {
            std::cout << "No se pudo insertar el rectangulo." << std::endl;
        }
    }
    //prueba de crear un arreglo de rectángulos en base a un arreglo de puntos
    int N;
      d_int ptr[]={9,8,8,8,
                    2,1,6,7,
                    1,1,20,2,
                    9,0,0,9,
                    12,1,1,2};
    vector <Rectangulo> r =  makeRectArray(ptr,20);
    for (int i = 0; i<5; i++){

        cout << "Rectangulo "<<i+1 << '\n'
        << "p1: "<<r[i].p1 <<" p2: "<< r[i].p2 << endl;
    }
    N = 5;
    //prueba de xquicksort
    xquickSort(r,0,N-1);
    for (int i = 0;i<N;i++){
        cout<<"centro: "<<r[i].centro.x <<" " << r[i].p1<<' '<<r[i].p2 <<endl;
    }
    //prueba de calcular mbr   
    Rectangulo mbr = calcularMBR(r,0,N);
        cout<<"MBR: "<<mbr.p1<<','<<mbr.p2<<endl;
    return 0;
}