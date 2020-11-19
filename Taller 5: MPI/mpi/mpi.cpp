#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;
#define endl '\n'

// Variables de la configuracion
int N;
int maxValue;
int numberOfProcess;
int numberOfWorkers;
int show;

// Entrada del problema
const int maxN = 2000;
int matriz1[maxN][maxN];
int matriz2[maxN][maxN];

// Datos intermedios
int traspuesta2[maxN][maxN];

// Salida del problema
int ans[maxN][maxN];


//---- SOLUCION: MPI
//---- Tipo de comunicacion: no bloqueante
//---- Funciones de los procesos: 
//----     - El proceso inicial solo recibe las respuestas
//----     - Los demas procesos son los worker
//---- Caracteristicas del algoritmo:
//----     - Variable local para el calculo de cada casilla
//----     - Uso de la matriz traspuesta para la segunda matriz
void mpiSolution(int argc, char* argv[]){
	// Definicion del identificador de proceso y etiquetas de mensajes
	int rank;
	int resultRowTag = 20000;

	// Calculo de la traspuesta para la segunda matriz
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[i][j] = matriz2[j][i];
	}

	// Inicia MPI - Se crean los procesos
	// Los datos de este proceso son copiados automaticamente por MPI al resto de procesos
	MPI::Init(argc, argv);

	// Obtiene el rank del proceso actual, el numero de procesos, y numero de workers
	rank = MPI::COMM_WORLD.Get_rank();
	numberOfProcess = MPI::COMM_WORLD.Get_size();
	numberOfWorkers = numberOfProcess - 1;

	if(rank == 0){
		// El proceso inicial indica que va recibir la respuesta de cada fila
		// de un determinado proceso (con un receive no bloqueante)
		// Espera hasta que tenga la respuesta de todas las filas
		MPI::Request requests[N];
		for(int i=0, idWorker=0; i<N; i++, idWorker=(idWorker+1)%numberOfWorkers)
		requests[i] = MPI::COMM_WORLD.Irecv(ans[i], N, MPI::INT, idWorker+1, resultRowTag);
		for(int i=0; i<N; i++)
		requests[i].Wait();
	}
	else{
		// Cada uno de los procesos worker resuelve y envia las filas que
		// le corresponden de acuerdo a su rank y se las envia al proceso inicial
		for(int i=rank-1; i<N; i+=numberOfWorkers){
			for(int j=0; j<N; j++){
				int suma = 0;
				for(int l=0; l<N; l++)
				suma += matriz1[i][l] * traspuesta2[j][l];
				ans[i][j] = suma;
			}
			MPI::COMM_WORLD.Isend(ans[i], N, MPI::INT, 0, resultRowTag);
		}
	}

	// Tiene una barrera de sincronizacion implicita
	// Se cierra la comunicacion entre los procesos
	MPI::Finalize();

	// Finaliza todos los procesos, excepto el proceso inicial
	if(rank != 0)
	exit(0);
}


// Llena la matriz de forma aleatoria
void randomGenerator(int matriz[maxN][maxN]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		matriz[i][j] = rand() % maxValue;
	}
}

// Muestra la matriz
void print(int matriz[maxN][maxN]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		cout<<matriz[i][j]<<" ";
		cout<<endl;
	} cout<<endl;
}


int main(int argc, char* argv[]){
	// Configuracion
	N = atoi(argv[1]);
	maxValue = atoi(argv[2]);
	show = atoi(argv[3]);

	// Inicializacion de variables
	srand(time(NULL));

	// Generacion aleatoria de las dos matrices a multiplicar
	randomGenerator(matriz1);
	randomGenerator(matriz2);

	// Multiplicacion de matrices
	mpiSolution(argc, argv);

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(ans);
	}
}