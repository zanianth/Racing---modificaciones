#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
			//1 box, 1 corredor al tiempo, varios corredores
#define TRUE 1
#define FALSE 0
#define NULO -1

int numCorredoresHastaAhora=0;
int filaBoxes;
struct corredores{
	int identificador;
	int posicionArray;
	int sancionado;
	pthread_t tid;
};
struct corredores misCorredores;
pthread_mutex_t mutex; //utilizado para las señales de sincronización de corredores con boxes y juez
pthread_cond_t condicionReparado; // han entrado los cinco threads
pthread_cond_t condicionEsperaSancion; //esto está remadre
pthread_cond_t condicionSancionCumplida; // han entrado los cinco threads
FILE *LOG;

void procesamiento_SIGUSR1();
void escribeEnLog(char * mensaje, int identificador, char entidad); //Habrá que tener en cuenta si quien llama a la función es un corredor, un box o el juez
int calculaDigitos(int numero);
void nuevoCorredor();
void accionesCorredor();
int accionesBox(int atendidos, int posicion);
void accionesJuez();

void *hiloCorredores(void *ptr){
	int posicion;
	posicion = *(int *)ptr;

	accionesCorredor();
	pthread_exit(NULL);
}

void *hiloBoxes(void *ptr) {
	int posicion, atendidos=0;
	posicion = *(int *)ptr;

	while(TRUE){
