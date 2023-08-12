#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
   char animal[30];
   int cant;
   int habitat;
   int idEspecie;
   char especie[20];
} registroArchivo;
typedef struct {
      char nombreAnimal [30];
	int cantidad;
	int habitat; /// 1 - selva, 2- savana, 3-bosque, 4-mar
} animal;
typedef struct
{
   animal dato;
   struct nodoArbol * izq;
   struct nodoArbol * der;
} nodoArbol;
typedef struct
{
   int idEspecie;
   char especie [20];/// "Mamiferos", "Aves", "Reptiles", "Peces", ”Anfibios”
   nodoArbol * arbolDeAnimales;
} celdaEspecie;

int buscarEspecie(celdaEspecie a[],  int validos, int idEspecie){
    int encontrado;
    int i=0;
    while (i<validos && a[i].idEspecie != idEspecie){
        i++;
    }
    if (i<validos){
        encontrado=i;
    }else{
        encontrado=-1;
    }

    return encontrado;
}
nodoArbol* inicArbol(){
    return NULL;
}

nodoArbol* crearNodo(registroArchivo dato){
    nodoArbol* nuevo = (nodoArbol*) malloc(sizeof(nodoArbol));
    strcpy(nuevo->dato.nombreAnimal, dato.animal);
    nuevo->dato.cantidad=dato.cant;
    nuevo->dato.habitat=dato.habitat;
    nuevo->der = NULL;
    nuevo->izq = NULL;
    return nuevo;
}

nodoArbol* insertarEnArbol(nodoArbol* arbol, nodoArbol* nuevo){
    if (arbol == NULL){
        arbol= nuevo;
    }else{
        if (strcmpi(nuevo->dato.nombreAnimal,arbol->dato.nombreAnimal)<0){
            arbol->izq=insertarEnArbol(arbol->izq, nuevo);
        }else{
            arbol->der=insertarEnArbol(arbol->der, nuevo);
        }

    }
    return arbol;

}

void imprimirRegArch(registroArchivo reg){
    printf("Nombre: %s\n", reg.animal);
    printf("Cantidad: %i\n", reg.cant);
    printf("Habitat: %i\n", reg.habitat);




}
void imprimirReg(animal dato){

    printf("Nombre: %s\n", dato.nombreAnimal);
    printf("Cantidad: %i\n", dato.cantidad);
    printf("Habitat: %i\n", dato.habitat);

}

void imprimirArbol(nodoArbol* arbol){

    if (arbol != NULL){
        imprimirArbol(arbol->izq);
        imprimirReg(arbol->dato);
        imprimirArbol(arbol->der);
    }
}

void imprimirArreglo(celdaEspecie a[], int validos){
    int i=0;
    while (i<validos){
        printf("Id de Especie: %i\n", a[i].idEspecie);
        printf("Especie: %s \n", a[i].especie);
        printf("----------------------------------------\n");
        imprimirArbol(a[i].arbolDeAnimales);
        printf("----------------------------------------\n");
        i++;
    }


}

int insertarEnArreglo(celdaEspecie a[], int validos, registroArchivo reg){


    nodoArbol* nuevo = crearNodo(reg);
    int pos = buscarEspecie(a, validos, reg.idEspecie);
    if (pos == -1){
        a[validos].idEspecie=reg.idEspecie;
        strcpy(a[validos].especie, reg.especie);
        a[validos].arbolDeAnimales= inicArbol();
        a[validos].arbolDeAnimales= insertarEnArbol(a[validos].arbolDeAnimales, nuevo);
        validos++;
    }else{
        a[pos].arbolDeAnimales = insertarEnArbol(a[pos].arbolDeAnimales, nuevo);
    }
    return validos;

}
int pasarDeArchivoToADA(celdaEspecie a[]){
    FILE* fp;
    int i=0;
    registroArchivo reg;
    fp = fopen("animales.dat", "rb");
    if (fp != NULL){
        while (fread(&reg, sizeof(registroArchivo), 1, fp)>0){

            i = insertarEnArreglo(a, i, reg);
        }
        fclose(fp);
    }
    return i;
}
registroArchivo copiarAnimales(nodoArbol* arbol, int id, char especie[20]){

    registroArchivo reg;
    reg.idEspecie=id;
    strcpy(reg.especie, especie);
    reg.cant=arbol->dato.cantidad;
    strcpy(reg.animal, arbol->dato.nombreAnimal);
    reg.habitat=arbol->dato.habitat;
    return reg;


}
void pasarAArchivo(celdaEspecie a){
    FILE* fp;

    char nombre[30];

    strcpy(nombre, a.especie);
    strcat(nombre,".dat");

    registroArchivo reg;
    fp = fopen(nombre, "wb");
    if (a.arbolDeAnimales != NULL){
        reg= copiarAnimales(a.arbolDeAnimales, a.idEspecie, a.especie);
        fwrite(&reg, sizeof(registroArchivo), 1, fp);
    }
    fclose(fp);


}
void pasarDeADAToArchivosAnimales(celdaEspecie a[], int validos){

    int i=0;
    while (i<validos){
        pasarAArchivo(a[i]);
        i++;
    }




}
int main()
{
    celdaEspecie a[100];
    int validos;
    validos = pasarDeArchivoToADA(a);
    imprimirArreglo(a, validos);
    pasarDeADAToArchivosAnimales(a, validos);

    return 0;
}
