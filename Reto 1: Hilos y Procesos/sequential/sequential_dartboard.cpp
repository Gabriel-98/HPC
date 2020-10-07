#include<bits/stdc++.h>
using namespace std;

// Variables de la configuracion
int N;
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


//---- SOLUCION: Montecarlo - Dartboard
//---- Caracteristicas:
//----		- Tipo de dato: Double
//----		- Los valores de X,Y se generan con 4 decimales en el intervalo [-1,1]
void dartboard(){
	hits = 0;
	RandomGenerator randomGenerator;
	for(int i=0; i<N; i++){
		double x = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
		double y = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
		if(x*x + y*y <= 1.0)
		hits++;
	}
	pi = (4.0 * hits) / N;
}

int main(int argc, char** argv){
	// Configuracion
	N = atoi(argv[1]);
	show = atoi(argv[2]);
	
	// Calculo del numero pi
	dartboard();

	// Mostar el resultado
	if(show){
		cout<<"Numero de intentos: "<<N<<endl;
		cout<<"Numero de aciertos: "<<hits<<endl;
		cout<<"Numero pi: "<<fixed<<setprecision(6)<<pi<<endl;
	}
}