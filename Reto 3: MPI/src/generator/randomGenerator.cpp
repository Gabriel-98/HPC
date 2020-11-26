#include<bits/stdc++.h>
using namespace std;
#define endl '\n'

// Datos de entrada
int N;

// Genera un array de forma aleatoria de 0s y 1s y lo muestra
void randomGeneratorAndShow(){
	cout<<N<<endl;
	for(int i=0; i<N; i++){
		int x = rand() % 2;
		if(i > 0)
		cout<<" ";
		cout<<x;
	} cout<<endl;
}

int main(int argc, char* argv[]){
	// Lee los datos de entrada
	N = atoi(argv[1]);

	// Inicializar variables
	srand(time(NULL));

	// Genera el array de 0s y 1s y lo muestra
	randomGeneratorAndShow();
}