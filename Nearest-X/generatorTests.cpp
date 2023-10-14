#include "NearestXRtree.cpp"
using namespace std;

//se definen los rangos en x e y para generar una nube de puntos
#define xrange  100
#define yrange  100


uint32_t randomNum(int minimo = 0, int maximo = 500000){
    // Genera un número aleatorio entre minimo y (maximo - 1)
    uint32_t numeroAleatorio = rand() % (maximo - minimo) + minimo;
    return numeroAleatorio;
}

int main(){
    int m = 5;
    //prueba de crear un arreglo de rectángulos en base a un arreglo de puntos
    int N=7;
      d_int ptr[]={9,8,10,9,
                    1,1,3,2,
                    5,5,6,6,
                    9,10,10,11,
                    3,1,4,3,
                    8,6,9,9,
                    3,4,5,6};
    vector <Rectangulo> r =  makeRectArray(ptr,N*4);
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
    nX_RTree(r,2);
    //prueba de lectura del binario

    FILE *arch=fopen("xRTree","rb");
    int blk_size=(sizeof(int));
    int rect[5*N];
    int nll[1];
    fread(nll,sizeof(int),1,arch); //se lee el nulo
    fread(rect,sizeof(int),5*N,arch); //se leen rectángulos
    int n1[5*N];
    fread(n1,sizeof(int),5*N,arch); //se leen los nodos padres
    int n2[5*N];
    fread(n2,sizeof(int),N*5,arch); //se leen los nodos abuelos
    fclose(arch);
    //impresión de la información del binario
    cout<<"Hojas: ";
    int ct = 0;
    for (int x: rect){
        if (ct == m){
            cout<<"|| ";
            ct = 0;
        }
        cout<<x<<" ";
        ct++;
    }
    cout<<endl;
    cout<<"Padres: ";
   
    for (int x: n1){
        cout<<x<<" "; 
    }
    cout<<endl;
    cout<<"Abuelos: ";
    for (int x: n2){
        cout<<x<<" ";
    }
    cout<<endl;
    return 0;
}
