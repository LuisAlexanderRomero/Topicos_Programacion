/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres:Romero Luis Alexander
*
* DNI:40228032
*
****************************************************************************************
* Corrección

****************************************************************************************
* Nota:
***************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "../Vector/Vector.h"
#include "../SolucionParcialMueblesTopicos/SolucionMuebles.h"

#include <string.h>
#define TAM_LIN 101
#define ERROR_BUSQUEDA 1
#define ERROR_MEMORIA 2
#define NO_ENCONTRADO -1


void actualizarDepositos_ALU(const char* archTransferencias, const char* archDepositos);
int actulizarDepositoStock(FILE* archDep,Vector* vecIdx,TransferenciaElem* tranElem, Cmp cmp);
void cargarIndice(FILE* archivoIdx, Vector* vecIdx);
void desencriptarTransferencias_ALU(const char* nombreArchivo);
void invertirCad_ALU(char* cad,int cant);
int vectorCargarDeArchivo_ALU(const char* nombreArch, Vector* vector, size_t tamElem);
int vectorOrdBuscar_ALU(const Vector* vec, void* elem, Cmp cmp);
int vectorInsertarAlFinal_ALU(Vector* vector, const void* elem);
int vectorCrear_ALU(Vector* vec, size_t tamElem);
void vectorDestruir_ALU(Vector* vec);

int cmpInd_ALU(const void* pvInd1, const void* pvInd2);

int main()
{
    generarArchivos(true, true);

    mostrarArchivoTransferencias("Transferencias.txt");

    puts("");
    puts("Depositos antes de actualizar:");
    mostrarArchivoDepositos("Depositos.dat");

    actualizarDepositos("Transferencias.txt", "Depositos.dat");
    //actualizarDepositos_ALU("Transferencias.txt", "Depositos.dat");

    puts("");
    puts("Depositos despues de actualizar:");
    mostrarArchivoDepositos("Depositos.dat");

    return 0;
}

void actualizarDepositos_ALU(const char* archTransferencias, const char* archDepositos)
{
    //descencriptar transferencias
    desencriptarTransferencias_ALU(archTransferencias);

    puts("");
    puts("Transferencias desencriptadas:");
    mostrarArchivoTransferencias(archTransferencias);
    //genera el mismo txt pero descencriptado

    FILE* archTrasf = fopen(archTransferencias,"rt");
    if(!archTrasf)
    {
        printf("Error al abrir %s",archTransferencias);
        return;
    }
    FILE* archDep = fopen(archDepositos,"r+b");
    if(!archDep)
    {
        printf("Error al abrir %s",archDepositos);
        fclose(archTrasf);
        return;
    }
    //ya tengo el archivo indice generado???
    //cargo el indice
    Vector vecIdx;
    vectorCrear_ALU(&vecIdx,sizeof(Indice));
    vectorCargarDeArchivo_ALU("Depositos.idx",&vecIdx,sizeof(Indice));

    //leo y guardo en un reg la transferencia
    Transferencia transf;
    char linea[TAM_LIN];
    char* actual;

    TransferenciaElem tranOrig;
    TransferenciaElem tranDest;

    while(fgets(linea,TAM_LIN,archTrasf))
    {
        //leo en la estructura transferencia
        actual= strchr(linea,'\n');
        *actual = '\0';

        actual = strrchr(linea,'|');
        sscanf(actual+1,"%d",&transf.cantidad);
        *actual = '\0';

        actual = strrchr(linea,'|');
        strncpy(transf.codProd,actual+1,15);
        transf.codProd[15]='\0';
        *actual = '\0';

        actual = strrchr(linea,'|');
        sscanf(actual+1,"%d",&transf.depDestino);
        *actual = '\0';

        sscanf(linea,"%d",&transf.depOrigen);

        //paso de transferencia a tranferenciaElem
        tranOrig.nroDep = transf.depOrigen;
        strcpy(tranOrig.codProd,transf.codProd);
        tranOrig.cantidad = -transf.cantidad;

        actulizarDepositoStock(archDep,&vecIdx,&tranOrig,cmpInd_ALU);

        tranDest.nroDep = transf.depDestino;
        strcpy(tranDest.codProd,transf.codProd);
        tranDest.cantidad = transf.cantidad;

        actulizarDepositoStock(archDep,&vecIdx,&tranDest,cmpInd_ALU);

    }

    vectorDestruir_ALU(&vecIdx);
    fclose(archDep);
    fclose(archTrasf);
}
void cargarIndice(FILE* archivoIdx, Vector* vecIdx)
{
    Indice elem;
    while (fread(&elem, sizeof(Indice), 1, archivoIdx))
    {
        vectorInsertarAlFinal(vecIdx, &elem);
    }
}
int actulizarDepositoStock(FILE* archDep,Vector* vecIdx,TransferenciaElem* tranElem, Cmp cmp)
{
    //cargo indice de busqueda
    Indice indBusq;
    indBusq.nroDep=tranElem->nroDep;
    strncpy(indBusq.codProd,tranElem->codProd,15);
    indBusq.codProd[15]='\0';
    indBusq.nroReg=-1;

    Deposito regDep;//inicializar registro

    return strcmp(ind1->codProd,ind2->codProd);
    if(vectorOrdBuscar_ALU(vecIdx,&indBusq,cmp) == NO_ENCONTRADO)
    {
        printf("error en busqueda");
        return ERROR_BUSQUEDA;
    }

    //me muevo en el archivo
    fseek(archDep,indBusq.nroReg*sizeof(Deposito),SEEK_SET);
    fread(&regDep,sizeof(Deposito),1,archDep);//movi un elemento

    //actualizo stock
    regDep.stock += tranElem->cantidad; //si es neg. lo resta

    //vuelvo a la posicion anterior
    fseek(archDep,indBusq.nroReg*sizeof(Deposito),SEEK_SET);
    fwrite(&regDep,sizeof(Deposito),1,archDep);//guardamos actualizacion

    return TODO_OK;
}
int vectorOrdBuscar_ALU(const Vector* vec, void* elem, Cmp cmp)
{
    //busqueda binaria yo lo manejo con la cantidad de elemntos del vector
    int comp;
    int inicio=0;
    int fin = vec->ce-1;
    int med;
    void* medActual;

    while(inicio <= fin)
    {
        //calcular medio
        med = (inicio+fin)/2;
        //obtengo la direccion
        medActual = vec->vec + (med * vec->tamElem);
        //comparo
        comp = cmp(elem,medActual);
        /*
        comp = 0: El elemento ha sido encontrado, retorna la posición.
        comp < 0: El elemento buscado está antes de la posición actual, ajusta fin.
        comp > 0: El elemento buscado está después de la posición actual, ajusta inicio.
        */

        if(comp == 0)
        {
            memcpy(elem,medActual,vec->tamElem);//retorna en elemto el nro de reg
            return med;//retornas el nro de pos encontrado
        }
        if(comp<0)
        {
            fin = med-1;
        }
        if(comp>0)
        {
            inicio = med +1;
        }
    }
    //sino se encuentra retorna -1
    return NO_ENCONTRADO;
}
//desencriptarTransferencias
void desencriptarTransferencias_ALU(const char* nombreArchivo)
{
    FILE* arch = fopen(nombreArchivo,"rt");
    if(!arch)
    {
        printf("error al abrir archivo %s",nombreArchivo);
        return;
    }
    FILE* archTemp = fopen("temporal.txt","wt");
    if(!archTemp)
    {
        printf("error al crear archivo temporal");
        fclose(arch);
        return;
    }

    char linea[TAM_LIN];
    char* pLinea;
    char* finCad;
    int cant;

    while(fgets(linea,TAM_LIN,arch))
    {
        pLinea = linea;
        finCad = linea;
        //primer valor
        cant = *pLinea-'0';//siempre sera un numero?¿
        pLinea++;

        invertirCad_ALU(pLinea,cant);

        while(*finCad) finCad++;//fin de cadena
        //tambien funciona
        // finCad = linea + strlen(linea); // Fin de la cadena
        finCad-=2;//retrocedo el'\0' y '\n'
        //ultimo valor
        cant = *finCad-'0';

        invertirCad_ALU(finCad-cant,cant);

        //guarda en txt temporal
        fprintf(archTemp,"%s",linea);
    }

    fclose(arch);
    fclose(archTemp);

    //borramos y cambiamos el nombre
    remove(nombreArchivo);
    rename("temporal.txt",nombreArchivo);

}
void invertirCad_ALU(char* cad,int cant)
{
    char* aux= malloc(cant);
    memcpy(aux,cad,cant);

    while(cant>0)
    {
        *cad = aux[cant-1];
        cant--;
        cad++;
    }
    free(aux);
}
//vectorCargarDeArchivo para reemplazar cargar indice
int vectorCargarDeArchivo_ALU(const char* nombreArch, Vector* vector, size_t tamElem)
{
    FILE* arch= fopen(nombreArch,"rb");
    if(!arch)
    {
        printf("Error al abrir el archivo %s",nombreArch);
        return ERR_ARCHIVO;
    }
    fseek(arch,0l,SEEK_END);
    size_t tamArch = ftell(arch);//calcula el tamanio para todo el vector
    vector->vec = malloc(tamArch);

    if(vector->vec==NULL)
    {
        vector->ce = 0;
        vector->cap = 0;
        return ERROR_MEMORIA;
    }

    size_t ce = tamArch/tamElem;//calculo la cantidad de elementos del archivo

    rewind(arch);//vuelvo al principio el puntero archivo

    fread(vector->vec,tamElem,ce,arch);//leo Cant Elem del archivo y cargo todo en el vector

    fclose(arch);

    vector->cap = ce;
    vector->ce = ce;
    vector->tamElem = tamElem;

    /*puedo realizar un fread de a un archivo y llamo a vector insertar al final*/

    return TODO_OK;
}
int vectorInsertarAlFinal_ALU(Vector* vector, const void* elem)
{
    if(vector->ce == vector->cap)
    {
        //redimencionar
        int nueCap = vector->cap *1.5;
        void * nueVec = realloc(vector->vec,nueCap*vector->tamElem);
        //sino se puedo hacer el ralloc
        if(!nueVec)
        {
            printf("error al redimensionar");
            return ERROR_MEMORIA;
        }
        //modificamos
        vector->vec = nueVec;
        vector->cap = nueCap;
    }
    //asigno posicion por la ce
    void* posIns = vector->vec + vector->ce * vector->tamElem;

    memcpy(posIns,elem,vector->tamElem);//copio el elemento a insertar
    vector->ce++;

    return TODO_OK;
}
//vectorDestruir
void vectorDestruir_ALU(Vector* vec)
{
    free(vec->vec);
    vec->vec = NULL;
    vec->cap = 0;
    vec->ce = 0;
    vec->tamElem = 0;
}
//vectorCrear
int vectorCrear_ALU(Vector* vec, size_t tamElem)
{
    vec->cap = 10;
    vec->vec = malloc(tamElem*vec->cap);
    if(vec->vec==NULL)
    {
        return ERROR_MEMORIA;
    }
    vec->ce=0;
    vec->tamElem = tamElem;

    return TODO_OK;
}

//cmpInd
int cmpInd_ALU(const void* pvInd1, const void* pvInd2)
{
    /*
    Devuelve < 0 si el primer elemento es menor que el segundo.
    Devuelve > 0 si el primer elemento es mayor que el segundo.
    Devuelve 0 si ambos elementos son iguales.
    */
    Indice* ind1 = (Indice*)pvInd1;
    Indice* ind2 = (Indice*)pvInd2;

    if(ind1->nroDep != ind2->nroDep)
    {
        return ind1->nroDep - ind2->nroDep;
    }

    return strcmp(ind1->codProd,ind2->codProd);
}

