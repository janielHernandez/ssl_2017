/*********************Scanner************************************/
TOKEN scanner(){
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
  int car;
  int col;
  int estado = 0;
  int i = 0;
  do
  {
    car = fgetc(in);
    col = columna(car);
    estado = tabla[estado][col];
    if ( col != 11 ){
    buffer[i] = car;
    i++;
    }
  }
  while ( !estadoFinal(estado) && !(estado == 14) );
  buffer[i] = '\0';
  switch ( estado ){
    case 2 : if ( col != 11 ){
      ungetc(car, in);
      buffer[i-1] = '\0';
      }
      return ID;
    case 4 : if ( col != 11 ){
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

int estadoFinal(int e){
  if ( e == 0 || e == 1 || e == 3 || e == 11 || e == 14 ) return 0;
  return 1;
}

int columna(int c){
  if ( isalpha(c) ) return 0;
  if ( isdigit(c) ) return 1;
  if ( c == '+' ) return 2;
  if ( c == '-' ) return 3;
  if ( c == '(' ) return 4;
  if ( c == ')' ) return 5;
  if ( c == ',' ) return 6;
  if( c == ';' ) return 7;
  if ( c == ':' ) return 8;
  if ( c == '=' ) return 9;
  if ( c == EOF ) return 10;
  if ( isspace(c) ) return 11;
return 12;
}
/********Fin Scanner**********************************************/

void Objetivo(void){
  Programa();
  Match(FDT);
  Terminar();
  }

void Programa(void){
  Comenzar();
  Match(INICIO);
  ListaSentencias();
  Match(FIN);
}

void ListaSentencias(void)
{
  Sentencia();
  while ( 1 ){
    switch (ProximoToken()){
      case ID :
      case LEER :
      case ESCRIBIR:
      Sentencia();
      break;
      default : return;
    }
  }
}

void Sentencia(void){
  TOKEN tok = ProximoToken();
  REG_EXPRESION izq, der;
  switch (tok){
    case ID :
      Identificador(&izq);
      Match(ASIGNACION);
      Expresion(&der);
      Asignar(izq, der);
      Match(PUNTOYCOMA);
      break;
    case LEER :
      Match(LEER);
      Match(PARENIZQUIERDO);
      ListaIdentificadores();
      Match(PARENDERECHO);
      Match(PUNTOYCOMA);
      break;
    case ESCRIBIR:
      Match(ESCRIBIR);
      Match(PARENIZQUIERDO);
      ListaExpresiones();
      Match(PARENDERECHO);
      Match(PUNTOYCOMA);
      break;
      default : return;
    }
}

void ListaIdentificadores(void){
  TOKEN t;
  REG_EXPRESION reg;
  Identificador(&reg);
  Leer(reg);
  for(t = ProximoToken(); t == COMA; t = ProximoToken() ){
    Match(COMA);
    Identificador(&reg);
    Leer(reg);
  }
}

void Identificador(REG_EXPRESION * presul){
  Match(ID);
  *presul = ProcesarId();
}

void ListaExpresiones(void){
  TOKEN t;
  REG_EXPRESION reg;
  Expresion(&reg);
  Escribir(reg);
  for ( t = ProximoToken(); t == COMA; t = ProximoToken()){
    Match(COMA);
    Expresion(&reg);
    Escribir(reg);
  }
}
void Expresion(REG_EXPRESION * presul){
  REG_EXPRESION operandoIzq, operandoDer;
  char op[TAMLEX];
  TOKEN t;
  Primaria(&operandoIzq);
  for ( t = ProximoToken(); t == SUMA || t == RESTA; t = ProximoToken() ){
    OperadorAditivo(op);
    Primaria(&operandoDer);
    operandoIzq = GenInfijo(operandoIzq, op, operandoDer);
    }
  *presul = operandoIzq;
}

void Primaria(REG_EXPRESION * presul){
  TOKEN tok = ProximoToken();
  switch ( tok ){
    case ID :
      Identificador(presul);
      break;
    case CONSTANTE :
      Match(CONSTANTE);
      *presul = ProcesarCte();
      break;
    case PARENIZQUIERDO :
      Match(PARENIZQUIERDO);
      Expresion(presul);
      Match(PARENDERECHO);
      break;
    default : return;
  }
}

void OperadorAditivo(char * presul){
  TOKEN t = ProximoToken();
  if ( t == SUMA || t == RESTA ){
    Match(t);
    strcpy(presul, ProcesarOp());
  }else{
    ErrorSintactico(t);
  }
}

/********/

REG_EXPRESION ProcesarCte(void){
  REG_EXPRESION reg;
  reg.clase = CONSTANTE;
  strcpy(reg.nombre, buffer);
  sscanf(buffer, "%d", &reg.valor);
  return reg;
}

REG_EXPRESION ProcesarId(void){
  REG_EXPRESION reg;
  Chequear(buffer);
  reg.clase = ID;
  strcpy(reg.nombre, buffer);
  return reg;
}

char * ProcesarOp(void){
  return buffer;
}

void Leer(REG_EXPRESION in){
  Generar("Read", in.nombre, "Entera", "");
}

void Escribir(REG_EXPRESION out){
    /* Genera la instruccion para escribir */
    Generar("Write", Extraer(&out), "Entera", "");
}

REG_EXPRESION GenInfijo(REG_EXPRESION e1, char * op, REG_EXPRESION e2){
  REG_EXPRESION reg;
  static unsigned int numTemp = 1;
  char cadTemp[TAMLEX] ="Temp&";
  char cadNum[TAMLEX];
  char cadOp[TAMLEX];
  if ( op[0] == '-' ) strcpy(cadOp, "Restar");
  if ( op[0] == '+' ) strcpy(cadOp, "Sumar");
  sprintf(cadNum, "%d", numTemp);
  numTemp++;
  strcat(cadTemp, cadNum);
  if ( e1.clase == ID) Chequear(Extraer(&e1));
  if ( e2.clase == ID) Chequear(Extraer(&e2));
  Chequear(cadTemp);
  Generar(cadOp, Extraer(&e1), Extraer(&e2), cadTemp);
  strcpy(reg.nombre, cadTemp);
  return reg;
}

void Match(TOKEN t){
  if ( !(t == ProximoToken()) ) ErrorSintactico();
  flagToken = 0;
}

TOKEN ProximoToken(){
  if (!flagToken){
    tokenActual = scanner();
    if ( tokenActual == ERRORLEXICO ) ErrorLexico();
    flagToken = 1;
    if ( tokenActual == ID ){
      Buscar(buffer, TS, &tokenActual);
      }
    }
    return tokenActual;
}

void ErrorLexico(){
  printf("Error Lexico\n");
}

void ErrorSintactico(){
    printf("Error Sintactico\n");
}


void Generar(char * co, char * a, char * b, char * c){
    printf("%s %s%c%s%c%s\n", co, a, ',', b, ',', c);
}

char * Extraer(REG_EXPRESION * preg){
    return preg->nombre;
}

int Buscar(char * id, RegTS * TS, TOKEN * t){
  int i = 0;
  while ( strcmp("$", TS[i].identifi) ){
      if ( !strcmp(id, TS[i].identifi) ){
        *t = TS[i].t;
        return 1;
        }
      i++;
      }
  return 0;
}

void Colocar(char * id, RegTS * TS){
    /* Agrega un identificador a la TS */
    int i = 4;
    while ( strcmp("$", TS[i].identifi) ) i++;
    if ( i < 999 ){
      strcpy(TS[i].identifi, id );
      TS[i].t = ID;
      strcpy(TS[++i].identifi, "$" );
      }
}
void Chequear(char * s){
    TOKEN t;
    if ( !Buscar(s, TS, &t) ){
      Colocar(s, TS);
      Generar("Declara", s, "Entera", "");
    }
}

void Comenzar(void){
}
void Terminar(void){
  Generar("Detiene", "", "", "");
}
void Asignar(REG_EXPRESION izq, REG_EXPRESION der){
  Generar("Almacena", Extraer(&der), izq.nombre, "");
}
