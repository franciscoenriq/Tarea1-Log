#include "STR-RTree.cpp"
using namespace std;

// se definen los rangos en x e y para generar una nube de puntos
#define xrange 100
#define yrange 100


//se definen los rangos en x e y para generar una nube de puntos
#define xrange  100
#define yrange  100



int main()
{
    int m = 2;
    //prueba de crear un arreglo de rectángulos en base a un arreglo de puntos
    int N=11;
      ull ptr[]={9,8,10,9,
                    1,1,3,5,
                    5,5,6,6,
                    9,10,11,11,
                    3,1,4,3,
                    8,6,9,9,
                    1,2,2,4,
                    6,7,8,9,
                    7,8,9,10,
                    10,9,11,12,
                    3,4,5,6};
    vector <Rectangulo> r =  makeRectArray(ptr, 4*N);
    for (int i = 0; i<N; i++){

        cout << "Rectangulo " << i + 1 << '\n'
             << "p1: " << r[i].p1 << " p2: " << r[i].p2 << endl;
    }
    // prueba de yquicksort
    yquickSort(r, 0, N - 1);
    for (int i = 0; i < N; i++)
    {
        cout << "centro: " << r[i].centro << " " << r[i].p1 << ' ' << r[i].p2 << endl;
    }
    //prueba de calcular mbr en y
    Rectangulo mbr = calcularMBR(r,0,N);
        cout<<"MBR: "<<mbr.p1<<','<<mbr.p2<<endl;

    //otro r para comparar
    //vector <Rectangulo> r2 =  makeRectArray(ptr, 4*N);
    xquickSort(r, 0, N - 1);
    for (int i = 0; i < N; i++)
    {
        cout << "centro: " << r[i].centro <<' '<< r[i].p1 << ' ' << r[i].p2 << endl;
    }

int pos =0;
//for (const auto &val : r2)
//     {
//         if (pos == 2 || (pos - 2) & (m * 5) == 0)
//             cout << '\n'
//                  << "Nodo: ";
//         if (pos == 1)
//         {
//             cout << '\n'
//                  << "puntero: ";
//         }
//         cout << val << " ";
//         pos++;
//     }
//     cout << endl;
    //prueba de generación del binario
    vector<ull> puntosRtree = vectorRTree(r,m,ordenSTR);
    grabarVector(puntosRtree,"STRtree.bin");
    // se lee el archivo completo en cadena
    char *file2 = "STRtree.bin";
    leerBinFile(file2);


 return 0;
}
