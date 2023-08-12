#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ARCHI "votos.bin"

typedef struct
{
    char provincia[30];
    char partido[30]; ///"FdT" // "FIT" // "UCR" // "JxC"
    int cant_votos;
} RecuentoVotos;

typedef struct nodoPartido
{
  char partido[30];
  int cant_votos;
  struct nodoPartido* sig;
}nodoPartido;

typedef struct nodoProvincia
{
  char provincia[30];
  nodoPartido* partido;
  struct nodoProvincia* sig;
}nodoProvincia;

typedef struct
{
     char provincia[30];
     int votos_Totales;
}VotosProvincia;

typedef struct
{
    VotosProvincia dato;
    struct nodoDoble* sig;
    struct nodoDoble* ant;
}nodoDoble;


nodoProvincia* inicListaProv(){
    return NULL;
}

nodoPartido* inicListaPart(){
    return NULL;
}

nodoProvincia* crearNodoProvincia(RecuentoVotos registro){

    nodoProvincia* nuevo = (nodoProvincia*)(malloc(sizeof(nodoProvincia)));
    strcpy(nuevo->provincia, registro.provincia);
    nuevo->partido = inicListaPart();
    nuevo->sig = NULL;

    return nuevo;

}

nodoPartido* crearNodoPartido(RecuentoVotos reg){

    nodoPartido* nuevo = (nodoPartido*)(malloc(sizeof(nodoPartido)));
    strcpy(nuevo->partido,reg.partido);
    nuevo->cant_votos = reg.cant_votos;
    nuevo->sig = NULL;
    return nuevo;

}


nodoProvincia* buscarProvincia(nodoProvincia* lista, char provincia[30]){

    nodoProvincia* encontrado;
    nodoProvincia* seg = lista;
    while (seg != NULL && strcmpi(seg->provincia,provincia)!=0){
        seg = seg->sig;
    }
    if (seg != NULL){
        encontrado = seg;
    }else{
        encontrado = NULL;
    }
    return encontrado;

}
nodoProvincia* buscarUltimoNodo(nodoProvincia* lista){

    nodoProvincia* ultimo = lista;
    while(ultimo->sig != NULL){
        ultimo = ultimo->sig;
    }
    return ultimo;
}

nodoProvincia* insertarAlFinal(nodoProvincia* lista, nodoProvincia* nuevo){
    if(lista == NULL){
        lista=nuevo;
    }else{
        nodoProvincia* ultimo = buscarUltimoNodo(lista);
        ultimo->sig = nuevo;
    }
    return lista;
}


/*nodoProvincia* insertarOrdXNombre(nodoProvincia* lista, nodoProvincia* nuevo){
    nodoProvincia* seg;
    nodoProvincia* ant;
    if (lista == NULL){
        lista = nuevo;
    }else{

        if (strcmpi(nuevo->provincia,lista->provincia)<0){
            nuevo->sig = lista;
            lista = nuevo;
        }else{

            seg = lista;
            while (seg != NULL && strcmpi(nuevo->provincia,seg->provincia)>0){

                ant = seg;
                seg = seg->sig;
            }
            ant->sig = nuevo;
            if (seg != NULL){
                nuevo->sig = seg;
            }
        }

    }

    return lista;

}
*/

/*nodoPartido* insertarAlFinal(nodoPartido* lista, nodoPartido* nuevo){

    nodoPartido* ant;
    nodoPartido* seg;
    if (lista == NULL){
        lista = nuevo;
    }else{
        seg = lista;
        while (seg != NULL){
            ant = seg;
            seg = seg->sig;
        }
        ant->sig = nuevo;
    }
    return lista;

}*/
nodoPartido* insertarOrd(nodoPartido* lista, nodoPartido* nuevo){
    if (lista == NULL){
        lista=nuevo;
    }else{
        if(strcmpi(nuevo->partido, lista->partido)<0){
            nuevo->sig = lista;
            lista = nuevo;
        }else{
            nodoPartido* ant = lista;
            nodoPartido* seg = lista->sig;
            while(seg != NULL && strcmpi(nuevo->partido, seg->partido)>0){
                ant = seg;
                seg = seg->sig;
            }
            if(seg == NULL){
                ant->sig = nuevo;
            }else{
                nuevo->sig = seg;
                ant->sig = nuevo;
            }
        }
    }
    return lista;

}
nodoProvincia* insertarOrdxNombre(nodoProvincia* lista, nodoProvincia* nuevo){
    if (lista == NULL){
        lista=nuevo;
    }else{
        if(strcmpi(nuevo->provincia, lista->provincia)<0){
            nuevo->sig = lista;
            lista = nuevo;
        }else{
            nodoProvincia* ant = lista;
            nodoProvincia* seg = lista->sig;
            while(seg != NULL && strcmpi(nuevo->provincia, seg->provincia)>0){
                ant = seg;
                seg = seg->sig;
            }
            if(seg == NULL){
                ant->sig = nuevo;
            }else{
                nuevo->sig = seg;
                ant->sig = nuevo;
            }
        }
    }
    return lista;

}


nodoProvincia* insertarProvincia(nodoProvincia* lista, RecuentoVotos reg){
    nodoProvincia* nuevo;
    nodoProvincia* pos = buscarProvincia(lista, reg.provincia);


    nodoPartido* nuevoP = crearNodoPartido(reg);
    if (pos == NULL){
        nuevo = crearNodoProvincia(reg);
        nuevo->partido = insertarOrd(nuevo->partido, nuevoP);
        lista = insertarOrdxNombre(lista, nuevo);
    }else{
        pos->partido = insertarOrd(pos->partido, nuevoP);
    }
    return lista;
}

nodoProvincia* archivoALista(nodoProvincia* lista){
    RecuentoVotos reg;
    nodoProvincia* pos;
    FILE* fp;
    fp = fopen(ARCHI,"rb");
    if (fp != NULL){
        while (fread(&reg, sizeof(RecuentoVotos), 1, fp)>0){
            lista = insertarProvincia(lista, reg);

        }
        fclose(fp);
      }
    return lista;

}

///Hacer una función que reciba la lista de provincias y pase a una lista doblemente enlazada votosXProvincia
///la cantidad de votos TOTALES por provincia almacenado en un registro de tipo VotosProvincia. Esta función debe ser recursiva.

int votosXProv(nodoPartido* lista){
    int suma = 0;
    if (lista != NULL){
        suma = lista->cant_votos + votosXProv(lista->sig);
    }
    return suma;

}

nodoDoble* inicListaDoble(){

    return NULL;

}

nodoDoble* crearNodoD(int votos, char prov[30]){

    nodoDoble* nuevo = (nodoDoble*)(malloc(sizeof(nodoDoble)));
    strcpy(nuevo->dato.provincia,prov);
    nuevo->dato.votos_Totales = votos;
    nuevo->ant = NULL;
    nuevo->sig = NULL;
    return nuevo;

}
nodoDoble* insertarAlfinalDoble(nodoDoble* lista, nodoDoble* nuevo){
    nodoDoble* seg;
    nodoDoble* ante;
    if (lista == NULL){
        lista = nuevo;
    }else{
        seg = lista;
        while (seg != NULL){
            ante = seg;
            seg = seg->sig;
        }
        ante->sig = nuevo;
        nuevo->ant = ante;
    }
    return lista;

}

nodoDoble* crearListaDoble(nodoDoble* listaD,nodoProvincia* lista){
    int total;
    nodoDoble* nuevo;
    nodoProvincia* seg = lista;
    while (seg != NULL){
        total = votosXProv(seg->partido);
        nuevo = crearNodoD(total, seg->provincia);
        listaD = insertarAlfinalDoble(listaD, nuevo);
        seg = seg->sig;
    }
    return listaD;



}
void mostrarPartido(nodoPartido* partidos){

    while (partidos != NULL){
        printf("Partido: %s\n", partidos->partido);
        printf("Cant de votos: %d\n",partidos->cant_votos);
        partidos = partidos->sig;
    }
}


void mostrarProvincia(nodoProvincia* provincias){

    while (provincias != NULL){
        int total = votosXProv(provincias->partido);
        printf("-------------------------------------------\n");
        printf("Provincia: %s\n",provincias->provincia);
        mostrarPartido(provincias->partido);
        printf("Total: %d\n",total);
        printf("-------------------------------------------\n");
        system("pause");
        provincias = provincias->sig;
    }


}

void imprimirTotales(nodoDoble* lista){
    while(lista != NULL){
        printf("----------------------------------\n");
        printf("Provincia: %s\n",lista->dato.provincia);
        printf("Total: %d\n", lista->dato.votos_Totales);
        printf("----------------------------------\n");

        lista = lista->sig;
    }

}

///Hacer una función que elimine una determinada provincia de la lista, debe llegar su nombre por parámetro.
nodoDoble* eliminarProvincia(nodoDoble* lista, char provincia[]){
    nodoDoble* seg;
    nodoDoble* ant;
    if (lista != NULL){
        if (strcmpi(lista->dato.provincia,provincia)==0){
            lista = lista->sig;
        }else{
            seg = lista;
            while (seg != NULL && strcmpi(seg->dato.provincia,provincia) != 0){
                ant = seg;
                seg = seg->sig;
            }

            ant->sig = seg->sig;
            if (seg->sig != NULL){
                seg = seg->sig;
                seg->ant = ant;
            }
        }
    }
    return lista;


}




int main()
{
    nodoProvincia* lista = inicListaProv();
    nodoDoble* listaD = inicListaDoble();
    char prov[30];
    lista = archivoALista(lista);
    mostrarProvincia(lista);
    listaD = crearListaDoble(listaD, lista);
    imprimirTotales(listaD);
    printf("Provincia a eliminar: ");
    gets(prov);
    listaD = eliminarProvincia(listaD, prov);
    imprimirTotales(listaD);

    return 0;
}
