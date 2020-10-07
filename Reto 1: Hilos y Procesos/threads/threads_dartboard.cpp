#include<bits/stdc++.h>
using namespace std;

// Variables de la configuracion
int N;
int numberOfThreads;
int show;

// Datos intermedios
int* hitsByThread;

// Salida del problema
int hits;
double pi;


// Estructura para generar numeros aleatorios
struct RandomGenerator{
	long long a = 6542315, b = 5617565, mod = 1e9 + 7;
	long long x;

	RandomGenerator() : x(time(NULL) % mod) {}
	int random(){
		x = (a * x + b) % mod;
		return x;
	}
};


//---- SOLUCION: Montecarlo - Dartboard
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Bloqueos o sincronizacion (datos): No
//---- Caracteristicas:
//----		- Los valores de X,Y se generan con 4 decimales en el intervalo [-1,1]
//----		- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
void dartboardWorker(int id, int numberOfTest){
	int hitsLocal = 0;
	RandomGenerator randomGenerator;
	for(int i=0; i<numberOfTest; i++){
		double x = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
		double y = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
		if(x*x + y*y <= 1.0)
		hitsLocal++;
	}
	hitsByThread[id] = hitsLocal;
}

void dartboardThreads(){
	// Inicializar variables y estructuras
	hitsByThread = (int*)malloc(sizeof(int)*numberOfThreads);

	// Creacion de hilos y asignacion de tareas (numero de pruebas)
	int numberOfTestByThread = N / numberOfThreads;
	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++){
		if(i < (N % numberOfThreads))
		threads[i] = thread(dartboardWorker, i, numberOfTestByThread + 1);
		else
		threads[i] = thread(dartboardWorker, i, numberOfTestByThread);
	}

	// Espera a la finalizacion de los hilos
	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();

	// Determinar el valor de pi
	hits = 0;
	for(int i=0; i<numberOfThreads; i++)
	hits += hitsByThread[i];
	pi = (4.0 * hits) / N;
}

int main(int argc, char** argv){
	// Configuracion
	N = atoi(argv[1]);
	numberOfThreads = atoi(argv[2]);
	show = atoi(argv[3]);

	// Calculo del numero pi
	dartboardThreads();

	// Mostrar el resultado
	if(show){
		cout<<"Numero de intentos: "<<N<<endl;
		cout<<"Numero de aciertos: "<<hits<<endl;
		cout<<"Numero pi: "<<fixed<<setprecision(6)<<pi<<endl;
	}
}