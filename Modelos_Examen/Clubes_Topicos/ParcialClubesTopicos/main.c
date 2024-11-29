/***************************************************************************************
******************************* Complete sus datos *************************************
****************************************************************************************
* Apellido, Nombres: Caro, Nicolas Dario
*
* DNI: 40766722
*
****************************************************************************************/

/**
 Ordenar usa la resuelta. RESUELTO.
 txtABin usa la resuelta. RESUELTO.
 Mal la condición usada para verificar el fin del vector.

 Nota: 4 (Cuatro)
*/


#include <stdio.h>
#include <locale.h>
#include <windows.h>
#include "../SolucionClubesTopicos/UtilitariasClubes.h"
#include "../SolucionClubesTopicos/SolucionClubes.h"

#define ARG_ARCH_CLUB_A 1
#define ARG_ARCH_CLUB_B 2
#define ARG_ARCH_CLUB_FUS 3

#define ERROR_TROZADO 6

#define TAM_LINEA 51
#define ERROR_ARCHIVO 10


int fusionarArchivos_ALU(const char* nombreSociosClubA, const char* nombreSociosClubBTxt, const char* nombreSociosFus);
int sociosBTxtABin_ALU(const char* nombreSociosBTxt, const char* nombreSociosBBin);
SocioB* crearVectorSociosB_ALU(const char* nombreSociosBBin, int* ce);
void ordenarVector_ALU(void* vec, int ce, int tamElem, Cmp cmp);
int fusionarArchivoYVector_ALU(const char* nombreSociosClubA, SocioB* sociosBVec, int ce, const char* nombreSociosFus);

int cmpNro(const void* ele1, const void* ele2);

//string
char* strchr_ALU(const char *cad, int car);
char* strrchr_ALU(const char *cad, int car);
char* strncpy_ALU(char *dest, const char *orig, size_t n);
char* strcat_ALU(char *dest, const char* orig);

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "spanish");	// Cambiar locale - Suficiente para máquinas Linux
    SetConsoleCP(1252); 			// Cambiar STDIN -  Para máquinas Windows
    SetConsoleOutputCP(1252);		// Cambiar STDOUT - Para máquinas Windows

    generarArchivoSociosClubA(argv[ARG_ARCH_CLUB_A]);
    generarArchivoSociosClubBTxtDesord(argv[ARG_ARCH_CLUB_B]);

    mostrarArchSociosA(argv[ARG_ARCH_CLUB_A]);
    mostrarArchSociosBTxt(argv[ARG_ARCH_CLUB_B]);


    /**
    	Esta función debe fusionar los archivos de los socios del club A y los del club B en un nuevo archivo.
    	La estructura del archivo fusionado es la del archivo de socios del club A.
    	El código de socio en el archivo fusionado debe incluir el club de donde proviene. Existen socios que se encuentran en
    	los 2 clubes. Ej: socio 1 del club A, el código resultante es 1-A. El código del socio 2, que está en los 2 clubes
    	queda 2-AB.
    	La antigüedad se toma la mayor.
    */
    ///Para que ejecute su código, debe descomentar fusionarArchivos_ALU y comentar fusionarArchivos.
    fusionarArchivos_ALU(argv[ARG_ARCH_CLUB_A], argv[ARG_ARCH_CLUB_B], argv[ARG_ARCH_CLUB_FUS]);
    ///fusionarArchivos(argv[ARG_ARCH_CLUB_A], argv[ARG_ARCH_CLUB_B], argv[ARG_ARCH_CLUB_FUS]);

    puts("Archivo Fusionado");
    mostrarArchSociosA(argv[ARG_ARCH_CLUB_FUS]);

    return TODO_OK;
}

int fusionarArchivos_ALU(const char* nombreSociosClubA, const char* nombreSociosClubBTxt, const char* nombreSociosFus)
{
    /****   Es necesario hacer el nombre dinamico?¡?    ****/
    //pasamos el txt a bin
    sociosBTxtABin_ALU(nombreSociosClubBTxt,"Socios_CLub_B.dat");

    //cargar archivos en vector
    int ce;
    SocioB* vecSociosB = crearVectorSociosB_ALU("Socios_CLub_B.dat",&ce);

    //ordenar vector generica
    ordenarVector_ALU(vecSociosB,ce,sizeof(SocioB),cmpNro);

    //fusionar archivos usando merge
    fusionarArchivoYVector_ALU(nombreSociosClubA,vecSociosB,ce,nombreSociosFus);

    return TODO_OK;
}
int sociosBTxtABin_ALU(const char* nombreSociosBTxt, const char* nombreSociosBBin)
{
    FILE* archTxt = fopen(nombreSociosBTxt,"rt");
    if(!archTxt)
    {
        printf("error al abrir %s",nombreSociosBTxt);
        return ERROR_ARCHIVO;
    }

    FILE* archBin = fopen(nombreSociosBBin,"rb");
    if(!archBin)
    {
        printf("error al crear %s",nombreSociosBBin);
        fclose(archTxt);
        return ERROR_ARCHIVO;
    }

    SocioB socio;
    char linea[TAM_LINEA];
    char* actual;

    while(fgets(linea,TAM_LINEA,archTxt))
    {
        actual = strchr_ALU(linea,'\n');
        *actual = '\0';

        actual = strrchr_ALU(linea,'|');
        sscanf(actual+1,"%d",&socio.antiguedad);
        *actual = '\0';

        actual = strrchr_ALU(linea,'|');
        strncpy_ALU(socio.apyn,actual+1,50);
        socio.apyn[50]='\0';
        *actual = '\0';

        sscanf(linea,"%d",&socio.nroSocio);
    }

    fclose(archBin);
    fclose(archTxt);

    return TODO_OK;
}
char* strchr_ALU(const char *cad, int car)
{
    while(*cad && *cad!=(char)car)
    {
        cad++;
    }
    if(*cad == '\0') return NULL;

    return (char*)cad;
}
char* strrchr_ALU(const char *cad, int car)
{
    char* aux=(char*)cad;

    while(*aux) aux++;

    while(aux > cad && *aux!=(char)car)
    {
        aux--;
    }

    if(*aux!=(char)car) return NULL;

    return aux;
}
char* strncpy_ALU(char *dest, const char *orig, size_t n)
{
    char *ret = dest;

    while (n-- && *orig)
    {
        *dest = *orig;
        dest++;
        orig++;
    }
    while (n--)
    {
        *dest= '\0';
        dest++;
    }

    return ret;
}

SocioB* crearVectorSociosB_ALU(const char* nombreSociosBBin, int* ce)
{

    FILE* archSocio = fopen(nombreSociosBBin,"rb");
    if(!archSocio)
    {
        printf("error al abrir %s",nombreSociosBBin);
        //return ;
    }

    fseek(archSocio, 0L, SEEK_END);
    *ce = ftell(archSocio)/sizeof(SocioB);

    fseek(archSocio,0L, SEEK_SET);

    SocioB* vecSocioB = malloc(*ce * sizeof(SocioB));
    if(!vecSocioB)
    {
        printf("error al reservar memoria");
        //return o exit?
    }

    SocioB socio;
    SocioB* pVecSocioB = vecSocioB;//para recorrer el vector

    while(fread(&socio,sizeof(SocioB),1,archSocio))
    {
        pVecSocioB->nroSocio = socio.nroSocio;
        strncpy_ALU(pVecSocioB->apyn,socio.apyn,50);
        pVecSocioB->apyn[50]='\0';
        pVecSocioB->antiguedad = socio.antiguedad;

        pVecSocioB++;
    }

    fclose(archSocio);
    return vecSocioB;
}

void ordenarVector_ALU(void* vec, int ce, int tamElem, Cmp cmp)//ordenar por insercion
{
    void* ult = vec + (ce-1)*tamElem;
    void* elemIns = malloc(tamElem);
    void* j;

    for(void*i = vec; i<=ult; i+= tamElem)
    {
        memcpy(elemIns,i,tamElem);
        j = i- tamElem;

        while(j >= vec && cmp(j,elemIns) > 0)
        {
            memcpy(j+tamElem,j,tamElem);
            j-= tamElem;
        }
        memcpy(j+tamElem,elemIns,tamElem);
    }

    free(elemIns);
}

int cmpNro(const void* ele1, const void* ele2)
{
    SocioB* elem1 = (SocioB*) ele1;
    SocioB* elem2 = (SocioB*) ele2;
    return elem1->nroSocio - elem2->nroSocio;
}
int fusionarArchivoYVector_ALU(const char* nombreSociosClubA, SocioB* sociosBVec, int ce, const char* nombreSociosFus)
{
    FILE* archSociosA = fopen(nombreSociosClubA, "rb");
    if(!archSociosA)
    {
        printf("Error al abrir el archivo %s.\n", nombreSociosClubA);
        return ERR_ARCHIVO;
    }

    FILE* archFus = fopen(nombreSociosFus, "wb");
    if(!archFus)
    {
        printf("Error al abrir el archivo %s.\n", nombreSociosFus);
        fclose(archSociosA);
        return ERR_ARCHIVO;
    }

    SocioA regSocioA;
    SocioA regFus;
    fread(&regSocioA,sizeof(SocioA),1,archSociosA);

    //SocioB* pSociosVec = sociosBVec;
    //SocioB* ultSocioVec = sociosBVec + (ce-1);
    int i = 0; //usamos porque tenemos ce
    int comp;

    while(!feof(archSociosA) && i < ce)
    {
        //comp = strcmp(regSocioA.codSocio,itoa(pSociosVec.nroSocio));
        comp = (atoi(regSocioA.codSocio) - sociosBVec->nroSocio);

        if(comp == 0 )
        {
            strcat_ALU(regSocioA.codSocio,"-AB");
            regSocioA.antiguedad = regSocioA.antiguedad > sociosBVec->antiguedad ? regSocioA.antiguedad:sociosBVec->antiguedad;
            fwrite(&regSocioA,sizeof(SocioA),1,archFus);
            fread(&regSocioA,sizeof(SocioA),1,archSociosA);
            sociosBVec++;
            i++;
        }
        if(comp < 0)
        {
            strcat_ALU(regSocioA.codSocio,"-A");
            fwrite(&regSocioA,sizeof(SocioA),1,archFus);
            fread(&regSocioA,sizeof(SocioA),1,archSociosA);
        }
        if(comp > 0)
        {
            //strcpy(regFus.codSocio,itoa(sociosBVec->nroSocio));
            sprintf(regFus.codSocio,"%d",sociosBVec->nroSocio);
            strcat_ALU(regFus.codSocio,"-B");
            strncpy_ALU(regFus.apyn,sociosBVec->apyn,50);
            regFus.apyn[50] = '\0';
            regFus.antiguedad = sociosBVec->antiguedad;
            fwrite(&regFus,sizeof(SocioA),1,archFus);
            sociosBVec++;
            i++;
        }
    }
    while(!feof(archSociosA))
    {
        strcat(regSocioA.codSocio,"-A");
        fwrite(&regSocioA,sizeof(SocioA),1,archFus);
        fread(&regSocioA,sizeof(SocioA),1,archSociosA);
    }
    while(i<ce)
    {
        //strcpy(regFus.codSocio,itoa(sociosBVec->nroSocio));
        sprintf(regFus.codSocio,"%d",sociosBVec->nroSocio);
        strcat_ALU(regFus.codSocio,"-B");
        strncpy_ALU(regFus.apyn,sociosBVec->apyn,50);
        regFus.apyn[50] = '\0';
        regFus.antiguedad = sociosBVec->antiguedad;
        fwrite(&regFus,sizeof(SocioA),1,archFus);
        sociosBVec++;
        i++;
    }

    fclose(archSociosA);
    fclose(archFus);

    return TODO_OK;
}
char* strcat_ALU(char *dest, const char* orig)
{
    char *ret = dest;
    while(*dest) dest++;//voy al final de cadena destino

    while(*orig)
    {
        *dest = *orig;
        dest++;
        orig++;
    }
    *dest='\0';

    return ret;
}



