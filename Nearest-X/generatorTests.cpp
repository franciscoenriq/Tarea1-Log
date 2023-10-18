#include "NearestXRtree.cpp"
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
                    1,1,3,2,
                    5,5,6,6,
                    9,10,11,11,
                    3,1,4,3,
                    8,6,9,9,
                    1,2,2,4,
                    7,7,8,9,
                    7,8,9,10,
                    10,10,13,13,
                    3,4,5,6};
    vector <Rectangulo> r =  makeRectArray(ptr, 4*N);
    for (int i = 0; i<N; i++){

        cout << "Rectangulo " << i + 1 << '\n'
             << "p1: " << r[i].p1 << " p2: " << r[i].p2 << endl;
    }
    // prueba de xquicksort
    xquickSort(r, 0, N - 1);
    for (int i = 0; i < N; i++)
    {
        cout << "centro: " << r[i].centro.x << " " << r[i].p1 << ' ' << r[i].p2 << endl;
    }
    //prueba de calcular mbr
    Rectangulo mbr = calcularMBR(r,0,N);
        cout<<"MBR: "<<mbr.p1<<','<<mbr.p2<<endl;
    //prueba de generación del binario

    vector<ull> puntosRtree = vectorRTree(r,m,ordenNearestX);
    grabarVector(puntosRtree,"xRTree.bin");
    // se lee el archivo completo en cadena
    char *file2 = "xRTree.bin";
    leerBinFile(file2);
/*
    //prueba de lectura del binario
    vector <ull> datos;
    ull entero;
    FILE *arch = fopen("xRtree.bin","rb");
    ifstream f("xRtree.bin", ios::binary);
        fread( &entero,sizeof(entero),1,arch);
        while (entero != NULL){
            fread(entero,sizeof(unsigned long long int),1,arch);
            datos.push_back(entero);
        }
    fclose(arch);  // se cierra el archivo

    // impresión de la información del binario
    int l = datos.size();
    cout << "tamaño archivo: "<< l << endl;
    int ct = 0;
    int i = (5*m*N);
    cout<<'\n'<<"N1: ";
    for (ull x = 2; x< datos.size(); x++){
        if (ct&5 == 0){
            cout<<" N"<<ct/5*m<<": ";
        }
        if(ct == i){
            cout<<'\n'<<"Nuevo bloque de nodos"<<endl;
            i = i/m;
        }
        cout<<datos[x]<<' ';
   }
    cout << endl;*/
 return 0;
}
