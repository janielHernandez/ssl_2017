#include "global.h"


int chequearArchivoMicro(char *);
int objetivoNuevo(char *);



int main(int argc, char * argv[]) {

  if (argc < 2) {
		printf("%s\n", "Debe ingresar el archivo de lenguaje micro.");
		return -1;
	}
  if (argc > 2) {
		printf("%s\n", "No puede ingresar mas de un archivo.");
		return -1;
  	  }

  char * nombreFileIn = argv[1];
  char * nombreFileOut = argv[2];

  if (!chequearArchivoMicro(nombreFileIn)){

	int validacion = objetivoNuevo(nombreFileIn,nombreFileOut);
    	inicializarTablaDeSimbolos();
	if (validacion) {
		return -2;
    	}

    cerrarArchivos()
  }

	return 0;

}

int chequearArchivoMicro(char *archivo) {
  int longitud = strlen(archivo);
  if (archivo[longitud - 2] == '.' && archivo[longitud - 1] == 'm') return 0;
  return -1;
}

int objetivoNuevo(char *nombreFileIn, char *nombreFileOut) {

	int c;
	fileIn = fopen(nombreFileIn,"r");
	if (!fileIn) {
		printf("%s\n", "Error al abrir el archivo de entrada.");
		return 0;
	}
	fileOut = fopen(nombreFileOut,"r+w");
	if (!fileOut) {
		printf("%s\n", "Error al abrir el archivo de salida.");
		return 0;
	}
	
	  return 1;

}

void cerrarArchivos(void) {
	fclose(fileIn);
	fclose(fileOut);
}

void inicializarTablaDeSimbolos(void) {

}
