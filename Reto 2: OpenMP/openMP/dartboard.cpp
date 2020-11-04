#include<bits/stdc++.h>
using namespace std;

// Variables de la configuracion
string version;
int N;
int numberOfThreads;
int show;

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


//---- Caracteristicas para realizar las pruebas con el metodo de dartboard:
//----		- Tipo de dato: double
//----		- Los valores de X,Y se generan con 4 decimales en el intervalo [-1,1]
int dartboardTest(RandomGenerator &randomGenerator){
	double x = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
	double y = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
	if(x*x + y*y <= 1.0)
	return 1;
	return 0;
}


//---- SOLUCION: Montecarlo - Dartboard
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
//---- Mecanismo de actualizacion: Region critica
void dartboardCritical(){
	hits = 0;
	#pragma omp parallel num_threads(numberOfThreads)
	{
		RandomGenerator randomGenerator;
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++){
			#pragma omp critical
			hits += dartboardTest(randomGenerator);
		}
	}
	pi = (4.0 * hits) / N;
}

//---- SOLUCION: Montecarlo - Dartboard
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
//---- Mecanismo de actualizacion: Operacion atomica
void dartboardAtomic(){
	hits = 0;
	#pragma omp parallel num_threads(numberOfThreads)
	{
		RandomGenerator randomGenerator;
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++){
			#pragma omp atomic
			hits += dartboardTest(randomGenerator);
		}
	}
	pi = (4.0 * hits) / N;
}

//---- SOLUCION: Montecarlo - Dartboard
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
//---- Mecanismo de actualizacion: Reduccion
void dartboardReduction(){
	hits = 0;
	#pragma omp parallel num_threads(numberOfThreads) reduction(+: hits)
	{
		RandomGenerator randomGenerator;
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++)
		hits += dartboardTest(randomGenerator);
	}
	pi = (4.0 * hits) / N;
}

int main(int argc, char** argv){
	// Configuracion
	version = argv[1];
	N = atoi(argv[2]);
	numberOfThreads = atoi(argv[3]);
	show = atoi(argv[4]);

	// Calculo del numero pi
	if(version == "critical")
	dartboardCritical();
	else if(version == "atomic")
	dartboardAtomic();
	else if(version == "reduction")
	dartboardReduction();
	else
	cout<<"Error! Nombre de version incorrecto"<<endl;

	// Mostrar el resultado
	if(show){
		cout<<"Numero de intentos: "<<N<<endl;
		cout<<"Numero de aciertos: "<<hits<<endl;
		cout<<"Numero pi: "<<fixed<<setprecision(6)<<pi<<endl;
	}
}