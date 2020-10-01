#include<bits/stdc++.h>
#include<unistd.h>
#include<wait.h>
#include<sys/mman.h>
#include <fcntl.h> 
using namespace std;

// Variables de la configuracion
string version;
int N;
int maxValue;
int numberOfProcess;
int show;

// Entrada del problema
const int maxN = 1000;
int matriz1[maxN][maxN], matriz2[maxN][maxN];

// Datos intermedios
int traspuesta2[maxN][maxN];

// Salida del problema
// Datos en la memoria compartida
struct Data{
	int ans[maxN][maxN];
	Data(){}
};

Data *publicData;


/*** SOLUCION 1															***/
/*** Metodo de asignacion de tareas: Asignacion estatica de filas 		***/
/*** Mecanismo de comunicacion: Memoria compartida						***/
/*** Optimizaciones: ninguna											***/
void initialWorker(int idTask){
	for(int i=idTask; i<N; i+=numberOfProcess){
		for(int j=0; j<N; j++){
			publicData->ans[i][j] = 0;
			for(int l=0; l<N; l++)
			publicData->ans[i][j] += matriz1[i][l] * matriz2[l][j];
		}
	}
}

void initialSolution(){
	// Detectar el pid del proceso, definicion del tamaño de la memoria compartida y el nombre del objeto de memoria compartida
	int rootPid = getpid();	
	int SIZE = 1<<27;	// 128 MB
	char publicDataName[] = "/public_data";

	// Crea el objeto de memoria compartida con permisos de lectura, escritura
	// lo crea si no existe, e inicia con un tamaño de 0 bytes
	int publicDataFD = shm_open(publicDataName, O_RDWR | O_CREAT | O_TRUNC, 0666);

	// Redimensiona el tamaño del espacio de memoria compartida
	ftruncate(publicDataFD, SIZE);

	// Obtiene la direccion al espacio de memoria compartida
	void *publicDataAddr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, publicDataFD, 0);
	publicData = (Data*)publicDataAddr;

	// Crear los procesos y se asigna las filas que deben resolver (asignadas de manera estatica)
	for(int i=0; i<numberOfProcess; i++){
		fork();

		if(getpid() != rootPid){
			void *publicDataAddr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, publicDataFD, 0);
			publicData = (Data*)publicDataAddr;
			initialWorker(i);

			// Elimina las referencias al objeto de memoria compartida y finaliza el proceso
			close(publicDataFD);
			shm_unlink(publicDataName);
			exit(0);
		}
	}

	// Espera a la terminacion de los procesos hijos
	for(int i=0; i<numberOfProcess; i++)
	wait(0);

	// Elimina las referencia al objeto de memoria compartida
	close(publicDataFD);
	shm_unlink(publicDataName);
}



/*** SOLUCION 2															***/
/*** Metodo de asignacion de tareas: Asignacion estatica de filas 		***/
/*** Optimizaciones: 													***/
/***	- Calcular el resultado de las casillas con variables locales 	***/
/***	- Traspuesta para la segunda matriz 							***/
void optimization2Worker(int idTask){
	for(int i=idTask; i<N; i+=numberOfProcess){
		for(int j=0; j<N; j++){
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * traspuesta2[j][l];
			publicData->ans[i][j] = suma;
		}
	}
}

void preprocessing(){
	// Calcular traspuesta dela matriz 2
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[i][j] = matriz2[j][i];
	}
}

void optimization2Solution(){
	// Precalculos realizados antes de hacer una copia de la memoria al crear el resto de procesos
	preprocessing();

	// Detectar el pid del proceso, definicion del tamaño de la memoria compartida y el nombre del objeto de memoria compartida
	int rootPid = getpid();	
	int SIZE = 1<<27;	// 128 MB
	char publicDataName[] = "/public_data";

	// Crea el objeto de memoria compartida con permisos de lectura, escritura
	// lo crea si no existe, e inicia con un tamaño de 0 bytes
	int publicDataFD = shm_open(publicDataName, O_RDWR | O_CREAT | O_TRUNC, 0666);

	// Redimensiona el tamaño del espacio de memoria compartida
	ftruncate(publicDataFD, SIZE);

	// Obtiene la direccion al espacio de memoria compartida
	void *publicDataAddr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, publicDataFD, 0);
	publicData = (Data*)publicDataAddr;

	// Crear los procesos y se asigna las filas que deben resolver (asignadas de manera estatica)
	for(int i=0; i<numberOfProcess; i++){
		fork();

		if(getpid() != rootPid){
			void *publicDataAddr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, publicDataFD, 0);
			publicData = (Data*)publicDataAddr;
			optimization2Worker(i);

			// Elimina las referencias al objeto de memoria compartida y finaliza el proceso
			close(publicDataFD);
			shm_unlink(publicDataName);
			exit(0);
		}
	}

	// Espera a la terminacion de los procesos hijos
	for(int i=0; i<numberOfProcess; i++)
	wait(0);

	// Elimina las referencia al objeto de memoria compartida
	close(publicDataFD);
	shm_unlink(publicDataName);
}



/*** Llena la matriz de forma aleatoria ***/ 
void randomGenerator(int matriz[maxN][maxN]){
	srand(clock());
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		matriz[i][j] = rand() % maxValue;
	}
}

/*** Muestra la matriz ***/
void print(int matriz[maxN][maxN]){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		cout<<matriz[i][j]<<" ";
		cout<<endl;
	} cout<<endl;
}

int main(int argc, char** argv){
	// Configuracion
	version = argv[1];
	N = atoi(argv[2]);
	maxValue = atoi(argv[3]);
	numberOfProcess = atoi(argv[4]);
	show = atoi(argv[5]);

	// Generacion aleatoria de las dos matrices a multiplicar
	randomGenerator(matriz1);
	randomGenerator(matriz2);

	// Multiplicacion de matrices
	if(version == "version1")
	initialSolution();
	else if(version == "version3")
	optimization2Solution();
	else
	cout<<"Error: Nombre de version incorrecto"<<endl;

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(publicData->ans);
	}
}
