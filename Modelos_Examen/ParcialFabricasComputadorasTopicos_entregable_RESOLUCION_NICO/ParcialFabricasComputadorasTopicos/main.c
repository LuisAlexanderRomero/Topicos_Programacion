/*****************************************************************************************************************
** Apellido/s, Nombre/s:                                      													**
** DNI:                                                       													**
******************************************************************************************************************
** Corrección																								   	**
** **********																								   	**
**                      																					   	**
******************************************************************************************************************
** Nota:																									   	**
*****************************************************************************************************************/

//#define SOLUCION_DOC
#define SOLUCION_ALU


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TDA_LISTA_IMPL_DINAMICA
#include "../Comun/Comun.h"
#include "../Cadena/Cadena.h"
#include "../Vector/Vector.h"
#include "../ArchivoTopicos/Archivo.h"
#include "../SolucionParcialFabricasComputadorasTopicos/SolucionParcialFabricasComputadorasTopicos.h"

#define PROD_COR "Productos-cor.dat"
#define PROD_CHU_TXT "Productos-chu.txt"
#define CLI_COR "Clientes-cor.dat"
#define CLI_CHU "Clientes-chu.dat"
#define PROD_FUS "Productos-fus.dat"
#define CLI_FUS "Clientes-fus.dat"
#define PROD_CHU_BIN "Productos-chu.dat"

#define TODO_OK 0
#define FALLA_RESERVA_MEMORIA 2
#define FALLA_APERTURA_ARCHIVO 3
#define FALLA_LINEA_LARGA 4
#define ERR_VECTOR_LLENO 5
#define ERR_REDIMENSIONAR 6
#define ERR_VECTOR_CREAR 7

#define TAM_LINEA 201
#define TAM_CODIGO 20
#define TAM_DESCRIPCION 50
#define TAM_PRECIO 10
#define TAM_STOCK 8
#define FACTOR_INCR 1.5
#define CAP_INI 10

int procesarArchivos_ALU(
    const char* nomArc_Prod_Cor, const char* nomArc_Prod_Chu,
    const char* nomArc_Clie_Cor, const char* nomArc_Clie_Chu,
    const char* nomArc_Prod_Fus, const char* nomArc_Clie_Fus );
int productosTxtABin_ALU(const char* nombreArchivoTxt,
                         const char* nombreArchivoBin);
int TxtVABin_ALU(char* linea, void* reg);
int ordenarArchivo_ALU(const char* nombreArchivo, size_t tamReg, Cmp cmp);
int vectorCrear_ALU(Vector* vector, size_t tamElem);
int vectorCargar_ALU(const char* nombreArch, Vector* vector, size_t tamElem);
bool redimensionarVector_ALU(Vector* vector, float factor);
void vectorOrdPorSeleccion_ALU(Vector* vector, Cmp cmp);
const void* buscarMenor_ALU(const void* ini, const void* fin,
                            size_t tamElem, Cmp cmp);
void intercambiar_ALU(void* a, void* b, size_t tamElem);
int vectorGrabar_ALU(Vector* vector, const char* nomArch);
void vectorDestruir_ALU(Vector* vector);
int fusionarArchivos_ALU(const char* nombreArchivo1,
                         const char* nombreArchivo2,
                         const char* nombreArchivoFus,
                         size_t tamReg, Cmp cmp,
                         Actualizar actualizar);
void cierreYLiberar_ALU(FILE* arch1,
                        FILE* arch2,
                        FILE* arch3,
                        void* objeto1,
                        void* objeto2,
                        void* objeto3);
int cmpProductos_ALU(const void* prod1, const void* prod2);
int cmpClientes_ALU(const void* cli1, const void* cli2);
void actualizarProductos_ALU(void* pvActualizado, const void* pvActualizador);

int main()
{
    int resp = generarArchivos();

    if(resp != TODO_OK)
    {
        printf("Error al generar los archivos");
        return resp;
    }

    #ifdef SOLUCION_DOC
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
    #endif

#ifdef SOLUCION_DOC
    resp = procesarArchivos(PROD_COR, PROD_CHU_TXT, CLI_COR, CLI_CHU, PROD_FUS, CLI_FUS);
#else
    resp = procesarArchivos_ALU(PROD_COR, PROD_CHU_TXT, CLI_COR, CLI_CHU, PROD_FUS, CLI_FUS);
#endif

    #ifdef SOLUCION_DOC
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
    #endif

    return 0;
}


int procesarArchivos_ALU(
    const char* nomArc_Prod_Cor, const char* nomArc_Prod_Chu,
    const char* nomArc_Clie_Cor, const char* nomArc_Clie_Chu,
    const char* nomArc_Prod_Fus, const char* nomArc_Clie_Fus )
{
    /// Resolver

    /// 1) Pasaje de txt a bin
    int res = 0;
    res = productosTxtABin_ALU(nomArc_Prod_Chu, PROD_CHU_BIN);
    if(res!=0)
    {
        printf("Falla con codigo %d", res);
    }
    puts("Post Pasaje txt-bin del Prod Chu\n");
    mostrarArchivoProductosBin(PROD_CHU_BIN);
    puts("\n");

    /// 2) Ordenar los archivos
    res = ordenarArchivo_ALU(PROD_CHU_BIN,sizeof(Producto),cmpProductos);
    if(res!=0)
    {
        printf("Falla con codigo %d", res);
    }
    res = ordenarArchivo_ALU(PROD_COR,sizeof(Producto),cmpProductos);
    if(res!=0)
    {
        printf("Falla con codigo %d", res);
    }
    res = ordenarArchivo_ALU(CLI_CHU,sizeof(Cliente),cmpClientes);
    if(res!=0)
    {
        printf("Falla con codigo %d", res);
    }
    res = ordenarArchivo_ALU(CLI_COR,sizeof(Cliente),cmpClientes);
    if(res!=0)
    {
        printf("Falla con codigo %d", res);
    }

    puts("Post ordenado del Cli Cor\n");
    mostrarArchivoClientes(CLI_COR);
    puts("\n");
    puts("Post ordenado del Cli Chu\n");
    mostrarArchivoClientes(CLI_CHU);
    puts("\n");
    puts("Post ordenado del Prod Cor\n");
    mostrarArchivoProductosBin(PROD_COR);
    puts("\n");
    puts("Post ordenado del Prod Chu\n");
    mostrarArchivoProductosBin(PROD_CHU_BIN);
    puts("\n");

    /// 3) Fusionar
    //fusionarArchivos(PROD_COR,PROD_CHU_BIN,PROD_FUS,sizeof(Producto),cmpProductos);
    fusionarArchivos_ALU(PROD_COR,PROD_CHU_BIN,PROD_FUS,sizeof(Producto),cmpProductos,actualizarProductos_ALU);
    fusionarArchivos_ALU(CLI_COR,CLI_CHU,CLI_FUS,sizeof(Cliente),cmpClientes,NULL);

    puts("Post fusion del Prod Fus\n");
    mostrarArchivoProductosBin(PROD_FUS);
    puts("\n");
    puts("Post fusion del Cli Fus\n");
    mostrarArchivoClientes(CLI_FUS);
    puts("\n");

    /*
    /// 4) Eliminar duplicados y actualizar
    eliminarDuplicadosArchivoOrd(PROD_FUS,sizeof(Producto),cmpProductos, actualizarProductos);
    eliminarDuplicadosArchivoOrd(CLI_FUS,sizeof(Cliente),cmpClientes, actualizarProductos);

    puts("Post eliminacion dup del Prod Fus\n");
    mostrarArchivoProductosBin(PROD_FUS);
    puts("\n");
    puts("Post eliminacion dup del Cli Fus\n");
    mostrarArchivoClientes(CLI_FUS);
    puts("\n");
    */

    return TODO_OK;
}

int productosTxtABin_ALU(const char* nombreArchivoTxt, const char* nombreArchivoBin)
{
    FILE* archBin = fopen(nombreArchivoBin, "wb");

    if(!archBin)
    {
        return FALLA_APERTURA_ARCHIVO;
    }

    FILE* archTxt = fopen(nombreArchivoTxt, "rt");

    if(!archTxt)
    {
        fclose(archBin);
        return FALLA_APERTURA_ARCHIVO;
    }

    size_t tamReg = sizeof(Producto);
    void* reg = malloc(tamReg);

    if(!reg)
    {
        fclose(archBin);
        fclose(archTxt);
        return FALLA_RESERVA_MEMORIA;
    }

    char linea[TAM_LINEA];

    while(fgets(linea, TAM_LINEA, archTxt))
    {
        TxtVABin_ALU(linea, reg);
        fwrite(reg, tamReg, 1, archBin);
    }

    free(reg);
    fclose(archBin);
    fclose(archTxt);

    return TODO_OK;
}

int TxtVABin_ALU(char* linea, void* reg)
{
    char* act = strchr(linea, '\n');

    if(!act)
    {
        return FALLA_LINEA_LARGA;
    }

    Producto* prod = reg;

    // stock
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act+1, "%d", &prod->stock);

    // precio
    *act = '\0';
    act = strrchr(linea, '|');
    sscanf(act+1, "%f", &prod->precio);

    // descripcion
    *act = '\0';
    act = strrchr(linea, '|');
    strncpy(prod->descripcion, act+1, TAM_DESCRIPCION);
    prod->descripcion[TAM_DESCRIPCION]='\0';

    // codigo
    *act = '\0';
    strncpy(prod->codigo, linea, TAM_CODIGO);
    prod->codigo[TAM_CODIGO]='\0';

    return TODO_OK;
}

int ordenarArchivo_ALU(const char* nombreArchivo, size_t tamReg, Cmp cmp)
{
    Vector vector;

    vectorCrear_ALU(&vector, tamReg);

    vectorCargar_ALU(nombreArchivo, &vector, tamReg);

    vectorOrdPorSeleccion_ALU(&vector, cmp);

    vectorGrabar_ALU(&vector, nombreArchivo);

    vectorDestruir_ALU(&vector);

    return TODO_OK;
}

int vectorCrear_ALU(Vector* vector, size_t tamElem)
{
    vector->ce = 0;
    vector->vec = malloc(CAP_INI * tamElem);

    if(vector->vec == NULL)
    {
        vector->cap = 0;
        return ERR_VECTOR_CREAR;
    }

    vector->cap = CAP_INI;
    vector->tamElem = tamElem;

    return TODO_OK;
}

int vectorCargar_ALU(const char* nombreArch, Vector* vector, size_t tamElem)
{
    FILE* arch = fopen(nombreArch, "rb");

    if(!arch)
    {
        return ERR_ARCHIVO;
    }

    size_t bytes = 0;

    while((bytes = fread(vector->vec + (vector->ce*tamElem), tamElem, 1, arch))>0)
    {
        vector->ce++;
        if(vector->ce == vector->cap)
        {
            if(!redimensionarVector_ALU(vector, FACTOR_INCR))
            {
                fclose(arch);
                return ERR_VECTOR_LLENO;
            }
        }
    }

    if (ferror(arch))
    {
        fclose(arch);
        return ERR_ARCHIVO;
    } // Si fread fallo verifico si fue feof o error de archivo.
    else
    {
        fclose(arch);
    }

    return TODO_OK;
}

bool redimensionarVector_ALU(Vector* vector, float factor)
{
    size_t nCap = vector->cap * factor;

    if(nCap < CAP_INI)
    {
        return ERR_REDIMENSIONAR;
    }

    int* nVec = realloc(vector->vec, nCap * vector->tamElem);

    if(!nVec)
    {
        return ERR_REDIMENSIONAR;
    }

    vector->vec = nVec;
    vector->cap = nCap;

    return true;
}

void vectorOrdPorSeleccion_ALU(Vector* vector, Cmp cmp)
{
    void* ult = vector->vec + (vector->ce - 1) * vector->tamElem;
    void* m = NULL;

    for(void* i = vector->vec; i < ult; i += vector->tamElem)
    {
        m = (void*)buscarMenor_ALU(i, ult, vector->tamElem, cmp);
        intercambiar_ALU(m, i, vector->tamElem);
    }
}


const void* buscarMenor_ALU(const void* ini, const void* fin, size_t tamElem, Cmp cmp)
{
    const void* m = ini;

    for(const void* j = ini + tamElem; j <= fin; j += tamElem)
    {
        if(cmp(j, m) < 0)
        {
            m = j;
        }
    }

    return m;
}

void intercambiar_ALU(void* a, void* b, size_t tamElem)
{
    void* aTemp = malloc(tamElem);
    memcpy(aTemp, a, tamElem);
    memcpy(a, b, tamElem);
    memcpy(b, aTemp, tamElem);
    free(aTemp);
}

int vectorGrabar_ALU(Vector* vector, const char* nomArch)
{
    FILE* arch = fopen(nomArch, "wb");

    if(!arch)
    {
        return ERR_ARCHIVO;
    }

    fwrite(vector->vec, vector->tamElem, vector->ce, arch);

    fclose(arch);

    return TODO_OK;
}


void vectorDestruir_ALU(Vector* vector)
{
    free(vector->vec);
    vector->vec = NULL;
    vector->ce = 0;
    vector->cap = 0;
}

int fusionarArchivos_ALU(const char* nombreArchivo1,
                         const char* nombreArchivo2,
                         const char* nombreArchivoFus,
                         size_t tamReg, Cmp cmp,
                         Actualizar actualizar)
{
    FILE* arch1 = fopen(nombreArchivo1, "r+b");

    if(!arch1)
    {
        return ERR_ARCHIVO;
    }

    FILE* arch2 = fopen(nombreArchivo2, "rb");

    if(!arch2)
    {
        return ERR_ARCHIVO;
    }


    char nomArch3[51];
    strcpy(nomArch3, nombreArchivo1);
    char* ext = strchr(nomArch3, '-') + 1;
    strcpy(ext, "fusion.bin");

    FILE* archFus = fopen(nomArch3, "wb");

    if(!archFus)
    {
        return ERR_ARCHIVO;
    }

    void* objeto1 = malloc(tamReg);
    void* objeto2 = malloc(tamReg);
    void* objetoNuevo = malloc(tamReg);

    size_t bytes1 = fread(objeto1, sizeof(tamReg), 1, arch1);
    size_t bytes2 = fread(objeto2, sizeof(tamReg), 1, arch2);
    int comp = 0;

    while(bytes1 > 0 && bytes2 > 0 )
    {
        comp = cmp(objeto1,objeto2);

        if(comp == 0)
        {
            if(actualizar)
            {
                actualizar(objeto1, objeto2);
            }
            /// INICIO Aqui junto duplicados de objeto1
            memcpy(objetoNuevo,objeto1,tamReg);
            while((bytes1 > 0) && (cmp(objetoNuevo, objeto1) == 0))
            {
                if(actualizar)
                {
                    actualizar(objetoNuevo, objeto1);
                }

                bytes1 = fread(objeto1, sizeof(tamReg), 1, arch1);
            }
            memcpy(objeto1,objetoNuevo,tamReg);
            /// FIN Aqui junto duplicados de objeto1
            bytes2 = fread(objeto2, sizeof(tamReg), 1, arch2);
        }

        if(comp < 0) // Prod sin (mas) movs
        {
            fwrite(objeto1, sizeof(tamReg), 1, archFus);
            bytes1 = fread(objeto1, sizeof(tamReg), 1, arch1);
        }

        if(comp > 0) // Prod nuevo
        {
            memcpy(objetoNuevo,objeto2,tamReg);
            while((bytes2 > 0) && (cmp(objetoNuevo, objeto2) == 0))
            {
                if(actualizar)
                {
                    actualizar(objetoNuevo, objeto2);
                }

                bytes2 = fread(objeto2, sizeof(tamReg), 1, arch2);
            }
            fwrite(objetoNuevo, sizeof(tamReg), 1, archFus);
        }
    }

    if(ferror(arch1) || ferror(arch2))
    {
        cierreYLiberar_ALU(arch1, arch2, archFus, objeto1, objeto2, objetoNuevo);
        return ERR_ARCHIVO;
    }

    while(bytes1 > 0)
    {
        memcpy(objetoNuevo,objeto1,tamReg);
        while((bytes1 > 0) && (cmp(objetoNuevo, objeto1) == 0))
        {
            if(actualizar)
            {
                actualizar(objetoNuevo, objeto1);
            }
            bytes1 = fread(objeto1, sizeof(tamReg), 1, arch1);
        }
        fwrite(objetoNuevo, sizeof(tamReg), 1, archFus);
    }

    if(ferror(arch1))
    {
        cierreYLiberar_ALU(arch1, arch2, archFus, objeto1, objeto2, objetoNuevo);
        return ERR_ARCHIVO;
    }

    while(bytes2 > 0)
    {

        memcpy(objetoNuevo,objeto1,tamReg);
        while((bytes1 > 0) && (cmp(objetoNuevo, objeto1) == 0))
        {
            if(actualizar)
            {
                actualizar(objetoNuevo, objeto1);
            }
            bytes1 = fread(objeto1, sizeof(tamReg), 1, arch1);
        }
        fwrite(objetoNuevo, sizeof(tamReg), 1, archFus);
    }

    if(ferror(arch2))
    {
        cierreYLiberar_ALU(arch1, arch2, archFus, objeto1, objeto2, objetoNuevo);
        return ERR_ARCHIVO;
    }

    cierreYLiberar_ALU(arch1, arch2, archFus, objeto1, objeto2, objetoNuevo);
    return TODO_OK;
}

void cierreYLiberar_ALU(FILE* arch1,
                        FILE* arch2,
                        FILE* arch3,
                        void* objeto1,
                        void* objeto2,
                        void* objeto3)
{
    fclose(arch1);
    fclose(arch2);
    fclose(arch3);
    free(objeto1);
    free(objeto2);
    free(objeto3);
}

int cmpProductos_ALU(const void* prod1, const void* prod2)
{
    const Producto* prodI = (const Producto*) prod1;
    const Producto* prodII = (const Producto*) prod1;
    return strcmp(prodI->codigo, prodII->codigo);
}
int cmpClientes_ALU(const void* cli1, const void* cli2)
{
    const Cliente* cliI = (const Cliente*) cli1;
    const Cliente* cliII = (const Cliente*) cli2;
    return cliI->codigo-cliII->codigo;
}

void actualizarProductos_ALU(void* pvActualizado, const void* pvActualizador)
{
    Producto* prod1 = (Producto*) pvActualizado;
    Producto* prod2 = (Producto*) pvActualizador;

    if(prod1->precio<prod2->precio)
        prod1->precio = prod2->precio;

    prod1->stock+=prod2->stock;
}
