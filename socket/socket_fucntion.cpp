//socket编程中一些常用的函数说明


/*****************************套接字地址结构****************************/
/*
sa_family是通信类型，最常用的值是 "AF_INET"
sa_data14字节，包含套接字中的目标地址和端口信息
*/
struct sockaddr {  
     unsigned short sa_family;	/*Protocol family 2bytes address family ,AF_xxx*/
　　 char sa_data[14];          /*Address data*/
　　 }; 

/*
sockaddr_in 结构体：struct sockaddr_in中的in 表示internet，就是网络地址，
这只是我们比较常用的地址结构，属于AF_INET地址族，他非常的常用
sockaddr_in结构体解决了sockaddr的缺陷，把port和addr 分开储存在两个变量中
*/
struct in_addr { 
	unsigned long s_addr;	//4 bytes 
};
/*这里的sin 是socket_internet的缩写*/
struct sockaddr_in { 
　　short int sin_family;			/*2 bytes Protocol familty(always AF_INET)*/
　　unsigned short int sin_port; 	/*2 bytes Port number in Network byte order*/
    struct in_addr sin_addr;		/*4 bytes IP address in Network byte order*/
    unsigned char sin_zero[8];		/*8 bytes Pad to sizeof(struct sockaddr)*/
};
/*
sockaddr和sockaddr_in都是用14字节表示的，两个结构体是一样大的
不同之处：sockaddr用其余14个字节表示sa_data
而sockaddr_in把14字节拆分成sin_port,sin_addr和sin_zero
分别表示端口和ip地址。sin_zero是用来填充字节，使得两个结构体的大小保持一样
*/
//sockaddr_in结构体变量的基本配置
struct sockaddr_in ina;

bzero(&ina,sizeof(ina));

ina.sin_family=AF_INET;

ina.sin_port=htons(23);
ina.sin_addr.s_addr = inet_addr("132.241.5.10"); 

/*
  sockaddr 和 sockaddr_in的相互关系
一般先把sockaddr_in变量赋值后，强制类型转换后传入用sockaddr做参数的函数
    sockaddr_in用于socket定义和赋值
    sockaddr用于函数参数
*/

/*
  最典型的源、目的节点socket定义
对于源、目的地址和源、目的地址端口，需要建立两个socket变量
cliaddr绑定源地址和源端口
servaddr用于connect和sendto的设定目的地址和目的端口
*/
struct sockaddr_in servaddr,cliaddr;

create_socket(char *server_addr_string,unsigned int server_port)
{
//源socket赋值
       bzero(&cliaddr,sizeof(cliaddr));
       cliaddr.sin_family = AF_INET;
 //      通常TCP/UDP 协议源地址和端口都是随机的
       cliaddr.sin_addr.s_addr = htons(INADDR_ANY);
       cliaddr.sin_port = htons(0);

//目的socket赋值
       bzero(&servaddr,sizeof(servaddr));
       servaddr.sin_family = AF_INET;
       inet_aton(server_addr_string,&servaddr.sin_addr);
       servaddr.sin_port = htons(server_port);
}
/*
网络字节顺序 (Network Byte Order)      NBO(大端BE)
结构体的sin_port和sin_addr都必须是NBO

本机字节顺序 (Host Byte Order)    HBO(小端LE)
一般可视化的数字都是HBO
*/

/*
  NBO，HBO二者转换
inet_addr()    				将字符串点数格式地址转化成NBO的无符号长整型（unsigned long s_addr;）
inet_aton()(char to net)    将字符串点数格式地址转化成NBO(网络字节顺序)
inet_ntoa () (net to char)  将NBO地址(sockaddr_in.sin_addr)转化成字符串点数格式,注意函数的参数是结构体in_addr作为一个整体，不是sockaddr_in.sin_addr.s_addr
htons()    "Host to Network Short"
htonl()    "Host to Network Long"
ntohs()    "Network to Host Short"
ntohl()    "Network to Host Long"
常用的是htons(),inet_addr()正好对应结构体的端口类型和地址类型
h--host；in/n--internet; l--long;
s--short; a--ascii; 

*/

 //三种给socket赋值地址的方法
inet_aton(server_addr_string,&myaddr.sin_addr);
myaddr.sin_addr.s_addr = inet_addr("132.241.5.10")；
//INADDR_ANY转不转NBO随便
myaddr.sin_addr.s_addr = htons(INADDR_ANY);  
myaddr.sin_addr.s_addr = INADDR_ANY;

//两种给socket 赋值端口的方法
#define MYPORT 3490 
myaddr.sin_port = htons(MYPORT);
//0（随机端口）转不转NBO随便
myaddr.sin_port = htons(0);
myaddr.sin_port = 0;  


/*
htons/l和ntohs/l等数字转换都不能用于地址转换，
因为地址都是点数格式，所以地址只能采用数字/字符串转换如inet_aton,inet_ntoa;
唯一可以用于地址转换的htons是针对INADDR_ANY
*/
 cliaddr.sin_addr.s_addr = htons(INADDR_ANY)
 
 /*
 inet_addr()与inet_aton()的区别
*/
//inet_addr()    是返回值型
struct sockaddr_in ina;

ina.sin_addr.s_addr = inet_addr("132.241.5.10"); 


//inet_aton()     是参数指针型
struct sockaddr_in ina;

inet_aton("132.241.5.10",&ina.sin_addr);

/*
inet_ntoa  将NBO地址转化成字符串点数格式
参数：结构体变量.sinaddr
返回值：字符串指针
*/
a1 = inet_ntoa(ina.sin_addr);
printf("address 1: %s\n",a1); 
address 1: 132.241.5.10 

/*
 inet_addr()的缺陷：必须对-1做检测处理
因为inet_addr()的结果是整型，而发生错误时返回-1。
而 ina.sin_addr.s_addr是unsigned long型
-1在long short显示成111111111,和IP地址255.255.255.255相符合！会被误认为广播地址！
*/


/****************************socket函数***************************/
/*
客户端和服务端使用socket函数来创建一个套接字描述符(socket descriptor)
*/
#include<sys/types.h>
#include<sys/socket.h>

int socket(int domain, int type, int protocol);	//返回一个非负数描述符，若出错则返回-1

//如果想使套接字成为连接的一个端点，就用如下编码的参数来调用socket函数
clientfd = socket(AF_INET,SOCK_STREAM,0);
/*
其中，AF_INET表明我们正在使用32位的IP地址，而SOCK_STREAM表明这个套接字是连接的一个端点
不过最好的方法是用getaddrinfo函数来自动生成这些参数，这样代码就与协议无关了
*/


/********************************connect函数************************************/
//客户端通过调用connect函数来建立和服务器的连接
#include<sys/socket.h>
int connect(int clientfd, const struct sockaddr* addr,
				socklen_t addrlen);		//返回：若成功则为0；出错则为-1
/*
*clientfd是创建socket得到的socket句柄。
*其中addrlen是sizeof(socketaddr_in).connect函数会阻塞，一直到连接成功建立或者发生错误
*如果成功，clientfd描述符现在就准备好可以读写了，并且得到的连接是由套接字对
*(x:y, addr.sin_addr:addr.sin_port)刻画的，其中x表示客户端的IP地址，而y表示的是临时端口
*它唯一的确定了客户端主机上的客户端进程。对于socket，最好的方法是用getaddrinfo来为connect
*提供参数
*/
/*
        clientfd = socket(AF_INET,SOCK_STREAM,0);
        assert(clientfd>=0);
        retVal = connect(clientfd, (struct sockaddr*)&client_address,sizeof(client_address));
        int s = send(clientfd,sendbuf,strlen(sendbuf),0);
		recv(clientfd,recvbuf,BUF_SIZE-2,0);
		close(clientfd);
*/


/*************************bind函数****************************/
#include<sys/socket.h>

int bind(int sockfd, const struct sockaddr *addr,
			socklen_t addrlen);	//返回：成功则为0，出错则为-1
/*
*bind函数告诉内核将addr中的服务器套接字地址和套接字描述符sockfd联系起来。
参数addrlen就是sizeof(sockaddr_in).对于socket和connect，
*最好的方法使用getaddrinfo来为bind提供参数
*/



/*****************************listen函数******************************/
/*
*客户端是发起连接请求的主动实体。服务器是等待来自客户端的连接请求的被动实体。
默认情况下，内核会认为socket函数创建的描述符对应于主动套接字(active socket)
他存在于一个连接的客户端。
服务器调用listen函数高度内核，描述符是被服务器而不是客户端使用的
*/
#include<sys/socket.h>

int listen(int sockfd, int backlog);
/*
*listen函数将sockfd从一个主动套接字转化为一个监听套接字(listening socket)
*该套接字可以接受来自客户端的连接请求。backlog参数暗示了内核在开始拒绝来凝结请求之前，
*队列涨要排队的未完成的请求的数量。backlog参数的确切含义要求对TCP/IP协议的理解；
*通常我们会把它设置为一个较大的值。
*/


/*************************accep函数****************************/
//服务器通过调用accept函数来等待来自客户端的连接请求。
#include<sys/socket.h>

int accept(int listenfd, struct sockaddr *addr, int *addrlen);
//返回：成功则返回一个非负数连接描述符，失败则返回-1
/*
参数，sockaddr是一个sockaddr_in类型的变量，只不过在传入这个函数中的时候，需要转换成sockaddr类型
int *addrlen是传入这个sockaddr变量的大小，这里将客户端的IP和port等参数会填充到sockaddr变量中
通过inet_ntoa(addr.sin_addr)可以将这个参数转换成字符串点类型
*/
/*
*accept函数等待来自客户端的连接请求到达侦听描述符listenfd，然后在addr中填写客户端的套接字地址
*并返回以恶搞已连接描述符(connected descriptor),这个描述符可被用来利用 Unix I/O函数与客户端通信
*监听描述符和已连接描述符之间的区别是很多人感到迷惑。监听描述符是是作为客户端连接请求的一个端点。
*它通常被创建一次，并存在于服务器的整个生命周期。已连接描述符是客户端和服务端之间已经建立起来了的一个端点。
*服务器每次接受连接请求时都会创建一次，它只存在于服务器为一个客户端服务的过程中。

*step1:服务器调用accept函数，等待连接请求到达监听描述符，具体地我们设定为描述符3. 描述符0~2是预留给了标准文件的
*step2：客户端调用connect函数，发送一个连接请求到listenfd.
*step3：accept函数打开了一个新的已连接描述符connfd(假设描述符4)，在clientfd和connfd之间建立连接，
*并随后返回connfd给应用程序。客户端从connect返回，在这一点后，客户端和服务器就可以通过读和写
*clientfd和connfd来回传送数据。
*/
 
 /****************************sendfile函数******************************/
 /*
 *sendfile函数在两个文件描述符之间直接传递数据（完全在内核空间中操作），从而避免了内核缓冲区和用户缓冲之间的数据拷贝，效率很高，这被称为零拷贝。
 *
 */
 
 #include <sys/sendfile.h>
ssize_t sendfile(int out_fd,int in_fd,off_t *offset,size_t count);
/*
*in_fd必须指向真实的文件，而out_fd则必须是一个socket。
由此可见，sendfile几乎是专门为在网络上传输文件而设计的。
利用sendfile函数将服务器上的一个文件传输给客户端。
代码没有给目标文件分配任何用户空间的缓存，也没用执行读取文件的操作，
但同样实现了文件的发送，其效率显然要高的多。
*/

 
 /*********************主机和服务之间的转换*****************/
 /*
 *Linux提供了一些强大的函数(称为getaddrinfo和getnameinfo)实现二进制套接字地址结构
 *和主机名，服务名和端口号的字符串表示之间的相互转化。当和套接字接口一起使用时，这些函数能
 *使我们编写独立于任何特定版本的IP协议的网络程序。
 */
 /*****************************getaddrinfo函数*******************/
 /*
 *getaddrinfo函数是将主机名，主机地址，服务名和端口号的字符串表示转化为套接字地址结构
 *它是已弃用的gethostbyname和getserverbyname函数的新的替代品。
 *和以往的那些函数不同，这个函数是可重入的，适用于任何协议。
 */
 #include<sys/types.h>
 #include<sys/socket.h>
 #include<netdb.h>
 
 int getaddrinfo(const char *host, cost char *service,
				const struct addrinfo *hints,
				struct addrinfo **result);
				//若成功则返回0，不成功则返回错误代码
void freeaddrinfo(struct addrinfo *result); 

const char* gai_strerror(int errcode);	//返回错误信息
 
 
 
 
 
 
 
 
 
 
 
 
/**************************************************************/
#include <string.h>
// bzero() 会将内存块（字符串）的前n个字节清零;
// s为内存（字符串）指针，n 为需要清零的字节数。
// 在网络编程中会经常用到。
void bzero(void *s, int n);

#include <assert.h>
void assert( int expression );
//assert的作用是先计算表达式 expression ，如果其值为假（即为0），
//那么它先向stderr打印一条出错信息，然后通过调用 abort 来终止程序运行
 
 
 