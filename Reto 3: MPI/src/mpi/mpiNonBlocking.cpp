#include<bits/stdc++.h>
#include<mpi.h>
using namespace std;
#define endl '\n'

// Definicion de constantes
const int maxN = 1e7;
const int maxIterations = 1e7;

// Variables de la configuracion
int readOfFile;
char* file;
int N;
int iterations;
int numberOfProcess;
int numberOfWorkers;
int show;

// Entrada del problema
int inRoad[maxN];

// Datos intermedios
int L[maxN], R[maxN];
int carCounterArray[maxIterations+1];

// Salida del problema
int outRoad[maxN];
double averageVelocity[maxIterations+1];



// Genera un array de enteros 0s y 1s aleatoriamente
void randomGenerator(){
	for(int i=0; i<N; i++)
	inRoad[i] = rand() % 2;
}


//---- SOLUCION: Operaciones no bloqueantes
//---- (los mensajes no se dividen)
void nonBlockingSolution(int argc, char* argv[]){
	// Identificadores de proceso y definicion de etiquetas de mensajes
	// Se distinguen los tag de izquierda y derecha debido a que si es uno o dos procesos,
	// el mismo proceso le puede dejar 2 mensajes y se debe identificar el
	// lado al que hace referencia el mensaje
	int rank;
	int leftRank;
	int rightRank;
	int leftSendTag = 20001;
	int rightSendTag = 20002;
	int leftReceiveTag = 20002;
	int rightReceiveTag = 20001;
	int subarrayTag = 20003;
	int carCounterArrayTag = 20004;

	// Inicializa el array donde se realizaran todos los cambios en cada iteracion
	// y el array de contadores de carros
	for(int i=0; i<N; i++)
	outRoad[i] = inRoad[i];
	for(int i=0; i<=iterations; i++)
	carCounterArray[i] = 0;

	// Inicia MPI - Se crean los procesos
	// Los datos de este proceso son copiados automaticamente por MPI al resto de procesos
	MPI::Init(argc, argv);

	// Obtiene el rank del proceso actual y los 2 adyacentes, el numero de procesos, y numero de workers
	numberOfProcess = MPI::COMM_WORLD.Get_size();
	numberOfWorkers = min(numberOfProcess, N);
	rank = MPI::COMM_WORLD.Get_rank();
	leftRank = (numberOfWorkers + rank - 1) % numberOfWorkers;
	rightRank = (rank + 1) % numberOfWorkers;

	// Solo se usaran la cantidad de procesos permitidos por el N
	if(rank < numberOfWorkers){
		int bloque = N / numberOfWorkers;

		// El nodo 0 guarda el intervalo que le corresponde a cada nodo
		// para saber que datos debe recibir de cada uno
		if(rank == 0){
			for(int idRank=0,x=0; idRank<numberOfWorkers; idRank++){
				L[idRank] = x;
				x += bloque;
				if(idRank < N % numberOfWorkers)
				x++;
				R[idRank] = x - 1;
			}
		}

		// Se define el intervalo que le corresponde a este proceso
		int ini, fin;
		if(rank < N % numberOfWorkers){
			ini = (bloque + 1) * rank;
			fin = ini + bloque;
		}
		else{
			ini = (bloque * rank) + (N % numberOfWorkers);
			fin = ini + bloque - 1;
		}

		// Cada nodo inicia su trabajo
		for(int iter=1; iter<=iterations; iter++){
			// Copia el primer y ultimo elemento de su intervalo
			int first = outRoad[ini];
			int last = outRoad[fin];
			int firstLeft, firstRigth;

			// Actualiza el intervalo que le corresponde
			// Solo la primera y ultima posicion pueden cambiar
			// con los datos de los otros procesos
			int carCounter = 0;
			for(int i=ini; i<fin; ){
				if(outRoad[i] == 1 && outRoad[i+1] == 0){
					carCounter++;
					outRoad[i] = 0;
					outRoad[i+1] = 1;
					i += 2;
				}
				else
				i++;
			}

			// Envia el primer valor al proceso de la izquierda y el ultimo valor al proceso de la derecha (operaciones no bloqueantes)
			// y se reciben los valores de las 2 celdas adyacentes de los otros procesos
			MPI::Request leftSendRequest = MPI::COMM_WORLD.Isend(&first, 1, MPI::INT, leftRank, leftSendTag);
			MPI::Request rightSendRequest = MPI::COMM_WORLD.Isend(&last, 1, MPI::INT, rightRank, rightSendTag);
			MPI::Request leftRecvRequest = MPI::COMM_WORLD.Irecv(&firstLeft, 1, MPI::INT, leftRank, leftReceiveTag);
			MPI::Request rightRecvRequest = MPI::COMM_WORLD.Irecv(&firstRigth, 1, MPI::INT, rightRank, rightReceiveTag);

			// Bloquea hasta que las operaciones de envio y recepcion se complenten localmente
			leftSendRequest.Wait();
			rightSendRequest.Wait();
			leftRecvRequest.Wait();
			rightRecvRequest.Wait();

			// Actualiza los valores de inicio y fin, de acuerdo con los datos recibidos por los nodos adyacentes
			if(firstLeft == 1 && first == 0){
				carCounter++;
				outRoad[ini] = 1;
			}
			if(last == 1 && firstRigth == 0)
			outRoad[fin] = 0;

			carCounterArray[iter] = carCounter;
		}

		// Los mensajes que cada nodo envia no se dividieron
		// con un N grande o un numero de iteraciones grande hara que estos ultimos mensajes
		// sean muy grandes, por eso los casos se ensayaran con entradas pequeñas
		if(rank != 0){
			// Cada nodo envia su subarray y el numero de autos que avanzaron en cada iteracion
			// al nodo 0 y esperan hasta que se completen las operaciones de envio localmente
			MPI::Request subarrayRequest = MPI::COMM_WORLD.Isend(&outRoad[ini], fin + 1 - ini, MPI::INT, 0, subarrayTag);
			MPI::Request carCounterArrayRequest = MPI::COMM_WORLD.Isend(carCounterArray, iterations+1, MPI::INT, 0, carCounterArrayTag);
			subarrayRequest.Wait();
			carCounterArrayRequest.Wait();
		}
		else{
			// El nodo 0 recibe los subarrays de cada nodo
			MPI::Request subarrayRequests[numberOfWorkers];
			MPI::Request carCounterArrayRequests[numberOfWorkers];
			for(int idRank=1; idRank<numberOfWorkers; idRank++)
			subarrayRequests[idRank] = MPI::COMM_WORLD.Irecv(&outRoad[L[idRank]], R[idRank] + 1 - L[idRank], MPI::INT, idRank, subarrayTag);
			
			// El nodo 0 recibe la informacion sobre el numero de carros que avanzo en cada iteracion para cada uno de los nodos
			int carCounterArrayTemp[numberOfWorkers][iterations+1];
			for(int idRank=1; idRank<numberOfWorkers; idRank++)
			carCounterArrayRequests[idRank] = MPI::COMM_WORLD.Irecv(carCounterArrayTemp[idRank], iterations + 1, MPI::INT, idRank, carCounterArrayTag);

			// Se espera hasta que reciba todos los mensajes que espera de cada proceso
			for(int idRank=1; idRank<numberOfWorkers; idRank++)
			subarrayRequests[idRank].Wait();
			for(int idRank=1; idRank<numberOfWorkers; idRank++)
			carCounterArrayRequests[idRank].Wait();

			// Suma los contadores de carros que avanzaron en cada iteracion retornados por cada uno de los nodos
			// y obtiene la velocidad promedio de cada iteracion
			for(int idRank=1; idRank<numberOfWorkers; idRank++){
				for(int iter=0; iter<=iterations; iter++)
				carCounterArray[iter] += carCounterArrayTemp[idRank][iter];
			}
			for(int iter=0; iter<=iterations; iter++)
			averageVelocity[iter] = (double)carCounterArray[iter] / N;
		}
	}

	// Tiene una barrera de sincronizacion implicita
	// Se cierra la comunicacion entre los procesos
	MPI::Finalize();

	// Finaliza todos los procesos, excepto el proceso inicial
	if(rank != 0)
	exit(0);
}



int main(int argc, char* argv[]){
	// Configuracion
	readOfFile = atoi(argv[1]);
	if(readOfFile)
	file = argv[2];
	else
	N = atoi(argv[2]);
	iterations = atoi(argv[3]);
	show = atoi(argv[4]);

	// Obtener datos de entrada
	if(readOfFile){
		freopen(file, "r", stdin);
		cin>>N;
		for(int i=0; i<N; i++)
		cin>>inRoad[i];
	}
	else{
		// Inicializar variables
		srand(time(NULL));
		// Generacion aleatoria del array inicial
		randomGenerator();
	}

	// Automata celular
	nonBlockingSolution(argc, argv);

	// Mostrar el array de entrada, el array final y el promedio de velocidad para cada iteracion
	if(show){
		cout<<"ARRAY INICIAL:"<<endl;
		for(int i=0; i<N; i++)
		cout<<inRoad[i]<<" ";
		cout<<endl;

		cout<<"\nARRAY FINAL:"<<endl;
		for(int i=0; i<N; i++)
		cout<<outRoad[i]<<" ";
		cout<<endl;
	
		cout<<"\nVELOCIDAD PROMEDIO EN CADA ITERACION:"<<endl;
		for(int iter=1; iter <= iterations; iter++)
		cout<<"Iteracion: "<<iter<<"\t\t"<<averageVelocity[iter]<<endl;
	}
}