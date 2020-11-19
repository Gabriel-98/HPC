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
	system("g++ sequential/sequential.cpp -o bin/binSequential");
	system("mpicxx mpi/mpi.cpp -o bin/binMPI");
}

void runSequentialSolutions(){
	vector<string> binaries = { "bin/binSequential"};
	vector<string> N = {"1000", "2000"};
	string maxValue = "500";
	string show = "0";

	cout<<endl<<"***** SOLUCION SECUENCIAL *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		for(int nId=0; nId < N.size(); nId++){
			vector<string> params = {
				"./stats",
				"type1",
				binaries[binaryId],
				N[nId],
				maxValue,
				show
			};

			cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<")"<<endl;
			system(stringToCharArray(paramsToCommand(params)));
			cout<<endl;
		}
	}
}

void runMPISolutions(){
	vector<string> binaries = { "bin/binMPI"};
	vector<string> N = {"1000", "2000"};
	string maxValue = "500";
	vector<string> numberOfProcess = {"2", "3", "4", "5", "6", "7"};
	vector<string> host = {
		"head,wn1",
		"head,wn1,wn2",
		"head,wn1,wn2,wn3",
		"head,wn1,wn2,wn3,wn4",
		"head,wn1,wn2,wn3,wn4,wn5",
		"head,wn1,wn2,wn3,wn4,wn5,wn6"
	};
	string show = "0";

	cout<<endl<<"***** SOLUCION CON MPI *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		for(int nId=0; nId < N.size(); nId++){
			for(int numberOfProcessId=0; numberOfProcessId < numberOfProcess.size(); numberOfProcessId++){
				vector<string> params = {
					"./stats",
					"type1",
					"mpirun",
					"-n " + numberOfProcess[numberOfProcessId],
					"-hosts " + host[numberOfProcessId],
					binaries[binaryId],
					N[nId],
					maxValue,
					show
				};

				cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<", procesos="<<numberOfProcess[numberOfProcessId]<<")"<<endl;
				system(stringToCharArray(paramsToCommand(params)));
				cout<<endl;
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