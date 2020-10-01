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
	system("g++ sequential/sequential.cpp -O2 -o bin/binSequentialComOptO2");

	system("g++ threads/threads.cpp -pthread -o bin/binThreads");
	system("g++ threads/threads.cpp -pthread -O2 -o bin/binThreadsComOptO2");

	system("g++ process/process.cpp -lrt -o bin/binProcess");
	system("g++ process/process.cpp -lrt -O2 -o bin/binProcessComOptO2");
}

void runSequentialSolutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binSequential", "bin/binSequentialComOptO2"};
	vector<string> versions = {"version1", "version2", "version3", "version4"};
	string N = "1000";
	string maxValue = "1000";
	string show = "0";

	cout<<endl<<"***** SOLUCIONES SECUENCIALES *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int versionId=0; versionId < versions.size(); versionId++){
				vector<string> params = {
					"./stats",
					"type1",
					binaries[binaryId],
					versions[versionId],
					N,
					maxValue,
					show
				};

				cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", version="<<versions[versionId]<<", N="<<N<<", rep="<<r<<")"<<endl;
				system(stringToCharArray(paramsToCommand(params)));
				cout<<endl;
			}
		}
	}
}

void runThreadSolutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binThreads", "bin/binThreadsComOptO2"};
	vector<string> versions = {"version1", "version3"};
	string N = "1000";
	string maxValue = "1000";
	vector<string> numberOfThreads = {"3"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON HILOS *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int versionId=0; versionId < versions.size(); versionId++){
				for(int numberOfThreadsId=0; numberOfThreadsId < numberOfThreads.size(); numberOfThreadsId++){
					vector<string> params = {
						"./stats",
						"type1",
						binaries[binaryId],
						versions[versionId],
						N,
						maxValue,
						numberOfThreads[numberOfThreadsId],
						show
					};

					cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", version="<<versions[versionId]<<", N="<<N<<", hilos: "<<numberOfThreads[numberOfThreadsId]<<", rep="<<r<<")"<<endl;
					system(stringToCharArray(paramsToCommand(params)));
					cout<<endl;
				}
			}
		}
	}
}

void runProcessSolutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binProcess", "bin/binProcessComOptO2"};
	vector<string> versions = {"version1", "version3"};
	string N = "1000";
	string maxValue = "1000";
	vector<string> numberOfProcess = {"3"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON PROCESOS *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int versionId=0; versionId < versions.size(); versionId++){
				for(int numberOfProcessId=0; numberOfProcessId < numberOfProcess.size(); numberOfProcessId++){
					vector<string> params = {
						"./stats",
						"type1",
						binaries[binaryId],
						versions[versionId],
						N,
						maxValue,
						numberOfProcess[numberOfProcessId],
						show
					};

					cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", version="<<versions[versionId]<<", N="<<N<<", procesos: "<<numberOfProcess[numberOfProcessId]<<", rep="<<r<<")"<<endl;
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
	runThreadSolutions();
	runProcessSolutions();
}


int main(){
	runTest();
}