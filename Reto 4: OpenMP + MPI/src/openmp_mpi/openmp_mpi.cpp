#include<bits/stdc++.h>
#include<mpi.h>
#include<omp.h>
using namespace std;
#define endl '\n'

// Variables de la configuracion
int N;
int maxValue;
int numberOfProcess;
int numberOfWorkers;
int numberOfThreadsByWorker;
int numberOfWorkerThreads;
int show;

// Entrada del problema
const int maxN = 5000;
int matriz1[maxN][maxN];
int matriz2[maxN][maxN];

// Datos intermedios
int traspuesta2[maxN][maxN];

// Salida del problema
int ans[maxN][maxN];


//---- SOLUCION: OpenMP + MPI
//---- Tipo de comunicacion: no bloqueante
//---- Funciones de los hilos: 
//----     - El hilo del proceso inicial:
//----          - Envia la traspuesta de la segunda matriz a todos los procesos
//----          - Envia solo las filas de la matriz1 al hilo que le corresponde
//---- 	        - Recibe la matriz resultado
//----     - Los hilos del resto de procesos son los worker y hacen lo siguiente:
//----          - Reciben la traspuesta de la segunda matriz
//----          - A medida que va recibiendo una fila de la matriz1, genera la respuesta y la envia
//---- Caracteristicas del algoritmo:
//----     - Variable local para el calculo de cada casilla
//----     - Uso de la matriz traspuesta para la segunda matriz
//---- Identificacion de mensajes:
//----     - A cada hilo worker le corresponde un tag, y ese tag identifica un subconjunto de las filas de la matriz
void openMPWithMPISolution(int rank){
	if(rank == 0){
		// Calcula la traspuesta para la segunda matriz
		for(int i=0; i<N; i++){
			for(int j=0; j<N; j++)
			traspuesta2[i][j] = matriz2[j][i];
		}

		// Se definen los arrays que almacenaran las requests asociadas a los mensajes no bloqueantes
		MPI::Request recvRequests[N];
		MPI::Request sendRequests[N*numberOfProcess];
		
		// ---- ENVIA LA TRASPUESTA2
		// Envia la traspuesta de la segunda matriz (usando send en modo estandar no bloqueante)
		for(int i=0,requestId=0; i<N; i++){
			for(int idRank=1; idRank < numberOfProcess; idRank++,requestId++)
			sendRequests[requestId] = MPI::COMM_WORLD.Isend(traspuesta2[i], N, MPI::INT, idRank, idRank);
		}
		for(int i=0; i<N*(numberOfProcess-1); i++)
		sendRequests[i].Wait();

		
		// ---- ENVIA LA MATRIZ1 Y RECIBE LA MATRIZ RESULTADO
		// A medida que va enviando una fila de la matriz1, indica que va a recibir el
		// resultado para esa fila, ambas operaciones en modo no bloqueante.
		// por medio del tag indica que va a enviar o recibir la proxima fila que le corresponde
		// a un determinado hilo worker
		// Al final espera hasta que todas las operaciones de envio y de recepcion se completen
		// Es decir continua cuando ya tiene las respuestas de todas las filas		
		for(int i=0; i<N; i++){
			int threadTag = i % numberOfWorkerThreads;
			int idRank = (threadTag / numberOfThreadsByWorker) + 1;
			sendRequests[i] = MPI::COMM_WORLD.Isend(matriz1[i], N, MPI::INT, idRank, threadTag);
			recvRequests[i] = MPI::COMM_WORLD.Irecv(ans[i], N, MPI::INT, idRank, threadTag);
		}
		for(int i=0; i<N; i++){
			sendRequests[i].Wait();
			recvRequests[i].Wait();
		}
	}
	else{
		// Se definen los arrays que almacenaran las requests asociadas a los mensajes no bloqueantes
		MPI::Request recvRequests[N];
		MPI::Request sendRequests[N];

		// ---- RECIBE LA TRASPUESTA2
		// Recibe la traspuesta de la segunda matriz (usando recv no bloqueante)
		for(int i=0; i<N; i++)
		recvRequests[i] = MPI::COMM_WORLD.Irecv(traspuesta2[i], N, MPI::INT, 0, rank);
		for(int i=0; i<N; i++)
		recvRequests[i].Wait();

		// A cada hilo worker se le genera un tag que sirve para identificar sus mensajes
		// (tanto los mensajes que envia como los que recibe),
		// Cada hilo worker recibe las filas, las resuelve y envia el resultados de
		// esas filas, esas filas le corresponden de acuerdo a su tag
		// El tag permite identificar ademas todas las filas que le corresponden a cada hilo
		// Primero indica que va a recibir todas las filas que le correspoden de la amatriz1
		// A medida que obtiene una fila de la matriz1, la resuelve y se la envia al proceso con rank 0
		#pragma omp parallel num_threads(numberOfThreadsByWorker)
		{
			int threadId = omp_get_thread_num();
			int threadTag = ((rank - 1) * numberOfThreadsByWorker) + threadId;

			for(int i=threadTag; i<N; i+=numberOfWorkerThreads){
				#pragma omp critical (r1)
				{
					recvRequests[i] = MPI::COMM_WORLD.Irecv(matriz1[i], N, MPI::INT, 0, threadTag);
				}
			}
			for(int i=threadTag; i<N; i+=numberOfWorkerThreads){
				#pragma omp critical (r1)
				{
					recvRequests[i].Wait();
				}
				for(int j=0; j<N; j++){
					int suma = 0;
					for(int l=0; l<N; l++)
					suma += matriz1[i][l] * traspuesta2[j][l];
					ans[i][j] = suma;
				}
				#pragma omp critical (r1)
				{
					sendRequests[i] = MPI::COMM_WORLD.Isend(ans[i], N, MPI::INT, 0, threadTag);
				}
			}
		}
		for(int i=rank-1; i<N; i+=numberOfWorkers)
		sendRequests[i].Wait();
	}
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
	numberOfThreadsByWorker = atoi(argv[3]);
	show = atoi(argv[4]);

	// Inicia MPI - Se crean los procesos
	MPI::Init(argc, argv);

	// Obtiene el rank del proceso actual, el numero de procesos, numero de workers y numero total de hilos worker
	int rank = MPI::COMM_WORLD.Get_rank();
	numberOfProcess = MPI::COMM_WORLD.Get_size();
	numberOfWorkers = numberOfProcess - 1;
	numberOfWorkerThreads = numberOfWorkers * numberOfThreadsByWorker;

	if(rank == 0){
		// Inicializacion de variables
		srand(clock() % 1000);

		// Generacion aleatoria de las dos matrices a multiplicar
		randomGenerator(matriz1);
		randomGenerator(matriz2);
	}

	// Multiplicacion de matrices
	openMPWithMPISolution(rank);

	// Tiene una barrera de sincronizacion implicita
	// Se cierra la comunicacion entre los procesos
	MPI::Finalize();

	// Finaliza todos los procesos, excepto el proceso inicial
	if(rank != 0)
	exit(0);

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(ans);
	}
}