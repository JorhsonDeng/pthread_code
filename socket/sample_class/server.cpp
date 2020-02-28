#include<stdio.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<assert.h>
#include<string.h>
#include"Socket.h"
#include<iostream>
using namespace std;
const int port = 8888;
int main(int argc,char *argv[])
{    
    if(argc<0)    
    {        
        printf("need two canshu\n");        
        return 1;    
    }    
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

   char request[1024];
   int rel;
    while(1)    
    {        
        TcpSocket *connsocket = new(TcpSocket); //每有一个新的连接，都会new一个新的TcpSocket对象，用来与客户端进行通信
        (*tcpsocket).Accept(connsocket);       
        //cout<<"the ip is:"<<(*connsocket).sockaddr.GetIp_str()<<endl; 
        printf("\r\n client addr =%s, client port = %d\n",(*connsocket).sockaddr.GetIp_str().c_str(),(*connsocket).sockaddr.GetPort());
        if(connfd<0)         
        {            
            printf("errno\n");        
        }        
        else{                
             
            memset(request,'\0',sizeof(request));              
            rel = (*connsocket).Recv(request,1024,0);  //这里用 connsocket与客户端进行通信，而不是 tcpsocket变量          
            request[strlen(request)+1]='\0';                
            printf("%d:%s\n",rel,request);                
            printf("successeful!\n");                
            char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应               
            int s = (*connsocket).Send(buf,strlen(buf));//发送响应                
            printf("send=%d\n",s);                
  
            (*connsocket).Close(); 
            free(connsocket);   //关闭socket之后，记得释放所占用的资源  
            if(strcmp(request,"end") == 0){
                printf("\r\n will exiting...\n");
                break;                
            }
           
        }    
    }                    
    printf("\r\n exiting.......\n");
    free(tcpsocket);
    return 0;
    
}
