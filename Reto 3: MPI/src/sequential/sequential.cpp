#include<bits/stdc++.h>
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
int show;

// Datos de entrada
int inRoad[maxN];

// Salida del problema
int outRoad[maxN];
double averageVelocity[maxIterations+1];



// Genera un array de enteros 0s y 1s aleatoriamente
void randomGenerator(){
	for(int i=0; i<N; i++)
	inRoad[i] = rand() % 2;
}


//---- SOLUCION
//---- Llevar un iterador que determine si un carro avanza o no
//---- Si el carro avanza, modifica la celda actual y la siguiente, ademas el iterador avanza 2 posiciones
//---- De lo contrario sigue con la siguiente celda
//---- Determina si intercambia la primer y ultima celda
void sequentialSolution(){
	// Inicializa el array donde se realizaran todos los cambios en cada iteracion
	for(int i=0; i<N; i++)
	outRoad[i] = inRoad[i];

	for(int iter=1; iter <= iterations; iter++){
		int first, last;
		first = outRoad[0];
		last = outRoad[N-1];

		int carCounter = 0;
		for(int i=0; i<N-1; ){
			if(outRoad[i] == 1 && outRoad[i+1] == 0){
				carCounter++;
				outRoad[i] = 0;
				outRoad[i+1] = 1;
				i += 2;
			}
			else
			i++;
		}
		if(last == 1 && first == 0){
			carCounter++;
			outRoad[0] = 1;
			outRoad[N-1] = 0;
		}

		averageVelocity[iter] = (double)carCounter / N;
	}
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
	sequentialSolution();

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