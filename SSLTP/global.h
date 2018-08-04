#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define TAMLEX 1000

typedef enum { INICIO,FIN,LEER,ESCRIBIR,ID,CONSTANTE,PARENIZQUIERDO,PARENDERECHO,PUNTOYCOMA,
	COMA,ASIGNACION,SUMA,RESTA,FDT,ERROR_LEXICO = 99 } token;

typedef struct {
  token clase;
  char nombre[TAMLEX];
  int valor;
} REG_EXPRESION;

typedef struct {
	char nombre[50+1];
	token clase;
} SYMBOL;
SYMBOL[1000] = { {"inicio", INICIO}, {"fin", FIN}, {"leer", LEER}, {"escribir", ESCRIBIR}, {"$", 99} };

FILE * fileIn;
FILE * fileOut;
token tokenActual;
char buffer[TAMLEX];
int flagToken = 0;
SYMBOL ts[TAMLEX];




/*
 * Funciones para el scanner
 */
token scanner(); /*Ahora analiza el buffer creo...*/
int columna (char);

/*
 *
 */

token proximoToken();
void match(token );
void listaIdentificadores(void);
void identificador(REG_EXPRESION *);
void listaSentencias(void);
void sentencia(void);
void operadorAditivo(void);

void chequear(char *);
void terminar(void);
void asignar(REG_EXPRESION , REG_EXPRESION );

void leer(REG_EXPRESION in);

REG_EXPRESION procesarCte(void);

void escribir (REG_EXPRESION );
REG_EXPRESION procesarId(void);


void errorSintactico(token );
void errorLexico(void);
void listaExpresiones(void);
void colocar(char *);
char* Extraer(REG_EXPRESION );
int buscar(char * );

void Generar(char * , char *, char *, char *);
void Expresion(REG_EXPRESION *);
void Primaria (REG_EXPRESION * );
REG_EXPRESION GenInfijo(REG_EXPRESION , char *, REG_EXPRESION );
token tipoPalabra(char *);
#endif /* GLOBAAL_H_ */
