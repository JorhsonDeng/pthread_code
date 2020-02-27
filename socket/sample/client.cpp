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
using namespace std;

const int port = 8889;
#define BUF_SIZE 1000
int main()
{
    int clientfd;    
    int retVal;   
    char sendbuf[BUF_SIZE];
    char recvbuf[BUF_SIZE];
    //定以一个socketaddr_in的变量 用于设置服务器端的IP地址和端口
    struct sockaddr_in client_address;    
    bzero(&client_address,sizeof(client_address));    
    client_address.sin_family = AF_INET;//PF_INET;    
    client_address.sin_addr.s_addr = htons(INADDR_ANY);    
    client_address.sin_port = htons(8888);    
    //这里创建一个client的socket描述符
 


    while(true){  
        clientfd = socket(AF_INET,SOCK_STREAM,0);
        assert(clientfd>=0);
        retVal = connect(clientfd, (struct sockaddr*)&client_address,sizeof(client_address));
        
        if(retVal<0)      
        {          
            cout << "connect failed!" << endl;            
            close(clientfd); //关闭套接字                   
            return -1;      
        }
        
        cout<<"please input the string youwant send!"<<endl;
        memset(sendbuf,0,sizeof(sendbuf));
        cin>>sendbuf;
        int s = send(clientfd,sendbuf,strlen(sendbuf),0);
        printf("\r\n we have send data length is %d",s);
        recv(clientfd,recvbuf,BUF_SIZE-2,0);
        recvbuf[BUF_SIZE-1] = '\0';
        printf("\r\n we have recv：%s \n",recvbuf);
        close(clientfd);
        ::sleep(1);
        if(strcmp(sendbuf,"endout") == 0){
            break;
        }
    }
    printf("\r\nclient exiting ...\n");
    return 0;
}