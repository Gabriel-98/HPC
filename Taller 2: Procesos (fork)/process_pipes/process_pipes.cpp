#include<bits/stdc++.h>
#include<wait.h>
#include "channel.h"
using namespace std;

int N;
int maxValue;
int numberOfProcess;
int show;

const int maxN = 5000;
int matriz1[maxN][maxN], matriz2[maxN][maxN], ans[maxN][maxN];


void solver(Channel channel, int id){
	// cierra los descriptores de archivo que no necesita
	channel.closeChildPipePrivateFD();

	// Calcula los resultados para las filas asignadas
	// y cada fila que se va obteniendo como resultado la manda al
	// proceso coordinador
	int temp[N];
	for(int i=id; i<N; i+=numberOfProcess){
		for(int j=0; j<N; j++){
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * matriz2[l][j];
			temp[j] = suma;
		}
		channel.writeIntArray(N, temp);
	}

	// Cierra las tuberias que esta usando
	channel.closeChannel();
	exit(0);
}

void pipeSolution(int N, int maxValue, int numberOfProcess){
	int rootPid = getpid();

	// Se crean las tuberias para comunicar a los procesos
	Channel channels[numberOfProcess];
	for(int i=0; i<numberOfProcess; i++)
	channels[i] = Channel();

	// Se crean los procesos y se asigna de forma estatica las tareas
	// que les corresponden (un identificador para el conjunto de filas asignadas)
	for(int i=0; i<numberOfProcess; i++){
		if(getpid() == rootPid){
			int childPid = fork();
			if(childPid == 0)
			solver(channels[i], i);
		}
	}

	// El proceso padre  (coordinador) obtiene los resultados que le envian los procesos hijos
	// Como las filas se asignaron de forma intercalada, va intercalando el proceso del cual va leer
	if(getpid() == rootPid){
		for(int row=0,process=0; row < N; row++,process++){
			if(process == numberOfProcess)
			process = 0;

			int* v = channels[process].readIntArray(N);
			for(int j=0; j<N; j++)
			ans[row][j] = v[j];	
		}
	}

	// Espera a que todos los procesos hijos finalicen
	for(int i=0; i<numberOfProcess; i++)
	wait(0);

	// Cierra las tuberias
	for(int i=0; i<numberOfProcess; i++)
	channels[i].closeChannel();
}

void randomGenerator(int matriz[maxN][maxN]){
	srand(clock());
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		matriz[i][j] = rand() % maxValue;
	}
}

void print(int matriz[maxN][maxN]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		cout<<matriz[i][j]<<" ";
		cout<<endl;
	} cout<<endl;
}


int main(int argc, char** argv){
	// Configuracion
	N = atoi(argv[1]);
	maxValue = atoi(argv[2]);
	numberOfProcess = atoi(argv[3]);
	show = atoi(argv[4]);

	// Generacion aleatoria de las dos matrices a multiplicar
	randomGenerator(matriz1);
	randomGenerator(matriz2);

	// Multiplicacion de matrices
	pipeSolution(N, maxValue, numberOfProcess);

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(ans);
	}
}