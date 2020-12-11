#include<bits/stdc++.h>
using namespace std;

char* stringToCharArray(string s){
	char* ans = (char*)malloc(sizeof(char)*s.size());
	for(int i=0; i<s.size(); i++)
	ans[i] = s[i];
	return ans;
}

// input: vector de cadenas (el nombre del programa binario y los parametros)
// output: una cadena concatenando los parametros separados por un espacio
string paramsToCommand(vector<string> &params){
	string command = "";
	if(params.size() > 0)
	command += params[0];
	for(int i=1; i<params.size(); i++)
	command += " " + params[i];		
	return command;
}

void compile(){
	system("g++ sequential/sequential.cpp -o bin/sequential");
	system("mpicxx openmp_mpi/openmp_mpi.cpp -fopenmp -o bin/openmp_mpi");
}

void runSequentialSolutions(){
	vector<string> binaries = {"bin/sequential"};
	string N = "5000";
	string maxValue = "500";
	string show = "0";

	cout<<endl<<"***** SOLUCION SECUENCIAL *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		vector<string> params = {
			"./stats",
			"type1",
			binaries[binaryId],
			N,
			maxValue,
			show
		};

		cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N<<")"<<endl;
		system(stringToCharArray(paramsToCommand(params)));
		cout<<endl;
	}
}

void runOpenMPWithMPISolutions(){
	vector<string> binaries = {"bin/openmp_mpi"};
	string N = "5000";
	vector<string> numberOfProcess = {"2", "3", "4", "5", "6", "7"};
	vector<string> numberOfThreadsByWorker = {"1", "2", "3", "4"};
	string maxValue = "500";
	vector<string> host = {
		"head,wn1",
		"head,wn1,wn2",
		"head,wn1,wn2,wn3",
		"head,wn1,wn2,wn3,wn4",
		"head,wn1,wn2,wn3,wn4,wn5",
		"head,wn1,wn2,wn3,wn4,wn5,wn6"
	};
	string show = "0";

	cout<<endl<<"***** SOLUCION CON OpenMP + MPI *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		for(int numberOfProcessId=0; numberOfProcessId < numberOfProcess.size(); numberOfProcessId++){
			for(int numberOfThreadsByWorkerId=0; numberOfThreadsByWorkerId < numberOfThreadsByWorker.size(); numberOfThreadsByWorkerId++){
				vector<string> params = {
					"./stats",
					"type1",
					"mpirun",
					"-n " + numberOfProcess[numberOfProcessId],
					"-hosts " + host[numberOfProcessId],
					binaries[binaryId],
					N,
					maxValue,
					numberOfThreadsByWorker[numberOfThreadsByWorkerId],
					show
				};

				cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N<<", procesos="<<numberOfProcess[numberOfProcessId];
				cout<<", hilos_por_worker="<<numberOfThreadsByWorker[numberOfThreadsByWorkerId]<<")"<<endl;
				system(stringToCharArray(paramsToCommand(params)));
				cout<<endl;
			}
		}
	}
}


void runTest(){
	compile();
	runSequentialSolutions();
	runOpenMPWithMPISolutions();
}


int main(){
	runTest();
}