#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/sendfile.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<assert.h>
#include<unistd.h>
#include<string.h>
#include<iostream>
#include "Socket.h"
using namespace std;

const int port = 8888;
#define BUF_SIZE 1000
int main()
{
    int clientfd;    
    int retVal;   
    char sendbuf[BUF_SIZE];
    char recvbuf[BUF_SIZE];
    //定以一个SockAddr对象的变量 用于设置服务器端的IP地址和端口
    SockAddr sockaddr(port); 
    /*
    *客户端这边比较简单，主要构造一个TcpSocket对象，然后按照原来的顺序依次调用即可
    *这里面最重要的是clientfd这个句柄，这个句柄作为一个变量保存在Socket对象中了，因此再次调用的时候
    *直接调用相应的methods即可，不用再关系内部的一些细节。
    */
    while(true){  
        TcpSocket tcpclient;
        clientfd = tcpclient.Open();
        assert(clientfd>=0);

        retVal = tcpclient.Connect(sockaddr);
        if(retVal<0)      
        {          
            cout << "connect failed!" << endl;            
            close(clientfd); //关闭套接字                   
            return -1;      
        }
        
        cout<<"please input the string you want send!"<<endl;
        memset(sendbuf,'\0',sizeof(sendbuf));

        cin>>sendbuf;

        int s = tcpclient.Send(sendbuf,strlen(sendbuf));
        printf("\r\n we have send data length is %d",s);
        tcpclient.Recv(recvbuf,BUF_SIZE-2);
        recvbuf[BUF_SIZE-1] = '\0';
        printf("\r\n we have recv：%s \n",recvbuf);
        tcpclient.Close();
        //::sleep(1);
        if(strcmp(sendbuf,"end") == 0){
            break;
        }
    }
    printf("\r\nclient exiting ...\n");
    return 0;
}