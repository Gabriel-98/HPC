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


//---- SOLUCION: Montecarlo - Needles
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Bloqueos o sincronizacion (datos): No
//---- Caracteristicas:
//----		- El valor de Y se genera con 4 decimales en el intervalo [0,1]
//----		- El angulo se genera con 4 decimales en el intervalo [0,20]
//----		- Numero de iteraciones de la serie de taylor: 50
//----		- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
double taylorSin(double x){
	double ans = x, term = x;
	for(int i=1; i<=50; i++){
		term *= -(x * x);
		term /= (2*i) * (2*i + 1);
		ans += term;
	}
	return ans;
}

void needlesWorker(int id, int numberOfTest){
	int hitsLocal = 0;
	RandomGenerator randomGenerator;
	for(int i=0; i<numberOfTest; i++){
		double y, h, ang, y1, y2;
		y = (randomGenerator.random() % 10001) / 10000.0;
		ang = (randomGenerator.random() % 200001) / 10000.0;
		h = abs(taylorSin(ang)) / 2.0;
		y1 = y - h;
		y2 = y + h;

		if(y1 <= 0 || y2 >= 1)
		hitsLocal++;
	}
	hitsByThread[id] = hitsLocal;
}

void needlesThreads(){
	// Inicializar variables y estructuras
	hitsByThread = (int*)malloc(sizeof(int)*numberOfThreads);

	// Creacion de hilos y asignacion de tareas (numero de pruebas)
	int numberOfTestByThread = N / numberOfThreads;
	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++){
		if(i < (N % numberOfThreads))
		threads[i] = thread(needlesWorker, i, numberOfTestByThread + 1);
		else
		threads[i] = thread(needlesWorker, i, numberOfTestByThread);
	}

	// Espera a la finalizacion de los hilos
	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();

	// Determinar el valor de pi
	hits = 0;
	for(int i=0; i<numberOfThreads; i++)
	hits += hitsByThread[i];
	pi = 2.0 * N / hits;
}

int main(int argc, char** argv){
	// Configuracion
	N = atoi(argv[1]);
	numberOfThreads = atoi(argv[2]);
	show = atoi(argv[3]);

	// Calculo del numero pi
	needlesThreads();

	// Mostrar el resultado
	if(show){
		cout<<"Numero de intentos: "<<N<<endl;
		cout<<"Numero de aciertos: "<<hits<<endl;
		cout<<"Numero pi: "<<fixed<<setprecision(6)<<pi<<endl;
	}
}