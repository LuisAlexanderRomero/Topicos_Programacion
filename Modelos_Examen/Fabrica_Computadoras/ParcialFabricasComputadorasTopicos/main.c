/*****************************************************************************************************************
** Apellido/s, Nombre/s:    Romero, Luis Alexander                                  													**
** DNI: 40228032                                                      													**
******************************************************************************************************************
** Corrección																								   	**
** **********																								   	**
**                      																					   	**
******************************************************************************************************************
** Nota:																									   	**
*****************************************************************************************************************/

//#define SOLUCION_DOC
#define SOLUCION_ALU

#include <string.h>
#include <stdlib.h>

#include <stdio.h>
#define TDA_LISTA_IMPL_DINAMICA
#include "../Comun/Comun.h"
#include "../Cadena/Cadena.h"
#include "../Vector/Vector.h"
#include "../ArchivoTopicos/Archivo.h"
#include "SolucionParcialFabricasComputadorasTopicos.h"

#define PROD_COR "Productos-cor.dat"
#define PROD_CHU_TXT "Productos-chu.txt"
#define CLI_COR "Clientes-cor.dat"
#define CLI_CHU "Clientes-chu.dat"
#define PROD_FUS "Productos-fus.dat"
#define CLI_FUS "Clientes-fus.dat"
//agregados
#define TAM_LINEA 201
#define PROD_CHU_DAT "Productos-chu.dat"
#define ERR_TXTABIN 10
#define ERR_ORD_ARCH 20
#define ERR_FUSION 30
#define ERR_ELIM_DUP 40
#define ERR_INSERTAR_ORD 50
#define ERR_SIN_MEM 60


//solucion
int cmpProductos_ALU(const void* prod1, const void* prod2);
int cmpClientes_ALU(const void* cli1, const void* cli2);
int productosTxtABin_ALU(const char* nombreArchivoTxt, const char* nombreArchivoBin);
void actualizarProductos_ALU(void* actualizado, const void* actualizador);
int procesarArchivos_ALU(
    const char* nomArc_Prod_Cor, const char* nomArc_Prod_Chu,
    const char* nomArc_Clie_Cor, const char* nomArc_Clie_Chu,
    const char* nomArc_Prod_Fus, const char* nomArc_Clie_Fus);

//archivo
int ordenarArchivo_ALU(const char* nombreArchivo, size_t tamReg, Cmp cmp);
int fusionarArchivos_ALU(const char* nombreArchivo1, const char* nombreArchivo2, const char* nombreArchivoFus, size_t tamReg, Cmp cmp, Actualizar actualizar);

//vector
bool vectorCrear_ALU(Vector* vec, size_t tamElem);
int vectorCargar_ALU(const char* nombreArch, Vector* vector, size_t tamElem, Cmp cmp);
int vectorInsertarOrd(Vector* vector, const void* elem, Cmp cmp);
int vectorGrabar_ALU(Vector* v, const char* nombreArch);//lo pase a int porque abre un archivo
void vectorDestruir_ALU(Vector* vector);


//falta strings
int main()
{
    int resp = generarArchivos();

    if(resp != TODO_OK)
    {
        printf("Error al generar los archivos");
        return resp;
    }

    puts("Archivos Pre Fusion:\n");

    puts(PROD_COR);
    puts("=========================================");
    mostrarArchivoProductosBin(PROD_COR);

    puts("\n");

    puts(PROD_CHU_TXT);
    puts("=========================================");
    mostrarArchivoProductosTxt(PROD_CHU_TXT);

    puts("\n");

    puts(CLI_COR);
    puts("=========================================");
    mostrarArchivoClientes(CLI_COR);

    puts("\n");

    puts(CLI_CHU);
    puts("=========================================");
    mostrarArchivoClientes(CLI_CHU);


#ifdef SOLUCION_DOC
    resp = procesarArchivos(PROD_COR, PROD_CHU_TXT, CLI_COR, CLI_CHU, PROD_FUS, CLI_FUS);
#else
    resp = procesarArchivos_ALU(PROD_COR, PROD_CHU_TXT, CLI_COR, CLI_CHU, PROD_FUS, CLI_FUS);
#endif

    if(resp != TODO_OK)
    {
        printf("Error al procesar los archivos");
        return resp;
    }

    puts("\n\nArchivos Post Fusion:\n");
    puts(PROD_FUS);
    puts("=========================================");
    mostrarArchivoProductosBin(PROD_FUS);

    puts("\n");
    puts(CLI_FUS);
    puts("=========================================");
    mostrarArchivoClientes(CLI_FUS);

    return 0;
}


int procesarArchivos_ALU(
    const char* nomArc_Prod_Cor, const char* nomArc_Prod_Chu,
    const char* nomArc_Clie_Cor, const char* nomArc_Clie_Chu,
    const char* nomArc_Prod_Fus, const char* nomArc_Clie_Fus )
{
    int ret=0;

    //Pasar archivo de productos de Chubut a binario
    ret = productosTxtABin_ALU(nomArc_Prod_Chu,PROD_CHU_DAT);
    if(ret)
        return ERR_TXTABIN;

    //Ordenar los 4 archivos
    ret = ordenarArchivo_ALU(nomArc_Prod_Cor,sizeof(Producto),cmpProductos);
    if(ret)
        return ERR_ORD_ARCH;

    ret = ordenarArchivo_ALU(PROD_CHU_DAT,sizeof(Producto),cmpProductos);
    if(ret)
        return ERR_ORD_ARCH;

    ret = ordenarArchivo_ALU(nomArc_Clie_Cor,sizeof(Cliente),cmpClientes);
    if(ret)
        return ERR_ORD_ARCH;

    ret = ordenarArchivo_ALU(nomArc_Clie_Chu,sizeof(Cliente),cmpClientes);
    if(ret)
        return ERR_ORD_ARCH;

    //Fusionar archivos sin duplicados y actualizo
    ret = fusionarArchivos_ALU(PROD_CHU_DAT,nomArc_Prod_Cor,nomArc_Prod_Fus,sizeof(Producto),cmpProductos,actualizarProductos_ALU);
    if(ret)
        return ERR_FUSION;

    ret = fusionarArchivos_ALU(nomArc_Clie_Chu,nomArc_Clie_Cor,nomArc_Clie_Fus,sizeof(Cliente),cmpClientes,NULL);
    if(ret)
        return ERR_FUSION;


//    ///Eliminar duplicados y actualizar
//    ret = eliminarDuplicadosArchivoOrd(nomArc_Prod_Fus,sizeof(Producto),cmpProductos,actualizarProductos);
//    if(ret)
//        return ERR_ELIM_DUP;
//    mostrarArchivoProductosBin(nomArc_Prod_Fus);
//
//    ret = eliminarDuplicadosArchivoOrd(nomArc_Clie_Fus,sizeof(Cliente),cmpClientes,NULL);
//    if(ret)
//        return ERR_ELIM_DUP;
//    mostrarArchivoClientes(nomArc_Clie_Fus);

    return TODO_OK;
}

int cmpProductos_ALU(const void* prod1, const void* prod2)
{
    const Producto* prodA = prod1;
    const Producto* prodB = prod2;

    return strcmp(prodA->codigo,prodB->codigo);
}
int cmpClientes_ALU(const void* cli1, const void* cli2)
{
    const Cliente* cliA = cli1;
    const Cliente* cliB = cli2;
    return cliA->codigo - cliB->codigo;
}
int productosTxtABin_ALU(const char* nombreArchivoTxt, const char* nombreArchivoBin)
{
    FILE* arch_txt = fopen(nombreArchivoTxt,"rt");
    if(!arch_txt)
    {
        printf("error al abrir %s",nombreArchivoTxt);
        return ERR_ARCHIVO;
    }
    FILE* arch_bin = fopen(nombreArchivoBin,"wb");
    if(!arch_bin)
    {
        printf("error al abrir %s",nombreArchivoBin);
        fclose(arch_txt);
        return ERR_ARCHIVO;
    }
    //Producto* prod = malloc(sizeof(Producto));
    Producto prod;//pero tengo que pasar a usar . en ves de ->

    char linea[TAM_LINEA];
    char* actual;
    fgets(linea,TAM_LINEA,arch_txt);

    while(!feof(arch_txt))
    {
        //txt_a_bin(linea,prod);
        actual = strchr(linea,'\n');
        *actual = '\0';

        actual = strrchr(linea,'|');
        sscanf(actual+1,"%d",&prod.stock);
        *actual = '\0';

        actual = strrchr(linea,'|');
        sscanf(actual+1,"%f",&prod.precio);
        *actual = '\0';

        actual = strrchr(linea,'|');
        strncpy(prod.descripcion,actual+1,50);
        prod.descripcion[50]='\0';
        *actual = '\0';

        strncpy(prod.codigo,linea,20);
        prod.codigo[20]='\0';

        fwrite(&prod,sizeof(Producto),1,arch_bin);
        fgets(linea,TAM_LINEA,arch_txt);
    }

    fclose(arch_bin);
    fclose(arch_txt);

    return TODO_OK;
}
int ordenarArchivo_ALU(const char* nombreArchivo, size_t tamReg, Cmp cmp)
{
    Vector vector;
    vectorCrear_ALU(&vector,tamReg);//verificar

    vectorCargar_ALU(nombreArchivo,&vector,tamReg,cmp);

    vectorGrabar_ALU(&vector,nombreArchivo);

    vectorDestruir_ALU(&vector);

    return TODO_OK;
}//creo q para cuando esten fusionados no funciona ya que tengo duplicados
bool vectorCrear_ALU(Vector* vec, size_t tamElem)
{
    vec->cap=10;
    vec->vec = malloc(vec->cap * tamElem);

    if(vec->vec==NULL)
        return false;

    vec->ce=0;
    vec->tamElem = tamElem;

    return true;
}
int vectorCargar_ALU(const char* nombreArch, Vector* vector, size_t tamElem, Cmp cmp)
{
    int ret;
    FILE* arch = fopen(nombreArch,"rb");
    if(!arch)
    {
        printf("error al abrir %s",nombreArch);
        return ERR_ARCHIVO;
    }

    void* registro = malloc(tamElem);
    while(fread(registro,tamElem,1,arch))
    {
        ret = vectorInsertarOrd(vector,registro,cmp);
        if(ret)
        {
            //ingresa si hubo error
            fclose(arch);
            free(registro);
            return ERR_INSERTAR_ORD;
        }
    }
    free(registro);
    fclose(arch);
    return TODO_OK;
}
int vectorInsertarOrd(Vector* vector, const void* elem, Cmp cmp)
{
    if(vector->ce == vector->cap)
    {
        int nCap = vector->cap*1.5;
        int* nVec = realloc(vector->vec, nCap * vector->tamElem);

        if(!nVec)
            return ERR_SIN_MEM;

        vector->vec = nVec;
        vector->cap = nCap;
    }

    void* posIns = vector->vec;
    void* ult = vector->vec + (vector->ce - 1)*vector->tamElem;//ultima direccion

    //buscamos la posicion de insercion
    while(posIns <= ult && cmp(elem,posIns) > 0)//mientra posIns sea mayor a ult
        posIns+= vector->tamElem;

    if(posIns <= ult && cmp(elem,posIns) == 0)//si es duplicado
        return DUPLICADO;

    for(void* i= ult; i >= posIns; i-= vector->tamElem)//corro a la derecha los valores
        memcpy(i + vector->tamElem, i,vector->tamElem);//i+1 = i

    memcpy(posIns,elem,vector->tamElem);
    vector->ce++;

    return TODO_OK;
}
int vectorGrabar_ALU(Vector* v, const char* nombreArch)
{
    FILE* arch = fopen(nombreArch, "wb");
    if (!arch)
    {
        printf("Error al abrir %s", nombreArch);
        return ERR_ARCHIVO;
    }
    for (int i = 0; i < v->ce; i++)
    {
        fwrite(v->vec + i * v->tamElem, v->tamElem, 1, arch);
    }
    //fwrite(v->vec, v->tamElem, v->ce, arch);

    fclose(arch);
    return TODO_OK;
}
void vectorDestruir_ALU(Vector* vector)
{
    free(vector->vec);
    vector->vec=NULL;
    vector->ce=0;
    vector->cap=0;
    vector->tamElem=0;
}
int fusionarArchivos_ALU(const char* nombreArchivo1, const char* nombreArchivo2, const char* nombreArchivoFus, size_t tamReg, Cmp cmp, Actualizar actualizar)
{
    FILE* archivo1 = fopen(nombreArchivo1,"rb");
    if(!archivo1)
    {
        printf("error al abrir %s",nombreArchivo1);
        return ERR_ARCHIVO;
    }

    FILE* archivo2 = fopen(nombreArchivo2,"rb");
    if(!archivo2)
    {
        printf("error al abrir %s",nombreArchivo2);
        fclose(archivo1);
        return ERR_ARCHIVO;
    }

    FILE* archivoFus = fopen(nombreArchivoFus,"wb");
    if(!archivoFus)
    {
        printf("error al abrir %s",nombreArchivoFus);
        fclose(archivo1);
        fclose(archivo2);
        return ERR_ARCHIVO;
    }

    void* registro1 = malloc(tamReg);
    void* registro2 = malloc(tamReg);
    int comp;

    //leo ambos archivos
    fread(registro1,tamReg,1,archivo1);
    fread(registro2,tamReg,1,archivo2);

    while(!feof(archivo1) && !feof(archivo2))
    {
        comp= cmp(registro1,registro2);

        if(comp == 0)
        {
            if(actualizar)
            {
                actualizar(registro1,registro2);//suma stock y actualiza precio mayor
            }
            fread(registro2,tamReg,1,archivo2);//leo archivo 2
            /*
            que pasa si hay duplicados¿?¿?¿?
            */
        }
        if(comp < 0)
        {
            fwrite(registro1,tamReg,1,archivoFus);//grabo registro 1
            fread(registro1,tamReg,1,archivo1);//leo archivo1
        }
        if(comp > 0)
        {
            fwrite(registro2,tamReg,1,archivoFus);//grabo archivo 2
            fread(registro2,tamReg,1,archivo2);//leo archivo 2
        }
    }

    while(!feof(archivo1))
    {
        fwrite(registro1,tamReg,1,archivoFus);//grabo registro 1
        fread(registro1,tamReg,1,archivo1);//leo archivo1
    }
    while(!feof(archivo2))
    {
        fwrite(registro2,tamReg,1,archivoFus);//grabo archivo 2
        fread(registro2,tamReg,1,archivo2);//leo archivo 2
    }


    fclose(archivo1);
    fclose(archivo2);
    fclose(archivoFus);

    return TODO_OK;
}
void actualizarProductos_ALU(void* actualizado, const void* actualizador)
{
    Producto* prodA = actualizado;
    const Producto* prodB = actualizador;

    prodA->stock += prodB->stock;
    prodA->precio = prodA->precio >= prodB->precio ? prodA->precio : prodB->precio;

}
