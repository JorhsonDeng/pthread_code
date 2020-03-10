#ifndef _Thread_H
#define _Thread_H
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <list>
#include <iostream>
using namespace std;

#define True 1

struct PThreadhdl;
class Thread
{
public:
	Thread();
	~Thread();
	
	//创建并启动
	virtual int run();
	
	//等待和收回资源
	static void Join(Thread& thrd);
	
	//sleep函数
	static void Msleep(int ms);
	static void Sleep(int s);
//定以成纯虚函数的形式，如果派生类不重复进行定以的话，则无法进行调用！	
public:
	virtual int routine() = 0;
	
private:
//这里PThreadhdl是一个未定义的结构体，因此只能用其指针的形式定以变量
//如果定以引用或者其他形式的变量，则会出现incomplete type的错误！
	struct PThreadhdl* hThread;
	
};

class Sema
{
public:
	Sema();
	~Sema();
	bool wait();	//等待信号量
	bool post();	//增加信号量
//private:
	sem_t sem;
};



class Mutex
{
public:
	Mutex();
	~Mutex();
	bool lock();
	bool unlock();
private:
	pthread_mutex_t mutex; 
};


/*
void server_hdl(TcpSocket *pconnsocket)
{
    printf("\r\n server_hdl is running...\n");
    TcpSocket *connsocket = pconnsocket;
    char request[1024];
    int rel;
	while(!exit_flag){
        char buf[520]; 
		const char *clientip = (*connsocket).sockaddr.GetIp_str().c_str();
        int clientport = (*connsocket).sockaddr.GetPort();
        sprintf(buf,"%s:%d receive ok!",clientip,clientport);
        memset(request,'\0',sizeof(request));             
        rel = (*connsocket).Recv(request,1024,0);  //这里用 connsocket与客户端进行通信，而不是 tcpsocket变量          
        request[strlen(request)+1]='\0';                
        printf("receive buf: %d:%s\n",rel,request);  
        if(rel <= 0)
            break;              
        int s = (*connsocket).Send(buf,strlen(buf));//发送响应                
        printf("send=%d\n",s);                
        if(strcmp(request,"end") == 0){
            printf("\r\n will exiting...\n");
            break;                
        }
	}
    (*connsocket).Close(); 
    free(connsocket);   //关闭socket之后，记得释放所占用的资源 
}
*/
/*
*多重继承派生类的构造函数问题：
*派生类构造函数名(总参数列表):基类1构造函数(参数表),基类3构造函数(参数表),基类4构造函数(参数表)
*{派生类中新增数据成员初始化变量}
*/


#endif