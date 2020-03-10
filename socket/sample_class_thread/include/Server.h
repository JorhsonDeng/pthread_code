#ifndef _SERVER_H
#define  _SERVER_H
#include "Thread.h"

typedef void (*FunType)(void *argc);	//定义函数指针对象变量 
class MyThread:public Thread
{
public:
    MyThread(){}
	~MyThread(){}
	virtual int routine() //这里的virtual可加可不加了，也是虚函数！
	{
		//cout<<"routine fucntion is calling!!!"<<endl;
        fun_hdl(arg);
	}
	void set(FunType hdl,void *argc)
	{
		fun_hdl = hdl;
		arg = argc;
	}
private:
	FunType fun_hdl;
	void *arg;
};

template<typename T>
class Server
{
public:
    //Server(int max_thread = 10, int max_request = 100);
	Server(int max_thread, int max_request);
	~Server(){}

	static void work(void *argc);
	//void work(void *argc);
	void run();

	void addrequest(T *request);
private:
	int MAX_THREAD = 10;
	int MAX_REQUEST = 100;
	MyThread mythread[10];
    T *ptcpsocket = NULL;
	std::list<T*> socketqueue;
	Sema sock_sem;
	Mutex sock_mutex;
};

template<typename T>
Server<T>::Server(int max_thread, int max_request)
{
	MAX_THREAD = max_thread;
	MAX_REQUEST = max_request;

	for(int i=0; i<MAX_THREAD;i++){
		//cout<<"constructor function calling!!!"<<endl;
		mythread[i].set((FunType)&Server::work,this);	//传入函数指针到Mythread对象中！
		//printf("function pointer is:%x \n",(FunType)&Server::work);
		mythread[i].run();
	}

}

template<typename T>
void Server<T>::run()
{
	cout<<"run function is calling!!!"<<endl;
	while(True){
		//cout<<"sock_sem pointer is:"<<&(sock_sem.sem)<<endl;
		//cout<<"we start wait a semaphore!!!"<<endl;
		sock_sem.wait();
		//cout<<"we got a semaphore"<<endl;
		sock_mutex.lock();
		if(socketqueue.empty() == True){
			sock_mutex.unlock();
			continue;
		}
		T *conn = socketqueue.front();
		socketqueue.pop_front();
		sock_mutex.unlock();
		cout<<"calling the doit function!!!"<<endl;
		conn->doit();
	}
	
}
/*
*这里的work函数时静态函数，所以再调用对象中的成员函数的时候需要指出对应的
*对象实例，如果不指定的话，就不能判断调用的是哪个实例中的成员函数了！
*/
template<typename T>
void Server<T>::work(void *argc)
{
	cout<<"work function is calling!!!"<<endl;
	Server *pool = (Server*)argc;
	pool->run();
}

template<typename T>
void Server<T>::addrequest(T *request)
{
	cout<<"addrequest function is calling!!!"<<endl;
	sock_mutex.lock();
	if(socketqueue.size()>MAX_REQUEST){
		//cout<<"Add Mysocket Fail, the request number is more than the max_request!"<<endl;
		sock_mutex.unlock();
		return;
	}
	socketqueue.push_back(request);
	sock_mutex.unlock();
	sock_sem.post();
	cout<<"addrequest post a semaphore!!!"<<endl;
}

#endif