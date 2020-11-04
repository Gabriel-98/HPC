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
	system("g++ openMP/needles.cpp -fopenmp -o bin/binOpenMPNeedles");
	system("g++ openMP/dartboard.cpp -fopenmp -o bin/binOpenMPDartboard");
}

void runOpenMPSolutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binOpenMPNeedles", "bin/binOpenMPDartboard"};
	vector<string> versions = {"critical", "atomic", "reduction"};
	string N = "10000000";
	vector<string> numberOfThreads = {"1", "2", "4"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON OpenMP *****"<<endl;
	for(int binaryId=0; binaryId < binaries.size(); binaryId++){
		for(int versionId=0; versionId < versions.size(); versionId++){
			for(int numberOfThreadsId=0; numberOfThreadsId < numberOfThreads.size(); numberOfThreadsId++){
				vector<string> params = {
					"./stats",
					"type1",
					binaries[binaryId],
					versions[versionId],
					N,
					numberOfThreads[numberOfThreadsId],
					show
				};

				cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", version="<<versions[versionId]<<", N="<<N<<", hilos="<<numberOfThreads[numberOfThreadsId]<<")"<<endl;
				system(stringToCharArray(paramsToCommand(params)));
				cout<<endl;
			}
		}
	}
}


void runTest(){
	compile();
	runOpenMPSolutions();
}


int main(){
	runTest();
}