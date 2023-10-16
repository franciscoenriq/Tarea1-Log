#include "NearestXRtree.cpp"
using namespace std;

// se definen los rangos en x e y para generar una nube de puntos
#define xrange 100
#define yrange 100



int main()
{
    int m = 2;
    // prueba de crear un arreglo de rectángulos en base a un arreglo de puntos
    int N = 7;
    int ptr[] = {9, 8, 10, 9,
                1, 1, 3, 2,
                5, 5, 6, 6,
                9, 10, 11, 11,
                3, 1, 4, 3,
                8, 6, 9, 9,
                3, 4, 5, 6};
    vector<Rectangulo> r = makeRectArray(ptr, N * 4);
    for (int i = 0; i < N; i++)
    {

        cout << "Rectangulo " << i + 1 << '\n'
             << "inf_izq: " << r[i].inf_izq << " sup_der: " << r[i].sup_der << endl;
    }
    // prueba de xquicksort
    xquickSort(r, 0, N - 1);
    for (int i = 0; i < N; i++)
    {
        cout << "centro: " << r[i].centro.x << " " << r[i].inf_izq << ' ' << r[i].sup_der << endl;
    }
    // prueba de calcular mbr
    Rectangulo mbr = calcularMBR(r, 0, N);
    cout << "MBR: " << mbr.inf_izq << ',' << mbr.sup_der << endl;
    // prueba de generación del binario
    nX_RTree(r, m);
    // prueba de lectura del binario

    FILE *arch = fopen("xRTree", "rb");
    // int blk_size=(sizeof(int));
    // int rect[5*N];
    int nll[1];
    fread(nll, sizeof(int), 1, arch); // se lee el nulo
    // fread(rect,sizeof(int),5*N,arch);      //se leen rectángulos

    // Notar que de esta parte en adelante, es posible recorrer o rescatar
    // los valores del binario usando un ciclo while e indicando offsets múltiplos
    //  de 5*m
    int size = N / m; // tamaño del siguiente sector de nodos
    if (size % 2 != 0 && size != 1)
        size++;
    int n1[5 * m * size];                       // tamaño (rectangulo +puntero)*capacidad nodo * cantidad mbrs
    fread(n1, sizeof(int), 5 * size * m, arch); // se leen los nodos padres

    size = size / m;
    if (size % 2 != 0 && size != 1)
        size++;
    int n2[5 * m * size];
    fread(n2, sizeof(int), 5 * m * size, arch); // se leen los nodos abuelos

    size = size / m;
    if (size % 2 != 0 && size != 1)
        size++;
    int n3[5 * m * size];
    fread(n3, sizeof(int), 5 * m * size, arch); // se lee la raíz
    fclose(arch);                               // se cierra el archivo

    // impresión de la información del binario
    int ct = 0;
    /*cout<<"Arbol con m = "<< m <<endl;
    cout<<"Hojas: "<<'\n';
    for (int x: rect){
        if (ct == 5){   //es posible leer la hojas iterando de a 5 int
            cout<<'\n';
            ct = 0;
        }
        cout<<x<<" ";
        ct++;
    }
    cout<<endl;
    */
    for (int x : nll)
    {
        cout << x << endl;
        cout << "Padres: ";
        ct = m * 5;
        for (int x : n1)
        {
            if (ct == m * 5)
            { // es posible ver el resto de los nodos iterando de 5*m int
                cout << '\n';
                ct = 0;
            }
            cout << x << " ";
            ct++;
        }
        ct = m * 5;
        cout << endl;
        cout << "Abuelos: ";
        for (int x : n2)
        {
            if (ct == m * 5)
            {
                cout << '\n';
                ct = 0;
            }
            cout << x << " ";
            ct++;
        }
        ct = m * 5;
        cout << endl;
        cout << "Raíz: ";
        for (int x : n3)
        {
            if (ct == m * 5)
            {
                cout << '\n';
                ct = 0;
            }
            cout << x << " ";
            ct++;
        }
        cout << endl;
    }
    return 0;
}
