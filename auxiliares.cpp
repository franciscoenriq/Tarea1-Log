#include <iostream>
#include <cstdlib> //rand()
#include "NodoRTree.cpp"

using namespace std;

int randomNum(int minimo = 0, int maximo = 500000)
{
    // Genera un número aleatorio entre minimo y (maximo - 1)
    int numeroAleatorio = std::rand() % (maximo - minimo) + minimo;
    return numeroAleatorio;
}

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
Rectangulo calcularMBR(vector<Rectangulo> &listaRectangulos, int start, int end)
{
    int lsize = listaRectangulos.size();
    if (listaRectangulos.empty())
    {
        return Rectangulo{{0, 0}, {0, 0}};
    }

    if (lsize == 1)
    {
        return listaRectangulos[0];
    }

    int minX = listaRectangulos[start].inf_izq.x;
    int maxX = listaRectangulos[start].sup_der.x;
    int minY = listaRectangulos[start].inf_izq.y;
    int maxY = listaRectangulos[start].sup_der.y;
    if (lsize < end)
        end = lsize;
    for (int i = start; i < end; i++)
    {
        const Rectangulo &rect = listaRectangulos[i];
        minX = min(minX, (int)rect.inf_izq.x);
        maxX = max(maxX, (int)rect.sup_der.x);
        minY = min(minY, (int)rect.inf_izq.y);
        maxY = max(maxY, (int)rect.sup_der.y);
    }

    // retornamos el MBR calculado como un nuevo rectángulo
    return Rectangulo(Punto(minX, minY), Punto(maxX, maxY));
}


/*Función auxiliar para grabar un vector en memoria */
void grabarVector(vector<int> vect, const char* fileName){
     FILE* arch;
    arch = fopen(fileName, "wt");
    if (arch) {
        for (int data : vect) {
            fwrite(&data, sizeof(int), 1, arch);
        }
        fclose(arch);
        std::cout << "Archivo '" << fileName << "' creado y datos escritos con éxito." << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo '" << fileName << "'." << std::endl;
    }
}

/*
Función que  genera un array que linealmente representa a un  RTree
La estructura final dispone de los datos en nodos de la siguiente forma en memoria

0 1    k+1  claves (rects)    5k+1 otro nodo...
|-|-----|-----|-----|-----|-----|
|_|hijos|_______________________|.......
                    
---cada rectángulo contiene 4 enteros + 0: 5*R enteros
---cada nodo contiene 4k + k enteros; 
            4k para rectangulos  MBR
            k  para punteros (offset) a hijos.
--- se tienen R/m nodos en total
*/
vector<int> vectorRTree(vector <Rectangulo> &r_vect, int m){
    // suponemos que los rectangulos vienen ordenados

    //vector que contendrá los datos a insertar en la interación actual
    vector <int> arr(1,0);
    // se calcula la cantidad de rectángulos totales
    int R_totales = r_vect.size();
    cout<<"Size of vector: "<< R_totales <<endl;
    //Se ordenan los rectángulos del vector de entrada.Notar que esta función puede ser reemplazada por otras 
    

    // PRIMERA ITERACIÓN
    //se calcula la cantidad de mbrs a calcular
    //Notar que esto también corresponde a cuantos nodos se crearán en esta iteración
    int n_mbr = R_totales/m;
    if (R_totales%2 != 0 && R_totales != 1) n_mbr++;     //si existe algun resto, se debe calcular un mbr más
    cout<<"Starting..."<<"MBRs to calculate: "<< n_mbr<<endl;

    // se calculan los mbrs
    vector <Rectangulo> mbrs(n_mbr,Rectangulo(Punto(0,0),Punto(0,0)));
    // Se recorre el vector de rectángulos para generar los MBR
    for (int rect = 0; rect < n_mbr; rect++){  
        mbrs[rect] = calcularMBR(r_vect,rect*m,(rect+1)*m); 
    }
    //DEBUG
    for (Rectangulo x :mbrs){
        cout<<"MBR: "<<x.inf_izq.x<<','<<x.inf_izq.y<<';'<<x.sup_der.x<<','<<x.sup_der.y<<endl;
    }

    // algoritmo de construcción
    int offset = 1 + m;   // se establece la posición del primer nodo hijo
    //Se van insertando los nodos padre de a m datos
    for (int padres = 0; padres < n_mbr; padres++){
        int ds = m*padres;              //desplazamiento de los padres por los mbrs  
        for (int k = 0; k<m; k++){      //primero los "punteros" a hijos
            if (k >= R_totales) arr.push_back(0);//Hay menos de k hijos reales para el nodo
            else arr.push_back(offset);
            offset +=5;                 //se avanza a la primera posición de la siguiente hoja
        }
        for (int k = 0; k<m; k++){      //luego las claves (rectángulos de hijos)
            if (k+ds >= R_totales){       //Hay menos de k mbr reales para el nodo
                arr.push_back(0);
                arr.push_back(0);
                arr.push_back(0);
                arr.push_back(0);
            } 
            else {
                arr.push_back(r_vect[k+ds].inf_izq.x);
                arr.push_back(r_vect[k+ds].inf_izq.y);
                arr.push_back(r_vect[k+ds].sup_der.x);
                arr.push_back(r_vect[k+ds].sup_der.y);
            }
        }
    }
    offset = 1; //se retrocede el offset al primer nodo hijo
    // si la cantidad de mbrs fué mayor a la cantidad de datos guardables en un nodo,
    // se repite una variación del algoritmo anterior.
    if (n_mbr > m){
        int last = 0; //marcador que indica si es la última iteración
        while (true){
            for (int padres = 0; padres < n_mbr; padres++){
                int ds = m*padres;                  //desplazamiento de los padres por los mbrs  
                if (ds >=n_mbr)                      //si el desplazamiento es mayor al tamaño del vector de rectángulos
                    ds=0;                           // no se usa
                //inserción de punteros a hijos
                for (int k = 0; k<m; k++){          
                    if (k>=R_totales) 
                        arr.push_back(0);        //Hay menos de k hijos reales para el nodo
                    else 
                        arr.push_back(offset);
                    offset +=5*m;                 //se avanza a la primera posición de la siguiente hoja
                }
                //linserción de claves (rectángulos de hijos)
                for (int k = 0; k < m; k++){            
                    if (k+ds >= R_totales){           //Hay menos de k mbr reales para el nodo
                        arr.push_back(0);
                        arr.push_back(0);
                        arr.push_back(0);
                        arr.push_back(0);
                    } 
                    else { 
                        arr.push_back(mbrs[k+ds].inf_izq.x);
                        arr.push_back(mbrs[k+ds].inf_izq.y);
                        arr.push_back(mbrs[k+ds].sup_der.x);
                        arr.push_back(mbrs[k+ds].sup_der.y);
                    }
                 }
            }

            if (last)       //si la siguiente es la última iteración, se rompe el bucle
                break;
            //se re-calculan los MBR
            int br = n_mbr;
            n_mbr = n_mbr/m;
            // Se guarda cuantos elemntos será necesario quitar del vector;
            br = br-n_mbr;
            // Se recorre el vector de mbrs para generar otros MBR
            if (n_mbr>0){
                cout<<"MBRs to calculate: "<< n_mbr<<endl;
                for (int rect = 0; rect < n_mbr; rect++){  
                    mbrs[rect] = calcularMBR(mbrs,rect*m,(rect+1)*m); 
                }
                // se quitan los espacios innecesarios de mbrs
                for (int i = 0; i<br; i++){
                    mbrs.pop_back();
                }
                //DEBUG
                for (Rectangulo x :mbrs){
                    cout<<"MBR*: "<<x.inf_izq.x<<','<<x.inf_izq.y<<';'<<x.sup_der.x<<','<<x.sup_der.y<<endl;
                }
            }
            if (n_mbr < m) 
                last = 1;   //se marca como ultima la siguiente iteración
            n_mbr = n_mbr/m;
            if (n_mbr != 1 && n_mbr%2 != 0) n_mbr++;
            
        }
    }
    // Se retorna el vector creado
    return arr;
}
