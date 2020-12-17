#include<bits/stdc++.h>
using namespace std;

struct Matriz{
	const static int N = 1000;
	int matriz[N][N];

	void randomGenerator(int maxValue);
	void print();
};

void Matriz::randomGenerator(int maxValue){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		matriz[i][j] = rand() % maxValue;
	}
}

void Matriz::print(){
	for(int i=0; i<N; i++){
		for(int j=0; j<N; j++)
		cout<<matriz[i][j]<<" ";
		cout<<endl;
	}
}