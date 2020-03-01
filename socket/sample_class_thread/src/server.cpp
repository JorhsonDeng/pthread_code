#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
#include"Socket.h"
#include<iostream>
#include"Thread.h"

int exit_flag = 0;
#define MAX_SIZE 1024
char recv_buf[MAX_SIZE];
char snd_buf[MAX_SIZE];


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
/*
*提示如下的错误：
*pure virtual method called
*terminate called without an active exception
*在构造对象的时候，把ServerThread srvThd放到while(1)中就会出现上面的错误，因为while一次这个对象就会被创建和删除一次
*上次创建对象中的函数也会被删除，从而造成错误！
*如果把这个对象的定以放在while循环外边的话，就不会出现该问题、！
*/
/*
*多重继承派生类的构造函数问题：
*派生类构造函数名(总参数列表):基类1构造函数(参数表),基类3构造函数(参数表),基类4构造函数(参数表)
*{派生类中新增数据成员初始化变量}
*/
class ServerThread:public Thread
{
public:
    ServerThread(){}
    void set(TcpSocket *tcpsocket){
        ptcpsocket = tcpsocket;
    }
	virtual int routine() //这里的virtual可加可不加了，也是虚函数！
	{
        if(ptcpsocket == NULL){
            printf("\r\n the tcpsocket pointer is NULL!\n");
            return 0;
        }
        server_hdl(ptcpsocket);
        ptcpsocket = NULL;
		printf("\r\n ServerThread is exiting...\n");
		return 0;
	}
    TcpSocket *ptcpsocket = NULL;
};

class myThread:public Thread
{
public:
    void set(int i){
        num = i;
    }
	//virtual int routine() //这里的virtual可加可不加了，也是虚函数！
	int routine()
	{
		while(!exit_flag){
			printf("\r\n myThread %d is running...\n",num);
			Thread::Sleep(1);
		}
		printf("\r\n myThread is exiting...\n");
		return 0;
	}
    int num;
};

using namespace std;
const int port = 8888;
int main(int argc,char *argv[])
{    
    if(argc<0)    
    {        
        printf("need two canshu\n");        
        return 1;    
    }    
#if 0
    for(int i=0;i<6;i++){
        myThread *thrd = new(myThread);
        (*thrd).set(i);
	    (*thrd).run(); 
        ::sleep(1);
    }
    getchar();
	exit_flag = 1;
    return 0;
#endif
    int sock;    
    int connfd;   
    /* 创建一个SockAddr对象，用于存放地址*/
    SockAddr sockaddr(port);

    TcpSocket *tcpsocket = new TcpSocket();
    int ret = (*tcpsocket).Open();//这个主要用来创建一个socket，并且把句柄放到TcpSocket对象中
    assert(ret != -1); 

    ret = (*tcpsocket).Bind(sockaddr);  //bind sockaddr_in地址
    assert(ret != -1); 
  
    ret = (*tcpsocket).Listen(10);
    assert(ret != -1);   
    ServerThread *srvThd;
    while(1)    
    {       
        TcpSocket *connsocket = new(TcpSocket); //每有一个新的连接，都会new一个新的TcpSocket对象，用来与客户端进行通信
        (*tcpsocket).Accept(connsocket);       
         
        printf("\r\n client addr =%s, client port = %d\n",(*connsocket).sockaddr.GetIp_str().c_str(),(*connsocket).sockaddr.GetPort());
        if(connfd<0)         
        {            
            printf("errno\n");        
        }else{  
            /*这里创建的资源并没有进行释放，后面需要继续处理一下！*/
            /*这里需要用一个向量或者链表，对这个指针进行处理，判断如果当前的描述符关闭的话，则直接认为这个线程死掉了*/
            srvThd = new(ServerThread);
            printf("serThd = %p",srvThd);
            (*srvThd).set(connsocket);
            printf("\r\n start run function!\n");
            (*srvThd).run(); //
        }    
    }                    
    printf("\r\n exiting.......\n");
    free(tcpsocket);
    return 0;
    
}
