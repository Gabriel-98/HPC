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


// Calculo de la funcion sin(x) usando la serie de taylor
double taylorSin(double x){
	double ans = x, term = x;
	for(int i=1; i<=50; i++){
		term *= -(x * x);
		term /= (2*i) * (2*i + 1);
		ans += term;
	}
	return ans;
}


//---- Caracteristicas para realizar las pruebas con el metodo de needles:
//----		- Tipo de dato: double
//----		- El valor de Y se genera con 4 decimales en el intervalo [0,1]
//----		- El angulo se genera con 4 decimales en el intervalo [0,20]
//----		- Numero de iteraciones de la serie de taylor: 50
int needlesTest(RandomGenerator &randomGenerator){
	double y, h, ang, y1, y2;
	y = (randomGenerator.random() % 10001) / 10000.0;
	ang = (randomGenerator.random() % 200001) / 10000.0;
	h = abs(taylorSin(ang)) / 2.0;
	y1 = y - h;
	y2 = y + h;

	if(y1 <= 0 || y2 >= 1)
	return 1;
	return 0;
}



//---- SOLUCION: Montecarlo - Needles
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
//---- Mecanismo de actualizacion: Region critica
void needlesCritical(){
	hits = 0;
	#pragma omp parallel num_threads(numberOfThreads)
	{
		RandomGenerator randomGenerator;
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++){
			#pragma omp critical
			hits += needlesTest(randomGenerator);
		}
	}
	pi = 2.0 * N / hits;
}

//---- SOLUCION: Montecarlo - Needles
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
//---- Mecanismo de actualizacion: Operacion atomica
void needlesAtomic(){
	hits = 0;
	#pragma omp parallel num_threads(numberOfThreads)
	{
		RandomGenerator randomGenerator;
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++){
			#pragma omp atomic
			hits += needlesTest(randomGenerator);
		}
	}
	pi = 2.0 * N / hits;
}

//---- SOLUCION: Montecarlo - Needles
//---- Metodo de asignacion de tareas: Asignacion estatica del numero de pruebas
//---- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada hilo
//---- Mecanismo de actualizacion: Reduccion
void needlesReduction(){
	hits = 0;
	#pragma omp parallel num_threads(numberOfThreads) reduction(+: hits)
	{
		RandomGenerator randomGenerator;
		#pragma omp for schedule(static)
		for(int i=0; i<N; i++)
		hits += needlesTest(randomGenerator);
	}
	pi = 2.0 * N / hits;
}

int main(int argc, char** argv){
	// Configuracion
	version = argv[1];
	N = atoi(argv[2]);
	numberOfThreads = atoi(argv[3]);
	show = atoi(argv[4]);

	// Calculo del numero pi
	if(version == "critical")
	needlesCritical();
	else if(version == "atomic")
	needlesAtomic();
	else if(version == "reduction")
	needlesReduction();
	else
	cout<<"Error! Nombre de version incorrecto"<<endl;

	// Mostrar el resultado
	if(show){
		cout<<"Numero de intentos: "<<N<<endl;
		cout<<"Numero de aciertos: "<<hits<<endl;
		cout<<"Numero pi: "<<fixed<<setprecision(6)<<pi<<endl;
	}
}