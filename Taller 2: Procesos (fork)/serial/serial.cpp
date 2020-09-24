#include<bits/stdc++.h>
using namespace std;

int N;
int maxValue;
int show;

const int maxN = 5000;

int matriz1[maxN][maxN], matriz2[maxN][maxN], ans[maxN][maxN];

void serialSolution(){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++){
			int suma = 0;
			for(int l=0; l<N; l++)
			suma += matriz1[i][l] * matriz2[l][j];
			ans[i][j] = suma;
		}
	}
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
	show = atoi(argv[3]);

	// Generacion aleatoria de las dos matrices a multiplicar
	randomGenerator(matriz1);
	randomGenerator(matriz2);

	// Multiplicacion de matrices
	serialSolution();

	// Mostrar las matrices
	if(show){
		print(matriz1);
		print(matriz2);
		print(ans);
	}
}