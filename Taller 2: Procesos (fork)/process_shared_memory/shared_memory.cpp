#include<bits/stdc++.h>
#include<unistd.h>
#include<wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
using namespace std;
#define endl '\n'

int N;
int maxValue;
int numberOfProcess;
int show;

const int maxN = 5000;

int matriz1[maxN][maxN], matriz2[maxN][maxN];


// Datos en la memoria compartida
struct Data{
	int ans[maxN][maxN];
	Data(){}
};

Data *publicData;

void solverRows(int id){
	for(int i=id; i<N; i+=numberOfProcess){
		for(int j=0; j<N; j++){
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * matriz2[l][j]; 
			publicData->ans[i][j] = suma;
		}
	}
}

void sharedMemorySolution(){
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
			solverRows(i);

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

int main(int argc, char	**argv){
	// Configuracion
	N = atoi(argv[1]);
	maxValue = atoi(argv[2]);
	numberOfProcess = atoi(argv[3]);
	show = atoi(argv[4]);

	// Generacion aleatoria de las dos matrices a multiplicar
	randomGenerator(matriz1);
	randomGenerator(matriz2);
	
	// Multiplicacion de matrices
	sharedMemorySolution();

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(publicData->ans);
	}
}