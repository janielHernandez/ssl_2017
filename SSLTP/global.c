/*
 * global.c
 *
 *  Created on: 20/10/2017
 *      Author: martin
 */
#include "global.h"


void IniciarTS(void){
	int i=0, j=0;


	ts[0].clase =LEER;
	strcpy(ts[0].nombre,"leer");


	ts[1].clase=ESCRIBIR;
	strcpy(ts[1].nombre,"palabra");

	ts[2].clase=INICIO;
	strcpy(ts[2].nombre,"palabra");

	ts[3].clase=FIN;
	strcpy(ts[3].nombre,"palabra");

	strcpy(ts[4].nombre,"%");


}

token tipoPalabra(char * buff){

		int i= 5;

		for (i=0;i<=5;i++){

			if (ts[i].nombre == buff)
			{
				return ts[i].clase;
			}
		}
		return ERROR_LEXICO;
}

token scanner() {
	static int tabla[15][13]= {
			{1,3,5,6,7,8,9,10,11,14,13,0,14}, /*0*/
			{1,1,2,2,2,2,2,2,2,2,2,2,2}, /*1*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*2*/
			{4,3,4,4,4,4,4,4,4,4,4,4,4}, /*3*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*4*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*5*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*6*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*7*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*8*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*9*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*10*/
			{14,14,14,14,14,14,14,14,14,12,14,14,14}, /*11*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*12*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99}, /*13*/
			{99,99,99,99,99,99,99,99,99,99,99,99,99} /*14*/
	};

	int i =0;
	char c = fgetc(fileIn);
	int col = columna(c);
	int estado = 0;

		do {
	 car = fgetc(in);
	 col = columna(car);
	 estado = tabla[estado][col];
	 if ( col != 11 ) {
	 buffer[i] = car;
	 i++;
	 }
	}
	while ( !estadoFinal(estado) && !(estado == 14) );
	buffer[i] = '\0';
	switch ( estado )
	{
	 case 2 : if ( col != 11 ){
	 ungetc(car, in);
	 buffer[i-1] = '\0';
	 }
	 return ID;
	 case 4 : if ( col != 11 ) {
	 ungetc(car, in);
	 buffer[i-1] = '\0';
	 }
	 return CONSTANTE;
	 case 5 : return SUMA;
	 case 6 : return RESTA;
	 case 7 : return PARENIZQUIERDO;
	 case 8 : return PARENDERECHO;
	 case 9 : return COMA;
	 case 10 : return PUNTOYCOMA;
	 case 12 : return ASIGNACION;
	 case 13 : return FDT;
	 case 14 : return ERRORLEXICO;
	}
	return 0;
	}
}

int estadoFinal(int e){
if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0;
return 1;
}


int columna(char c) {

	if (isalpha(c)) return 0;

	if (isdigit(c)) return 1;

	if (c == '+') return 2;

	if (c == '-') return 3;

	if (c == '(') return 4;

	if (c == ')') return 5;

	if (c == ',') return 6;

	if (c == ';') return 7;

	if (c == ':') return 8;

	if (c == '=') return 9;

	if (c == EOF) return 10;

	if (isspace(c)) return 11;

	return 12;
}

int buscar(char * buff){
	int i= 5;

	for (i=5;i<=TAMLEX;i++){

		if (ts[i].nombre != "%" && ts[i].nombre == buff)
		{
			return 1;
		}
	}
	return 0;
}

token proximoToken() {

  if(!flagToken) {
    tokenActual = scanner();
    if(tokenActual == ERROR_LEXICO) errorLexico();

    flagToken = 1;

    if(tokenActual == ID) {
      if (!buscar(buffer)) {
    	  colocar(buffer);
      }
    }
  }
  return tokenActual;
}

void Generar(char * nombreOp, char * op1, char * op2, char * op3){
	fprintf(fileOut,"%s ", nombreOp);
	fprintf(fileOut,"%s, ",op1);
	fprintf(fileOut,"%s, ", op2);
	fprintf(fileOut,"%s\n",op3);
}

char* Extraer(REG_EXPRESION reg){
	char * nameReg = reg.nombre;
	return nameReg;
}

void match(token t) {
  if (!(t == proximoToken())) errorSintactico(t);
  flagToken = 0;
}

void programa(void) {
  match(INICIO);
  listaSentencias();
  match(FIN);
}

void objetivo(void) {
  programa();
  match(FDT);
}


void listaIdentificadores(void) {
  token t;
  REG_EXPRESION reg;
  identificador(&reg);
  leer(reg);

  for(t = proximoToken(); t == COMA; t = proximoToken()) {
    match(COMA);
    identificador(&reg);
    leer(reg);
  }
}

void identificador(REG_EXPRESION *presul) {
  match(ID);
  *presul = procesarId();
}

void listaSentencias(void) {
  sentencia();
  while(1) {
    switch(proximoToken()) {
      case ID: case LEER: case ESCRIBIR:
        sentencia();
        break;
      default:
        return;
    }
  }
}

void Expresion(REG_EXPRESION *resultado) {
	REG_EXPRESION operandoIzq, operandoDer;
	REG_EXPRESION op;
	token t;
	Primaria(&operandoIzq);
  for(t = proximoToken(); t == SUMA || t == RESTA; t = proximoToken()) {
     operadorAditivo(op);
     Primaria(&operandoDer);
     operandoIzq = GenInfijo(operandoIzq, op.nombre, operandoDer);
   }
  *resultado = operandoIzq;
}

void listaExpresiones(void){
	token t;
	REG_EXPRESION reg;

	Expresion(&reg);
	escribir(reg);
	for(t=proximoToken(); t == COMA; t = proximoToken())
	{
		match(COMA);
		Expresion(&reg);
		escribir(reg);
	}
}

void Primaria (REG_EXPRESION * presul){
	token tok = proximoToken();

	switch(tok){
	case ID:
			identificador(presul);
			break;
	case CONSTANTE:
			match(CONSTANTE);
			*presul = procesarCte();
			break;
	case PARENIZQUIERDO:
			match(PARENIZQUIERDO);
			Expresion(presul);
			match(PARENDERECHO);
			break;
	default:
		return;
	}
}

void sentencia(void) {
  token tok = proximoToken();
  switch(tok) {
    case ID:
      match(ID);
      match(ASIGNACION);
      Expresion();
      match(PUNTOYCOMA);
      break;
    case LEER:
      match(LEER);
      match(PARENIZQUIERDO);
      listaIdentificadores();
      match(PARENDERECHO);
      match(PUNTOYCOMA);
      break;
    case ESCRIBIR:
      match(ESCRIBIR);
      match(PARENIZQUIERDO);
      listaExpresiones();
      match(PARENDERECHO);
      match(PUNTOYCOMA);
      break;
    default:
      errorSintactico(tok);
      break;
    }
}



void operadorAditivo(void) {
   token t = proximoToken();
   if(t == SUMA || t == RESTA)
      match(t);
   else
      errorSintactico(t);
}

void chequear(char *s) {
  if(!buscar(s)) {
    colocar(s);
    Generar("Declara", s, "Entera", "");
  }
}

void terminar(void) {
	Generar("Detiene", "", "", "");
}

void asignar(REG_EXPRESION izquierda, REG_EXPRESION derecha) {
  Generar("Almacena", Extraer(derecha), izquierda.nombre, "");
}

void leer(REG_EXPRESION in) {
   Generar("Read", in.nombre, "Entera", "");
}

REG_EXPRESION procesarCte(void) {
   REG_EXPRESION t;
   t.clase = CONSTANTE;
   sscanf(buffer, "%d", &t.valor);
   return t;
}

void escribir (REG_EXPRESION out) {
  Generar("Write", Extraer(out), "Entera", "");
}

REG_EXPRESION procesarId(void) {
   REG_EXPRESION t;
   chequear(buffer);
   t.clase = ID;
   strcpy(t.nombre, buffer);
   return t;
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2 ){

	REG_EXPRESION reg;
	static unsigned int numTemp = 1;
	char cadTemp[TAMLEX] = "Temp&";
	char cadNum[TAMLEX];
	char cadOp[TAMLEX];

	if(op[0] == "-")
	{
		strcpy(cadOp, "Resta");
	}
	if(op[0] == "+")
	{
		strcpy(cadOp, "Suma");
	}

	sprintf(cadNum,"%d", numTemp);
	numTemp++;
	strcat(cadTemp, cadNum);

	if(e1.clase == ID) chequear(Extraer(e1));
	if(e2.clase == ID) chequear(Extraer(e2));

	chequear(cadTemp);
	Generar(cadOp,Extraer(e1),Extraer(e2),cadTemp);
	strcpy(reg.nombre,cadTemp);

	return reg;
}

void colocar(char *buff){
	int i= 5;
		for (i=5;i<=TAMLEX;i++){
			if (ts[i].nombre == "%"){
				strcpy(ts[i].nombre, buff);
				ts[i].clase= ID;
				strcpy(ts[i++].nombre,"%");
			}
		}
}
