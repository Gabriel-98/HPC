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
	system("mpicxx mpi/mpiNonBlocking.cpp -o bin/mpiNonBlocking");
	system("mpicxx mpi/mpiSync.cpp -o bin/mpiSync");
}

void runSequentialSolutions(){
	vector<string> binaries = {"bin/sequential"};
	vector<string> N = {
		"10",
		"100",
		"1000",
		"10000",
		"100000"};
	vector<string> iterations = {"1000", "100000"};
	string readOfFile = "0";
	string show = "0";

	cout<<endl<<"***** SOLUCIONES SECUENCIALES *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		for(int nId=0; nId < N.size(); nId++){
			for(int iterationsId=0; iterationsId < iterations.size(); iterationsId++){
				vector<string> params = {
					"./stats",
					"type1",
					binaries[binaryId],
					readOfFile,
					N[nId],
					iterations[iterationsId],
					show
				};

				cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<", iteraciones="<<iterations[iterationsId]<<")"<<endl;
				system(stringToCharArray(paramsToCommand(params)));
				cout<<endl;
			}
		}
	}
}

void runMPISolutions(){
	vector<string> binaries = {"bin/mpiNonBlocking", "bin/mpiSync"};
	vector<string> N = {
		"10",
		"100",
		"1000",
		"10000",
		"100000"};
	vector<string> iterations = {"1000", "100000"};
	vector<string> numberOfProcess = {"2", "4"};
	vector<string> host = {
		"head,wn1",
		"head,wn1,wn2",
		"head,wn1,wn2,wn3"};
	string readOfFile = "0";
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON MPI *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		for(int nId=0; nId < N.size(); nId++){
			for(int iterationsId=0; iterationsId < iterations.size(); iterationsId++){
				for(int numberOfProcessId=0; numberOfProcessId < numberOfProcess.size(); numberOfProcessId++){
					vector<string> params = {
						"./stats",
						"type1",
						"mpirun",
						"-n " + numberOfProcess[numberOfProcessId],
						"-hosts " + host[numberOfProcessId],
						binaries[binaryId],
						readOfFile,
						N[nId],
						iterations[iterationsId],
						show
					};

					cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<", iteraciones="<<iterations[iterationsId]<<", procesos="<<numberOfProcess[numberOfProcessId]<<")"<<endl;
					system(stringToCharArray(paramsToCommand(params)));
					cout<<endl;
				}
			}
		}
	}
}


void runTest(){
	compile();
	runSequentialSolutions();
	runMPISolutions();
}


int main(){
	runTest();
}