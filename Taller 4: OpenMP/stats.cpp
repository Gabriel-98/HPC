#include<bits/stdc++.h>
#include<sys/time.h>
#include<sys/resource.h>
#include<unistd.h>
#include<wait.h>
using namespace std;
using namespace std::chrono;

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

void basicStats(char *command){
	int rootPid = getpid();
	double startTime, finalTime, realTime;

	fork();
	if(getpid() != rootPid)
	system(command);
	else{
		startTime = system_clock::now().time_since_epoch().count();
		wait(0);
		finalTime = system_clock::now().time_since_epoch().count();
		realTime = (finalTime - startTime) / 1e9;

		rusage r;
		getrusage(RUSAGE_CHILDREN, &r);
		double userCPUTime, systemCPUTime;
		userCPUTime =  r.ru_utime.tv_sec * 1.0 + r.ru_utime.tv_usec / 1000000.0;
		systemCPUTime =  r.ru_stime.tv_sec * 1.0 + r.ru_stime.tv_usec / 1000000.0;

		cout<<"real: "<<fixed<<setprecision(3)<<realTime<<"s\t";
		cout<<"user: "<<fixed<<setprecision(3)<<userCPUTime<<"s\t";
		cout<<"sys: "<<fixed<<setprecision(3)<<systemCPUTime<<"s\t";
		cout<<"mem: "<<r.ru_maxrss<<" KB"<<endl;
	}
}

void basicStats(vector<string> params){
	char *command = stringToCharArray(paramsToCommand(params));
	basicStats(command);
}

void stats(char* command){
	int rootPid = getpid();
	double startTime, finalTime, realTime;

	fork();
	if(getpid() != rootPid)
	system(command);
	else{
		startTime = system_clock::now().time_since_epoch().count();
		wait(0);
		finalTime = system_clock::now().time_since_epoch().count();
		realTime = (finalTime - startTime) / 1e9;
		
		rusage r;
		getrusage(RUSAGE_CHILDREN, &r);
		double userCPUTime, systemCPUTime;
		userCPUTime =  r.ru_utime.tv_sec * 1.0 + r.ru_utime.tv_usec / 1000000.0;
		systemCPUTime =  r.ru_stime.tv_sec * 1.0 + r.ru_stime.tv_usec / 1000000.0;

		cout<<"tiempo real: "<<fixed<<setprecision(3)<<realTime<<"s"<<endl;
		cout<<"tiempo de cpu de usuario: "<<fixed<<setprecision(3)<<userCPUTime<<"s"<<endl;
		cout<<"tiempo de cpu del sistema: "<<fixed<<setprecision(3)<<systemCPUTime<<"s"<<endl;
		cout<<"memoria: "<<r.ru_maxrss<<" KB"<<endl;
		cout<<"reclamos de pagina: "<<r.ru_minflt<<endl;
		cout<<"fallos de pagina: "<<r.ru_majflt<<endl;
		cout<<"bloqueos por operaciones de entrada (sistemas de archivos): "<<r.ru_inblock<<endl;
		cout<<"bloqueos por operaciones de salida (sistema de archivos): "<<r.ru_oublock<<endl;
		cout<<"cambios de contexto voluntarios: "<<r.ru_nvcsw<<endl;
		cout<<"cambios de contexto involuntarios: "<<r.ru_nivcsw<<endl;
	}
}

void stats(vector<string> params){
	char *command = stringToCharArray(paramsToCommand(params));
	stats(command);
}

int main(int argv, char** argc){
	string statsType;
	vector<string> params;
	statsType = argc[1];
	for(int i=2; i<argv; i++)
	params.push_back(argc[i]);

	if(statsType == "type1")
	basicStats(params);
	else if(statsType == "type2")
	stats(params);
	else
	cout<<"Error! Tipo de estadisticas incorrecto"<<endl;
}