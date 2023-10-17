#include <iostream>
#include <cstdlib> //rand()
#include "NodoRTree.cpp"
#include "NearestXRtree.cpp"

/*
Consultas: la ruta a el archivo con las consultas
Rtree: ruta al Rtree construido
*/
vector<pair<int,int>> consultaQ(const char* Consultas, const char* Rtree, int m){
    //vector para guradar el resultado. 
    //primer valor guarda la cantidad de accesos a disco de la consulta
    //segundo valor guarda el tiempo que tomó la ejecución 
    vector<pair<int,int>>result(1,make_pair(0,0));

    FILE *arch=fopen(Consultas,"rb");   //se abre el archivo de consultas 
    int blk_size=(sizeof(int));         //tamaño de entero
    int size = sizeof(*arch);           //tamaño del archivo
    //se carga el archivo con las consultas en el siguiente vector
    int Q[size];
    fread(Q,blk_size,2,arch);  
    vector<Rectangulo> Q_rects(1,Rectangulo(Punto(0,0),Punto(0,0)));
    //se generan los rectángulos
    Q_rects[0] = Rectangulo(Punto(Q[0],Q[1]),Punto(Q[2],Q[3]));
    for (int p = 1; p<size/4; p++){
        Q_rects.push_back(Rectangulo(Punto(Q[p*4],Q[p*4 + 1]),Punto(Q[p*4 +2],Q[p*4 +3])));
    } 

    //se buscan realiza la consulta
    for (Rectangulo consulta : Q_rects ){

    }
    return result;
}
