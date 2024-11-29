#ifndef TDAPALABRA_H
#define TDAPALABRA_H

#include <stdbool.h>

#define TAM_PAL 41


typedef struct
{
    char* cursor;
    bool finSec;
}
SecuenciaPal;


typedef struct
{
    char vec[TAM_PAL];
}
Palabra;


void crearSecuencia(SecuenciaPal* sec, char* cadena);
bool leerPalabra(SecuenciaPal* sec, Palabra* pal);
void escribirPalabra(SecuenciaPal* sec, const Palabra* pal);
void escribirCaracter(SecuenciaPal* sec, char c);
void aTitulo(Palabra* pal);
void reposicionarCursor(SecuenciaPal* sec, long despl);
void cerrarSecuencia(SecuenciaPal* sec);
bool finSecuencia(const SecuenciaPal* sec);


#endif // TDAPALABRA_H
