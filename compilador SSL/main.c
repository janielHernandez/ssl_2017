/************************Programa Principal************************/
int chequearArchivoMicro(char *);
int objetivoNuevo(char *);

int main(int argc, char * argv[]){
  TOKEN tok;
  char nomArchi[TAMNOM];
  int l;
  if (argc < 2) {
    printf("%s\n", "Debe ingresar el archivo de lenguaje micro.");
    return -1;
  }
  if (argc > 2) {
    printf("%s\n", "No puede ingresar mas de un archivo.");
    return -1;
    }
  /*char * nombreFile = argv[1];
  if (!chequearArchivoMicro(nombreFile)){
    int validacion = objetivoNuevo(nombreFile);
    inicializarTablaDeSimbolos();
  	if (validacion) {
        return -2;
      }*/
  strcpy(nomArchi, argv[1]);
  l = strlen(nomArchi);
  if ( l > TAMNOM ){
    printf("Nombre incorrecto del Archivo Fuente\n");
    return -1;
    }
  if ( nomArchi[l-1] != 'm' || nomArchi[l-2] != '.' ){
    printf("Nombre incorrecto del Archivo Fuente\n");
    return -1;
    }
  if ( (in = fopen(nomArchi, "r") ) == NULL ){
    printf("No se pudo abrir archivo fuente\n");
    return -1;
    }
  Objetivo();
  fclose(in);
  return 0;
}



int chequearArchivoMicro(char *archivo) {
  int longitud = strlen(archivo);
  if (archivo[longitud - 2] == '.' && archivo[longitud - 1] == 'm') return 0;
  return -1;
}
int objetivoNuevo(char *nombreFile) {
	int c;
	fileIn = fopen(nombreFile,"r");
	if (!fileIn) {
		printf("%s\n", "Error al abrir el archivo.");
		return 0;
	}
  c = fgetc(fileIn);
  while(!feof(fileIn)){
		/*programa(c);*/
  c = fgetc(fileIn);
}
return 1;
}
