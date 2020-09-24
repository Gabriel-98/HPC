#include<unistd.h>
#include <fcntl.h>
using namespace std;

struct Channel{
	int ppid, pid;
	int fatherInputPipeFD;
	int fatherOutputPipeFD;
	int childInputPipeFD;
	int childOutputPipeFD;
	int lastReadingStatus;

	Channel();
	int getInputFD();
	int getOutputFD();
	int getLastReadingStatus();
	void setPipeSize(int x);
	int getPipeSize();

	int readInt();
	long long readLongLong();
	float readFloat();
	double readDouble();
	string readString();
	int* readIntArray(int n);

	void writeInt(int message);
	void writeLongLong(long long message);
	void writeFloat(float message);
	void writeDouble(double message);
	void writeString(string task);
	void writeIntArray(int n, int a[]);

	void closeFatherPipePrivateFD();
	void closeChildPipePrivateFD();
	void closeChannel();
};

Channel::Channel(){
	ppid = getpid();
	int fatherWritingPipeFD[2];
	int childWritingPipeFD[2];

	pipe(fatherWritingPipeFD);
	pipe(childWritingPipeFD);
	
	childInputPipeFD = fatherWritingPipeFD[0];
	fatherOutputPipeFD = fatherWritingPipeFD[1];
	fatherInputPipeFD = childWritingPipeFD[0];
	childOutputPipeFD = childWritingPipeFD[1];
}

void Channel::setPipeSize(int size){
	fcntl(fatherOutputPipeFD, F_SETPIPE_SZ, size);
	fcntl(childOutputPipeFD, F_SETPIPE_SZ, size);
}

int Channel::getPipeSize(){
	return fcntl(fatherOutputPipeFD, F_GETPIPE_SZ);
}

int Channel::getInputFD(){
	if(getpid() == ppid)
	return fatherInputPipeFD;
	else
	return childInputPipeFD;
}

// Indica si al hacer la ultima lectura se cerro la tuberia
int Channel::getLastReadingStatus(){
	return lastReadingStatus;
}

int Channel::getOutputFD(){
	if(getpid() == ppid)
	return fatherOutputPipeFD;
	else
	return childOutputPipeFD;
}

int Channel::readInt(){
	int message;
	int inputPipeFD = getInputFD();
	lastReadingStatus = read(inputPipeFD, &message, sizeof(int));
	return message;
}

long long Channel::readLongLong(){
	long long message;
	int inputPipeFD = getInputFD();
	read(inputPipeFD, &message, sizeof(long long));
	return message;
}

float Channel::readFloat(){
	float message;
	int inputPipeFD = getInputFD();
	lastReadingStatus = read(inputPipeFD, &message, sizeof(float));
	return message;
}

double Channel::readDouble(){
	int inputPipeFD = getInputFD();
	double message;
	read(inputPipeFD, &message, sizeof(double));
	return message;
}

string Channel::readString(){
	int inputPipeFD = getInputFD();
	string message = "";
	char c;
	while(true){
		lastReadingStatus = read(inputPipeFD, &c, 1);
		if(lastReadingStatus == 0 || c == '\n')
		break;
		message += c;
	}
	return message;
}

int* Channel::readIntArray(int n){
	int inputPipeFD = getInputFD();
	int *ans = (int*)malloc(sizeof(int)*n);
	for(int i=0; i<n; i++)
	lastReadingStatus = read(inputPipeFD, &ans[i], sizeof(int));
	return ans;
}

void Channel::writeInt(int message){
	int outputPipeFD = getOutputFD();
	int x = write(outputPipeFD, &message, sizeof(int));
}

void Channel::writeLongLong(long long message){
	int outputPipeFD = getOutputFD();
	write(outputPipeFD, &message, sizeof(long long));
}

void Channel::writeFloat(float message){
	int outputPipeFD = getOutputFD();
	write(outputPipeFD, &message, sizeof(float));
}

void Channel::writeDouble(double message){
	int outputPipeFD = getOutputFD();
	write(outputPipeFD, &message, sizeof(double));
}

void Channel::writeString(string message){
	int outputPipeFD = getOutputFD();
	for(int i=0; i<message.size(); i++)
	write(outputPipeFD, &message[i], 1);
	write(outputPipeFD, "\n", 1);
}

void Channel::writeIntArray(int n, int a[]){
	int outputPipeFD = getOutputFD();
	write(outputPipeFD, a, sizeof(int)*n);
}

void Channel::closeFatherPipePrivateFD(){
	close(childInputPipeFD);
	close(childOutputPipeFD);
}

void Channel::closeChildPipePrivateFD(){
	close(fatherInputPipeFD);
	close(fatherOutputPipeFD);
}

void Channel::closeChannel(){
	if(getpid() == ppid){
		close(fatherInputPipeFD);
		close(fatherOutputPipeFD);
	}
	else{
		close(childInputPipeFD);
		close(childOutputPipeFD);
	}
}