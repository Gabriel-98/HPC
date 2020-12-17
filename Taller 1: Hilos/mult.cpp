#include<bits/stdc++.h>
#include<chrono>
#include "matriz.h"
#include "task_queue.h"
using namespace std;
using namespace std::chrono;
#define endl '\n'
typedef pair<int,int> par;

const int maxValue = 1000;
const int numberOfThreads = 4;

Matriz m1, m2, ans;


void init(){
	srand(time(NULL));
}


void executionTime(string solutionName, void (* f)()){
	double t1, t2, total;
	t1 = system_clock::now().time_since_epoch().count();
	f();
	t2 = system_clock::now().time_since_epoch().count();
	total = (t2 - t1) / 1e9;
	cout<<solutionName<<":"<<endl<<"time: "<<total<<endl<<endl;
}


/*** SOLUCION SERIAL ***/
void serialSolution(){
	for(int i=0; i<m1.N; i++){
		for(int j=0; j<m1.N; j++)
		ans.matriz[i][j] = solveBox(m1, m2, i,j);
	}
}


/*** SOLUCION SERIAL:TODO EL CALCULO DESDE LA MISMA FUNCION ***/
void serialSolution2(){
	for(int i=0; i<m1.N; i++){
		for(int j=0; j<m1.N; j++){
			ans.matriz[i][j] = 0;
			for(int l=0; l<m1.N; l++)
			ans.matriz[i][j] += m1.matriz[i][l] * m2.matriz[l][j];
		}
	}
}


/*** PRIMERA SOLUCION: ASIGNACION ESTATICA DE CASILLAS ***/
void staticAssignOfBox(int idTask){
	int ini, fin, taskProm, taskSobra;
	taskProm = (m1.N * m1.N) / numberOfThreads;
	taskSobra = (m2.N * m2.N) % numberOfThreads;

	if(idTask < taskSobra){
		ini = (taskProm+1) * idTask;
		fin = ini + taskProm;
	}
	else{
		ini = ((taskProm+1) * taskSobra) + (taskProm * (idTask - taskSobra));
		fin = ini + taskProm - 1;
	}

	for(int l=ini; l<=fin; l++){
		int x,y;
		x = l / m1.N;
		y = l % m2.N;
		ans.matriz[x][y] = solveBox(m1, m2, x, y);
	}
}

void firstSolution(){
	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++)
	threads[i] = thread(staticAssignOfBox, i);

	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();
}


/*** SEGUNDA SOLUCION: Asignacion Estatica de Filas ***/
void staticAssignOfRows(int idTask){
	for(int i=idTask; i<m1.N; i+=numberOfThreads){
		for(int j=0; j<m1.N; j++)
		ans.matriz[i][j] = solveBox(m1, m2, i, j);
	}
}

void secondSolution(){
	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++)
	threads[i] = thread(staticAssignOfRows, i);

	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();
}

/*** TERCERA SOLUCION: ASIGNACION DINAMICA DE CASILLAS ***/
void dinamicAssignOfBox(){
	while(true){
		par p;
		int active = Qu.pop(p);
		if(active == 1){
			int x, y;
			x = p.first;
			y = p.second;

			if(x != -1 || y != -1)
			ans.matriz[x][y] = solveBox(m1, m2, x, y);
		}
		else
		break;
	}
}

void makeBox(){
	for(int i=0; i<m1.N; i++){
		for(int j=0; j<m2.N; j++)
		Qu.push(par(i,j));
	}
	while(!Qu.empty()){ }
	Qu.disable();
}

void thirdSolution(){
	Qu.enable();
	thread prod = thread(makeBox);

	vector<thread> threads(numberOfThreads);
	for(int i=0; i<numberOfThreads; i++)
	threads[i] = thread(dinamicAssignOfBox);

	prod.join();
	for(int i=0; i<numberOfThreads; i++)
	threads[i].join();
}


int main(){
	init();
	string show;
	cout<<"Hilos usados: "<<numberOfThreads<<endl;
	cout<<"Mostrara las matrices[Y/]: ";
	cin>>show;

	m1.randomGenerator(maxValue);
	m2.randomGenerator(maxValue);
	if(show == "Y"){
		m1.print(); cout<<endl;
		m2.print(); cout<<endl;
	}

	executionTime("Solucion Serial", serialSolution);
	if(show == "Y")
	ans.print(); cout<<endl;

	executionTime("Solucion Serial Todo en una Funcion", serialSolution2);
	if(show == "Y")
	ans.print(); cout<<endl;

	executionTime("Solucion Asignacion Estatica de Casillas", firstSolution);
	if(show == "Y")
	ans.print(); cout<<endl;

	executionTime("Solucion Asignacion Estatica de Filas", secondSolution);
	if(show == "Y")
	ans.print(); cout<<endl;

	executionTime("Solucion Asignacion Dinamica de casillas", thirdSolution);
	if(show == "Y")
	ans.print();
}