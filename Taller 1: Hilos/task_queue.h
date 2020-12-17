typedef pair<int,int> par;

struct TaskQueue{
	queue<par> Q;
	mutex mtx;
	int active;

	TaskQueue(){ active = 1; }

	void enable();
	void disable();
	int empty();
	int push(par p);
	int pop(par &p);
};


TaskQueue Qu;

void TaskQueue::enable(){
	mtx.lock();
	active = 1;
	mtx.unlock();
}

void TaskQueue::disable(){
	mtx.lock();
	active = 0;
	mtx.unlock();
}

int TaskQueue::empty(){
	int isEmpty;
	mtx.lock();
	isEmpty = Q.empty();
	mtx.unlock();
	return isEmpty;
}

// retorna: 1 si esta activo, 0 si esta inactivo
int TaskQueue::push(par p){
	mtx.lock();
	if(active == 0){
		mtx.unlock();
		return 0;
	}
	else{
		Q.push(p);
		mtx.unlock();
		return 1;
	}
}

int TaskQueue::pop(par &p){
	mtx.lock();
	if(active == 0){
		mtx.unlock();
		return 0;
	}
	else{
		if(Q.empty()){
			p = par(-1,-1);
			mtx.unlock();
			return 1;
		}
		else{
			p = Q.front();
			Q.pop();
			mtx.unlock();
			return 1;
		}		
	}
}

int solveBox(Matriz &m1, Matriz &m2, int x, int y){
	int answer = 0;
	int z = m1.N;
	for(int i=0; i<z; i++)
	answer += m1.matriz[x][i] * m2.matriz[i][y];
	return answer;
}