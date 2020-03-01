// linux下的socket定义
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <sys/time.h>
#include <fcntl.h>
#include <iostream>
typedef int socket_t;

#define socket_open socket
#define socket_close close
#define socket_ioctl  ioctl

#define socket_accept  accept
#define socket_bind      bind
#define socket_connect connect
#define socket_listen    listen
#define socket_send     send
#define socket_recv      recv
#define socket_sendto   sendto
#define socket_recvfrom recvfrom
#define socket_select    select
#define socket_setsockopt setsockopt
#define socket_getsockopt getsockopt


/* 将sockaddr_in结构体进行封装，可以直接强转成sockaddr_in结构*/
class SockAddr
{
public:
	explicit SockAddr();
	explicit SockAddr(const char* ip, unsigned short port);
	explicit SockAddr(const char* ip); // 默认端口为0
	explicit SockAddr(unsigned short port); // 默认IP为0.0.0.0
	explicit SockAddr(sockaddr_in addr);

	void SetIp(const char* ip);
	void SetIp(unsigned int ip);
	void SetPort(unsigned short port);
/* 
在类成员函数的声明和定义中，
const的函数不能对其数据成员进行修改操作。
const的对象，不能引用非const的成员函数
*/
	std::string GetIp_str() const;
	unsigned int GetIp_n() const;
	unsigned short GetPort() const;

public:
	sockaddr_in iAddr;
};

/*对socket函数进行二次封装，主要是为了创建一个socket*/
class Socket
{
public:
	Socket(); 
	// ms=0时永不超时, 单位ms, ms=1可以认为是立即返回(1ms很快完成)
	int SetOpt_RecvTimeout(int ms); 
	int SetOpt_SendTimeout(int ms);
	int GetOpt_RecvTimeout(); 
	int GetOpt_SendTimeout();

	int Ioctl_SetBlockedIo(bool blocked);
	int SetOpt_ReuseAddr(bool reuse);

	int GetPeerAddr(SockAddr& addr) const;
	int GetLocalAddr(SockAddr& addr) const;

	// select机制:查询读写状态
	// 返回值: >0，表示可以读或写 =0表示超时，<0表示socket不可用
	int Select_ForReading(int timeout);
	int Select_ForWriting(int timeout);

public:
	socket_t hSock; // 可以直接访问这个handle，创建socket成功之后会返回一个句柄
	//在这里买呢添加了一个SockAddr对象变量，主要用于存放连接套接字中客户端传过来的套接字地址信息
	class SockAddr sockaddr;  
};

class TcpSocket : public Socket
{
public:
	int Open(bool resue = false);	//这里设置默认参数，如果不传入的话，则用的就是默认参数
	int Bind(const SockAddr& addr , bool reuse = false);

	void Close();

	// 服务器
	int Listen(int backlog = 16);
	int Accept(TcpSocket* peer);
	
	// 客户端
	int Connect(const SockAddr& addr);

	// 发送接收
	int Send(const void* buf, int len);
	int Recv(void* buf, int len, int waitall=0);

};

class UdpSocket : public Socket
{
public:
	int Open(bool resue = false);
	int Open(const SockAddr& addr,  bool reuse = false);

	void Close();

	int SendTo(const void* buf, int len, const SockAddr&  peers);
	int RecvFrom( void* buf, int max_len, SockAddr& peer);
};

class McastSocket : public Socket
{
public:
	int Open(const char* mcast_ip, int port, const char* local_ip);
	void Close();

	/* 发送多播时，使用普通UdpSock + 多播路由即可 */
	//int SendTo(const void* buf, int len, const OS_SockAddr& peer);
	int RecvFrom( void* buf, int max_len, SockAddr& peer);

private:
	ip_mreq m_McReq;
};