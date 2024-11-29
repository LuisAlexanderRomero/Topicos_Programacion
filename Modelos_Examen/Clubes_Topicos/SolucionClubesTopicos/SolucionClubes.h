#ifndef ARCHIVO_H
#define ARCHIVO_H

#include <stdio.h>

#define TODO_OK 0
#define ERR_ARCHIVO 1


typedef struct
{
	char codSocio[11];
	char apyn[51];
	int antiguedad;
}
SocioA;


typedef struct
{
	int nroSocio;
	char apyn[51];
	int antiguedad;
}
SocioB;


typedef int (*Cmp)(const void*, const void*);


int fusionarArchivos(const char* nombreSociosClubA, const char* nombreSociosClubBTxt, const char* nombreSociosFus);
int sociosBTxtABin(const char* nombreSociosBTxt, const char* nombreSociosBBin);
SocioB* crearVectorSociosB(const char* nombreSociosBBin, int* ce);
int fusionarArchivoYVector(const char* nombreSociosClubA, SocioB* sociosBVec, int ce, const char* nombreSociosFus);

void ordenarVector(void* vec, int ce, int tamElem, Cmp cmp);

#endif // ARCHIVO
