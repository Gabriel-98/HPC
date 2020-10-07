#include<bits/stdc++.h>
#include<unistd.h>
#include<wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
using namespace std;
#define endl '\n'


// Variables de la configuracion
int N;
int numberOfProcess;
int show;

// Salida del problema
int hits;
double pi;


// Datos en la memoria compartida
struct Data{
	const static int maxProcess = 32;
	int hitsByProcess[maxProcess];
	Data(){}
};

Data *publicData;


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
//---- Mecanismo de comunicacion: Memoria compartida
//---- Bloqueos o sincronizacion (datos): No
//---- Caracteristicas:
//----		- Los valores de X,Y se generan con 4 decimales en el intervalo [-1,1]
//----		- Generador de numeros aleatorios: Una instancia distinta de la estructura para cada proceso
void dartboardWorker(int id, int numberOfTest){
	int hitsLocal = 0;
	RandomGenerator randomGenerator;
	for(int i=0; i<numberOfTest; i++){
		double x = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
		double y = ((randomGenerator.random() % 20001) / 10000.0) - 1.0;
		if(x*x + y*y <= 1.0)
		hitsLocal++;
	}
	publicData->hitsByProcess[id] = hitsLocal;
}

void dartboardProcess(){
	// Detectar el pid del proceso, definicion del tamaño de la memoria compartida y el nombre del objeto de memoria compartida
	int rootPid = getpid();	
	int SIZE = 1<<27;	// 128 MB
	char publicDataName[] = "/public_data";

	// Crea el objeto de memoria compartida con permisos de lectura, escritura
	// lo crea si no existe, e inicia con un tamaño de 0 bytes
	int publicDataFD = shm_open(publicDataName, O_RDWR | O_CREAT | O_TRUNC, 0666);

	// Redimensiona el tamaño del espacio de memoria compartida
	ftruncate(publicDataFD, SIZE);

	// Obtiene la direccion al espacio de memoria compartida
	void *publicDataAddr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, publicDataFD, 0);
	publicData = (Data*)publicDataAddr;

	// Crear los procesos y se asigna el numero de pruebas que debe realizar cada proceso (asignadas de manera estatica)
	int numberOfTestByProcess = N / numberOfProcess;
	for(int i=0; i<numberOfProcess; i++){
		fork();

		if(getpid() != rootPid){
			void *publicDataAddr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, publicDataFD, 0);
			publicData = (Data*)publicDataAddr;

			if(i < (N % numberOfProcess))
			dartboardWorker(i, numberOfTestByProcess + 1);
			else
			dartboardWorker(i, numberOfTestByProcess);

			// Elimina las referencias al objeto de memoria compartida y finaliza el proceso
			close(publicDataFD);
			shm_unlink(publicDataName);
			exit(0);
		}
	}

	// Espera a la terminacion de los procesos hijos
	for(int i=0; i<numberOfProcess; i++)
	wait(0);

	// Determinar el valor de pi
	hits = 0;
	for(int i=0; i<numberOfProcess; i++)
	hits += publicData->hitsByProcess[i];
	pi = (4.0 * hits) / N;

	// Elimina las referencia al objeto de memoria compartida
	close(publicDataFD);
	shm_unlink(publicDataName);
}

int main(int argc, char	**argv){
	// Configuracion
	N = atoi(argv[1]);
	numberOfProcess = atoi(argv[2]);
	show = atoi(argv[3]);

	// Calculo del numero pi
	dartboardProcess();

	// Mostrar el resultado
	if(show){
		cout<<"Numero de intentos: "<<N<<endl;
		cout<<"Numero de aciertos: "<<hits<<endl;
		cout<<"Numero pi: "<<fixed<<setprecision(6)<<pi<<endl;
	}
}