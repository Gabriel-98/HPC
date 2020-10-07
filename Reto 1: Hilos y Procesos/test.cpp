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
	system("g++ sequential/sequential_needles.cpp -o bin/binSequentialNeedles");
	system("g++ sequential/sequential_dartboard.cpp -o bin/binSequentialDartboard");

	system("g++ threads/threads_needles.cpp -pthread -o bin/binThreadsNeedles");
	system("g++ threads/threads_dartboard.cpp -pthread -o bin/binThreadsDartboard");

	system("g++ process/process_needles.cpp -lrt -o bin/binProcessNeedles");
	system("g++ process/process_dartboard.cpp -lrt -o bin/binProcessDartboard");
}

void runSequentialSolutions(){
	int rep = 3;
	vector<string> binaries = {"bin/binSequentialNeedles", "bin/binSequentialDartboard"};
	vector<string> N = {"10000", "100000", "1000000", "10000000"};
	string show = "1";

	cout<<endl<<"***** SOLUCIONES SECUENCIALES *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int nId=0; nId < N.size(); nId++){
				vector<string> params = {
					"./stats",
					"type1",
					binaries[binaryId],
					N[nId],
					show
				};

				cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<", rep="<<r<<")"<<endl;
				system(stringToCharArray(paramsToCommand(params)));
				cout<<endl;
			}
		}
	}
}

void runThreadSolutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binThreadsNeedles", "bin/binThreadsDartboard"};
	vector<string> N = {"10000", "100000", "1000000", "10000000"};
	vector<string> numberOfThreads = {"4"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON HILOS *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int nId=0; nId < N.size(); nId++){
				for(int numberOfThreadsId=0; numberOfThreadsId < numberOfThreads.size(); numberOfThreadsId++){
					vector<string> params = {
						"./stats",
						"type1",
						binaries[binaryId],
						N[nId],
						numberOfThreads[numberOfThreadsId],
						show
					};

					cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<", hilos: "<<numberOfThreads[numberOfThreadsId]<<", rep="<<r<<")"<<endl;
					system(stringToCharArray(paramsToCommand(params)));
					cout<<endl;
				}
			}
		}
	}
}

void runProcessSolutions(){
	int rep = 1;
	vector<string> binaries = {"bin/binProcessNeedles", "bin/binProcessDartboard"};
	vector<string> N = {"10000", "100000", "1000000", "10000000"};
	vector<string> numberOfProcess = {"4"};
	string show = "0";

	cout<<endl<<"***** SOLUCIONES CON PROCESOS *****"<<endl;
	for(int r=1; r<=rep; r++){
		for(int binaryId=0; binaryId < binaries.size(); binaryId++){
			for(int nId=0; nId < N.size(); nId++){
				for(int numberOfProcessId=0; numberOfProcessId < numberOfProcess.size(); numberOfProcessId++){
					vector<string> params = {
						"./stats",
						"type1",
						binaries[binaryId],
						N[nId],
						numberOfProcess[numberOfProcessId],
						show
					};

					cout<<"[*] Resultado (binario="<<binaries[binaryId]<<", N="<<N[nId]<<", procesos: "<<numberOfProcess[numberOfProcessId]<<", rep="<<r<<")"<<endl;
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