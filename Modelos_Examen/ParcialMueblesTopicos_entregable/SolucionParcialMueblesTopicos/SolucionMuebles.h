#ifndef SOLUCION_MUEBLES_H
#define SOLUCION_MUEBLES_H

#include <stdbool.h>


typedef struct
{
	int nroDep;
	char codProd[16];
	int stock;
}
Deposito;


typedef struct
{
	int depOrigen;
	int depDestino;
	char codProd[16];
	int cantidad;
}
Transferencia;


typedef struct
{
	int nroDep;
	char codProd[16];
	int cantidad;
}
TransferenciaElem;


typedef struct
{
	int nroDep;
	char codProd[16];
	long nroReg;
}
Indice;


void generarArchivos(bool encriptarTransferencias, bool generaIndice);

void mostrarArchivoTransferencias(const char* archTransferencias);
void mostrarArchivoDepositos(const char* archDepositos);
void encriptarTransferencias(const char* nombreArchivo);
void desencriptarTransferencias(const char* nombreArchivo);
int cmpTransfLista(const void* pvTr1, const void* pvtr2);
int cmpInd(const void* pvInd1, const void* pvInd2);
void actualizarTransf(void* pvTrActualizada, const void* pvTrActualizadora);
void imprimirIndice(const void* elem, void* datosImprimir, int nivel);

void actualizarDepositos(const char* archTransferencias, const char* archDepositos);


#endif // SOLUCION_MUEBLES_H
