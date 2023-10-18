 #include "../auxiliares.cpp"
 /*
Método de ordenamiento para Nearest-X basado en un algoritmo quicksort.
Ordenar un vector rectángulos de menor a mayor respecto a la abscisa*/
void xquickSort(vector <Rectangulo> &arr, int start, int end){

    //caso base
    if (start >= end)
        return;

    // se particiona el arreglo
    d_int pivot = arr[start].centro.x;
    d_int count = 0;
    for (int i = start + 1; i <= end; i++) {
        if (arr[i].centro.x <= pivot)
            count++;
    }
    // asignando una posición al pivote
    int pivotIndex = start + count;


    swap(arr[pivotIndex], arr[start]);


    // ordenando particiones izquierdas y derechas del pivote
    int i = start, j = end;

    while (i < pivotIndex && j > pivotIndex) {

        while (arr[i].centro.x <= pivot) {
            i++;
        }

        while (arr[j].centro.x > pivot) {
            j--;
        }

        if (i < pivotIndex && j > pivotIndex) {
            swap(arr[i++], arr[j--]);
        }
    }
    // se ordena la mitad izquierda
    xquickSort(arr, start, pivotIndex - 1);
    // Sorting the right part
    xquickSort(arr, pivotIndex + 1, end);
}