#include<bits/stdc++.h>
using namespace std;

// Variables de la configuracion
int N;
int maxValue;
int show;

// Entrada del problema
const int maxN = 2000;
int matriz1[maxN][maxN];
int matriz2[maxN][maxN];

// Datos intermedios
int traspuesta2[maxN][maxN];

// Salida del problema
int ans[maxN][maxN];


//---- SOLUCION: Secuencial
//---- Caracteristicas del algoritmo:
//----     - Variable local para el calculo de cada casilla
//----     - Uso de la matriz traspuesta para la segunda matriz
void sequentialSolution(){
	// Calculo de la traspuesta para la segunda matriz
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		traspuesta2[i][j] = matriz2[j][i];
	}

	// filas x filas
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * traspuesta2[j][l];
			ans[i][j] = suma;
		}
	}
}


// Llena una matriz de forma aleatoria
void randomGenerator(int matriz[maxN][maxN]){
	srand(clock());
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

int main(int argc, char	**argv){
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
	sequentialSolution();

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(ans);
	}
}