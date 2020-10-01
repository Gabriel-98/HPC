#include<bits/stdc++.h>
using namespace std;

// Variables de la configuracion
string version;
int N;
int maxValue;
int numberOfThreads;
int show;

// Entrada del problema
const int maxN = 1000;
int matriz1[maxN][maxN], matriz2[maxN][maxN];

// Datos intermedios
int traspuesta2[maxN][maxN];

// Salida del problema
int ans[maxN][maxN];


/*** SOLUCION 1															***/
/*** Metodo de asignacion de tareas: Asignacion estatica de filas 		***/
/*** Optimizaciones: ninguna											***/
void initalWorker(int idTask){
	for(int i=idTask; i<N; i+=numberOfThreads){
		for(int j=0; j<N; j++){
			ans[i][j] = 0;
			for(int l=0; l<N; l++)
			ans[i][j] += matriz1[i][l] * matriz2[l][j];
		}
	}
}

void initialSolution(){
	// Creacion de hilos y asignacion de tareas (filas)
	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++)
	threads[i] = thread(initalWorker, i);

	// Espera a la finalizacion de los hilos
	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();
}

/*** SOLUCION 2															***/
/*** Metodo de asignacion de tareas: Asignacion estatica de filas 		***/
/*** Optimizaciones: 													***/
/***	- Calcular el resultado de las casillas con variables locales 	***/
/***	- Traspuesta para la segunda matriz 							***/
void optimization2Worker(int idTask){
	for(int i=idTask; i<N; i+=numberOfThreads){
		for(int j=0; j<N; j++){
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * traspuesta2[j][l];
			ans[i][j] = suma;
		}
	}
}

void optimization2Solution(){
	// Calculo de la traspuesta de la segunda matriz
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[i][j] = matriz2[j][i];
	}

	// Creacion de hilos y asignacion de tareas (filas)
	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++)
	threads[i] = thread(optimization2Worker, i);

	// Espera a la finalizacion de los hilos
	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();
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
	numberOfThreads = atoi(argv[4]);
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
		print(ans);
	}
}
