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
	system("g++ openMP/openMP.cpp -fopenmp -o bin/binOpenMP");
}

void runOpenMP1Solutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binOpenMP"};
	vector<string> versions = {"staticRow"};
	vector<string> N = {"100", "500", "1000"};
	string maxValue = "1000";
	vector<string> numberOfThreads = {"1", "2", "4"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON OpenMP *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int versionId=0; versionId < versions.size(); versionId++){
				for(int nId=0; nId < N.size(); nId++){
					for(int numberOfThreadsId=0; numberOfThreadsId < numberOfThreads.size(); numberOfThreadsId++){
						vector<string> params = {
							"./stats",
							"type1",
							binaries[binaryId],
							versions[versionId],
							N[nId],
							maxValue,
							numberOfThreads[numberOfThreadsId],
							show
						};

						cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", version="<<versions[versionId]<<", N="<<N[nId]<<", hilos="<<numberOfThreads[numberOfThreadsId]<<", rep="<<r<<")"<<endl;
						system(stringToCharArray(paramsToCommand(params)));
						cout<<endl;
					}
				}
			}
		}
	}
}

void runOpenMP2Solutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binOpenMP"};
	vector<string> versions = {"staticBox", "dynamicBox"};
	string N = "2000";
	string maxValue = "500";
	vector<string> numberOfThreads = {"4"};
	vector<string> blockSizes = {"1", "10", "100", "1000"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON OpenMP *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int versionId=0; versionId < versions.size(); versionId++){
				for(int numberOfThreadsId=0; numberOfThreadsId < numberOfThreads.size(); numberOfThreadsId++){
					for(int blockSizeId=0; blockSizeId < blockSizes.size(); blockSizeId++){
						vector<string> params = {
							"./stats",
							"type1",
							binaries[binaryId],
							versions[versionId],
							N,
							maxValue,
							numberOfThreads[numberOfThreadsId],
							blockSizes[blockSizeId],
							show
						};

						cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", show="<<show<<", version="<<versions[versionId]<<", N="<<N<<", hilos="<<numberOfThreads[numberOfThreadsId]<<", tamañoBloque="<<blockSizes[blockSizeId]<<", rep="<<r<<")"<<endl;
						system(stringToCharArray(paramsToCommand(params)));
						cout<<endl;
						system("exit");
					}
				}
			}
		}
	}
}

void runTest(){
	compile();
	runOpenMP1Solutions();
	runOpenMP2Solutions();
}


int main(){
	runTest();
}