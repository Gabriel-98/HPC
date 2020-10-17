#include<bits/stdc++.h>
#include <stdio.h>
#include <omp.h>
using namespace std;

// Variables de la configuracion
string version;
int N;
int maxValue;
int numberOfThreads;
int blockSize;
int show;

// Entrada del problema
const int maxN = 2000;
int matriz1[maxN][maxN];
int matriz2[maxN][maxN];

// Datos intermedios
int traspuesta2[maxN][maxN];

// Salida del problema
int ans[maxN][maxN];

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


//---- SOLUCION 1
//---- Tarea: Fila
//---- Estrategia de scheduling: Estatica
//---- Tamaño del bloque: 1
//---- Sincronizacion (Datos): No
void staticRowSolution(){
	omp_set_nested(1);
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[j][i] = matriz2[i][j];
	}
	#pragma omp parallel shared(matriz1, matriz2, traspuesta2, ans) num_threads(numberOfThreads)
	{
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++){
			for(int j=0; j<N; j++){
				int suma = 0;
				for(int l=0; l<N; l++)
				suma += matriz1[i][l] * traspuesta2[j][l];
				ans[i][j] = suma;
			}
		}
	}
}


//---- SOLUCION 2
//---- Tarea: Casilla
//---- Estrategia de scheduling: Estatica
//---- Tamaño del bloque: Parametro
//---- Sincronizacion: Barrera implicita de parallel y for
//---- Sincronizacion (Datos): No
void staticBoxSolution(){
	omp_set_nested(1);
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[j][i] = matriz2[i][j];
	}
	#pragma omp parallel shared(matriz1, matriz2, traspuesta2, ans) num_threads(numberOfThreads)
	{
		#pragma omp for schedule(static,blockSize)
		for(int box=0; box<N*N; box++){
			int i,j;
			i = box / N;
			j = box % N;
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * traspuesta2[j][l];
			ans[i][j] = suma;
		}	
	}
}


//---- SOLUCION 3
//---- Tarea: Casilla
//---- Estrategia de scheduling: Dinamica
//---- Tamaño del bloque: Parametro
//---- Sincronizacion:
//----     - Barrera implicita de parallel y for
//----     - Al distribuir de forma dinamica las iteraciones
//---- Sincronizacion (Datos): No
void dynamicBoxSolution(){
	omp_set_nested(1);
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[j][i] = matriz2[i][j];
	}
	#pragma omp parallel shared(matriz1, matriz2, traspuesta2, ans) num_threads(numberOfThreads)
	{
		#pragma omp for schedule(dynamic,blockSize)
		for(int box=0; box<N*N; box++){
			int i,j;
			i = box / N;
			j = box % N;
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * traspuesta2[j][l];
			ans[i][j] = suma;
		}	
	}
}

int main(int argc, char* argv[]){
	// Configuracion
	version = argv[1];
	if(version == "staticRow"){
		N = atoi(argv[2]);
		maxValue = atoi(argv[3]);
		numberOfThreads = atoi(argv[4]);
		blockSize = 1;
		show = atoi(argv[5]);
	}
	else if(version == "staticBox" || version == "dynamicBox"){
		N = atoi(argv[2]);
		maxValue = atoi(argv[3]);
		numberOfThreads = atoi(argv[4]);
		blockSize = atoi(argv[5]);
		show = atoi(argv[6]);
	}

	// Inicializacion de variables
	srand(time(NULL));

	// Generacion aleatoria de las dos matrices a multiplicar
	randomGenerator(matriz1);
	randomGenerator(matriz2);

	// Multiplicacion de matrices
	if(version == "staticRow")
	staticRowSolution();
	else if(version == "staticBox")
	staticBoxSolution();
	else if(version == "dynamicBox")
	dynamicBoxSolution();
	else
	cout<<"Error! Nombre de version incorrecto"<<endl;

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(ans);
	}
}