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
const int port = 8886;
int main(int argc,char *argv[])
{    
    if(argc<0)    
    {        
        printf("need two canshu\n");        
        return 1;    
    }    
    int sock;    
    int connfd;   
    //定以一个socketaddr_in的变量 
    struct sockaddr_in sever_address;    
    bzero(&sever_address,sizeof(sever_address));    
    sever_address.sin_family = AF_INET;//PF_INET;    
    sever_address.sin_addr.s_addr = htons(INADDR_ANY);    
    sever_address.sin_port = htons(8888);    

    sock = socket(AF_INET,SOCK_STREAM,0);     //创建一个socket
    assert(sock>=0);     

    int ret = bind(sock, (struct sockaddr*)&sever_address,sizeof(sever_address));   
    assert(ret != -1); 

    ret = listen(sock,5);    
    assert(ret != -1);   
    /*
    struct in_addr addr1,addr2;
    unsigned long int l1,l2;
    l1 = inet_addr("192.168.1.1");
    l2 = inet_addr("192.168.100.100");
    memcpy(&addr1,&l1,4);
    memcpy(&addr2,&l2,4);
    printf("\r\n %s\n",inet_ntoa(addr1));
    printf("\r\n %s \n",inet_ntoa(addr2));
    */
   char request[1024];
   int rel;
    while(1)    
    {        
        struct sockaddr_in client;   //这里重新定以一个sockaddr_in变量，作为参数传到accept函数中     
        socklen_t client_addrlength = sizeof(client);        
        connfd = accept(sock, (struct sockaddr*)&client, &client_addrlength); 
        printf("\r\n client.sin_addr.s_addr  =%s, client.sin_port = %d\n",inet_ntoa(client.sin_addr),client.sin_port);
        if(connfd<0)        
        {            
            printf("errno\n");        
        }        
        else{                
             
            memset(request,'\0',sizeof(request));              
            rel = recv(connfd,request,1024,0);               
            //request[strlen(request)+1]='\0';                
            printf("%d:%s\n",rel,request);                
            printf("successeful!\n");                
            char buf[520]="HTTP/1.1 200 ok\r\nconnection: close\r\n\r\n";//HTTP响应               
            int s = send(connfd,buf,strlen(buf),0);//发送响应                
            printf("send=%d\n",s);                
           // int fd = open("hello.html",O_RDONLY);//消息体        
           // sendfile(connfd,fd,NULL,2500);//零拷贝发送消息体       
           // close(fd);   
            close(connfd);   
            if(strcmp(request,"endout") == 0){
                printf("\r\n will exiting...\n");
                break;                
            }
            
        }    
    }                    
    printf("\r\n exiting.......\n");
    return 0;
    
}
