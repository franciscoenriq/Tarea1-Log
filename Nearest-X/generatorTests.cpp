#include "NearestXRtree.cpp"
using namespace std;



//se definen los rangos en x e y para generar una nube de puntos
#define xrange  100
#define yrange  100



int main(){
    int m = 3;
    //prueba de crear un arreglo de rectángulos en base a un arreglo de puntos
    int N=11;
      int ptr[]={9,8,10,9,
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

        cout << "Rectangulo "<<i+1 << '\n'
        << "p1: "<<r[i].p1 <<" p2: "<< r[i].p2 << endl;
    }
    //prueba de xquicksort
    xquickSort(r,0,N-1);
    for (int i = 0;i<N;i++){
        cout<<"centro: "<<r[i].centro.x <<" " << r[i].p1<<' '<<r[i].p2 <<endl;
    }
    //prueba de calcular mbr
    Rectangulo mbr = calcularMBR(r,0,N);
        cout<<"MBR: "<<mbr.p1<<','<<mbr.p2<<endl;
    //prueba de generación del binario
    vector<int> puntosRtree = nX_RTree(r,m);
    grabarVector(puntosRtree,"xRTree");

    //prueba de lectura del binario
    FILE *arch=fopen("xRTree","rb");
    int blk_size=(sizeof(int));
    //int rect[5*N];
    int nll[2];
    fread(nll,sizeof(int),2,arch);          //se lee el nulo
    //fread(rect,sizeof(int),5*N,arch);      //se leen rectángulos

    //Notar que de esta parte en adelante, es posible recorrer o rescatar
    //los valores del binario usando un ciclo while e indicando offsets múltiplos
    // de 5*m
    int size = N/m;                        // tamaño del siguiente sector de nodos
    if (N%m != 0 && size != 1) size++;
    int n1[5*m*size];             // tamaño (rectangulo +puntero)*capacidad nodo * cantidad mbrs
    fread(n1,sizeof(int),5*size*m,arch);   //se leen los nodos padres
    int tmp=size;
    size = size/m;
    if (tmp%m != 0 && tmp != 1) size++;
    int n2[5*m*size];
    fread(n2,sizeof(int),5*m*size,arch);    //se leen los nodos abuelos
    tmp=size;
    size = size/m;
    if (tmp%m != 0 && tmp != 1) size++;
    int n3[5*m*size];
    fread(n3,sizeof(int),5*m*size,arch);   //se lee la raíz
    fclose(arch);                          //se cierra el archivo

    //impresión de la información del binario
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
   for (const auto x : nll)
    cout<<x<<endl;
    cout<<"Padres: ";
    ct = m*5;
    for (const auto x: n1){
        if (ct == m*5){ //es posible ver el resto de los nodos iterando de 5*m int
            cout<<'\n';
            ct = 0;
        }
        cout<<x<<" ";
        ct++;
    }
    ct = m*5;
    cout<<endl;
    cout<<"Abuelos: ";
    for (const auto x: n2){
        if (ct == m*5){
            cout<<'\n';
            ct = 0;
        }
        cout<<x<<" ";
        ct++;
    }
    ct = m*5;
    cout<<endl;
    cout<<"Raíz: ";
    for (const auto x: n3){
        if (ct == m*5){
            cout<<'\n';
            ct = 0;
        }
        cout<<x<<" ";
        ct++;
    }
    cout<<endl;
    //se lee el archivo completo en cadena
    arch=fopen("xRTree","rb");
    int stream[(5*m*N)+1];
    int l = sizeof(*arch);
    fread(stream,sizeof(int),5*m*N+1,arch);   //se lee la raíz
    fclose(arch);
    for (const auto x : stream){
        cout<<x<<' ';
    }
    cout<<"tamaño archivo: "<<l<<endl;
    return 0;
}
