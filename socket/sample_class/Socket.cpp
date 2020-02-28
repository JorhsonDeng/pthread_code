#include <stdio.h>
#include "Socket.h"

/* IP地址的封装 */
SockAddr::SockAddr()
{
	iAddr.sin_family = AF_INET;
	iAddr.sin_addr.s_addr = htons(INADDR_ANY);
	iAddr.sin_port = 0;
}

SockAddr::SockAddr(const char* ip, unsigned short port)
{
	iAddr.sin_family = AF_INET;
	iAddr.sin_addr.s_addr = inet_addr(ip);
	iAddr.sin_port = htons(port);
}

SockAddr::SockAddr(const char* ip)
{
	iAddr.sin_family = AF_INET;
	iAddr.sin_addr.s_addr = inet_addr(ip);
	iAddr.sin_port = 0;
}

SockAddr::SockAddr(unsigned short port)
{
	iAddr.sin_family = AF_INET;
	iAddr.sin_addr.s_addr = htons(INADDR_ANY);
	iAddr.sin_port = htons(port);
}
SockAddr::SockAddr(sockaddr_in addr)
{
	iAddr = addr;
}

void SockAddr::SetIp(const char* ip)
{
	iAddr.sin_addr.s_addr = inet_addr(ip);
}
void SockAddr::SetIp(unsigned int ip)
{
	iAddr.sin_addr.s_addr = ip;
}
void SockAddr::SetPort(unsigned short port)
{
	iAddr.sin_port = htons(port);
}

std::string SockAddr::GetIp_str() const
{
	return std::string(inet_ntoa(iAddr.sin_addr));
}
unsigned int SockAddr::GetIp_n() const
{
	return iAddr.sin_addr.s_addr;
}
unsigned short SockAddr::GetPort() const
{
	return ntohs(iAddr.sin_port);
}

/* socket相关的工具函数 */
Socket::Socket()
:hSock(-1)		//构造函数，对句柄变量设置为-1
{
}
int Socket::SetOpt_RecvTimeout(int ms)
{
#ifdef _WIN32
	if(socket_setsockopt(hSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&ms,sizeof(int)) < 0) 
	{ 
		return  -1;
	} 
	return 0;

#else
	timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = ms % 1000 * 1000;

	if(socket_setsockopt(hSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv,sizeof(timeval)) < 0) 
	{ 
		return  -1;
	} 
	return 0;
#endif
}
int Socket::SetOpt_SendTimeout(int ms)
{
#ifdef _WIN32
	if(socket_setsockopt(hSock,SOL_SOCKET,SO_SNDTIMEO,(char*)&ms,sizeof(int)) < 0) 
	{ 
		return  -1;
	} 
	return 0;
#else
	timeval tv;
	tv.tv_sec = ms / 1000;
	tv.tv_usec = ms % 1000 * 1000;

	if(socket_setsockopt(hSock,SOL_SOCKET,SO_SNDTIMEO,(char*)&tv,sizeof(timeval)) < 0) 
	{ 
		return  -1;
	} 
	return 0;
	
#endif
}
int Socket::GetOpt_RecvTimeout()
{
#ifdef _WIN32
	int args = 0;
	socklen_t args_len = sizeof(int);
	if(socket_getsockopt(hSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&args, &args_len) < 0) 
	{ 
		return  -1;
	} 

	return args;
#else
	timeval tv;
	socklen_t tv_len = sizeof(timeval);
	if(socket_getsockopt(hSock,SOL_SOCKET,SO_RCVTIMEO,(char*)&tv, &tv_len) < 0) 
	{ 
		return  -1;
	} 

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
	
}

int Socket::GetOpt_SendTimeout()
{
#ifdef _WIN32
	int args = 0;
	socklen_t args_len = sizeof(int);
	if(socket_getsockopt(hSock,SOL_SOCKET,SO_SNDTIMEO,(char*)&args, &args_len) < 0) 
	{ 
		return  -1;
	} 

	return args;
#else

	timeval tv;
	socklen_t tv_len = sizeof(timeval);
	if(socket_getsockopt(hSock,SOL_SOCKET,SO_SNDTIMEO,(char*)&tv, &tv_len) < 0) 
	{ 
		return  -1;
	} 

	return tv.tv_sec * 1000 + tv.tv_usec / 1000;
#endif
}

int Socket::Ioctl_SetBlockedIo(bool blocked)
{
	unsigned   long   args =  blocked ? 0: 1;
	if( socket_ioctl(hSock,   FIONBIO,   (unsigned   long*)&args) < 0)
	{
		return -1;
	}
	return 0;
}

 int Socket::SetOpt_ReuseAddr(bool reuse)
 {
	int opt = reuse ? 1: 0;
	socklen_t len = sizeof(opt);
	if (socket_setsockopt(hSock, SOL_SOCKET,SO_REUSEADDR,(char*)&opt,len) < 0)
	{
		return -1;
	}

	return 0;
 }

 int Socket::GetPeerAddr(SockAddr& addr) const
 {
	 socklen_t len = sizeof(addr);
	 if(getpeername(hSock, (struct sockaddr*)&addr, &len) < 0)
	 {
		 return -1;
	 }
	 return 0;
 }

 int Socket::GetLocalAddr(SockAddr& addr) const
 {
	 socklen_t len = sizeof(addr);
	 if(getsockname(hSock, (struct sockaddr*)&addr, &len) < 0) 
	 {
		 return -1;
	 }
	 return 0;
 }
 // 返回值: >0，表示可以读或写 =0表示超时，<0表示socket不可用
int Socket::Select_ForReading(int timeout)
{
	timeval tm;
	tm.tv_sec = timeout / 1000;
	tm.tv_usec = timeout % 1000;

	fd_set fds;
	FD_ZERO(&fds); // 清空集合
	FD_SET(hSock, &fds); // 加入集合

	return  select(hSock+1, &fds, NULL, NULL, &tm);
}

int Socket::Select_ForWriting(int timeout)
{
	timeval tm;
	tm.tv_sec = timeout / 1000;
	tm.tv_usec = timeout % 1000;

	fd_set fds;
	FD_ZERO(&fds); // 清空集合
	FD_SET(hSock, &fds); // 加入集合

	return  select(hSock+1, NULL, &fds, NULL, &tm);
}

/*************** TCP SOCKET ***************/
int TcpSocket::Open(bool resue)
{
	hSock = socket_open(AF_INET, SOCK_STREAM, IPPROTO_TCP); //用默认参数
	if((int)hSock < 0)
	{
		return -1;
	}

	SetOpt_ReuseAddr(resue);

	return 0;
}

int TcpSocket::Bind(const SockAddr& addr, bool reuse)
{
	if(Open(reuse) < 0)
	{
		return -1;
	}
	if (socket_bind(hSock, (struct sockaddr*) &addr, sizeof(sockaddr_in)) < 0)
	{
		socket_close(hSock);
		hSock = -1;
		return -1;
	}

	return 0;
}

void TcpSocket::Close()
{
	if((int)hSock >= 0)
	{
		shutdown(hSock, 2);
		socket_close(hSock);
		hSock = -1;
	}
}

// 服务器
int TcpSocket::Listen(int backlog)
{
	if(socket_listen(hSock, backlog) < 0)
	{
		return -1;
	}

	return 0;
}

int TcpSocket::Accept(TcpSocket* peer)
{
	socklen_t addr_len = sizeof((*peer).sockaddr.iAddr);
	socket_t peer_handle = socket_accept( hSock, (struct sockaddr*)&((*peer).sockaddr.iAddr), &addr_len );
#ifdef _WIN32
	if (peer_handle == 0xffffffff)
	{
		return -1;
	}
#else
	if(peer_handle < 0)
	{
		return -1;
	}
#endif

	peer->hSock = peer_handle;
	return 0;
}

// 客户端
int TcpSocket::Connect(const SockAddr& addr)
{
	if(socket_connect(hSock, (struct sockaddr*) &addr, sizeof(sockaddr)) < 0)
	{
		return -1;
	}
	return 0;
}

// 发送接收
int TcpSocket::Send(const void* buf, int len)
{
	// 发送
	int n = socket_send(hSock, (const char*) buf, len, 0);
	return n;
}

int TcpSocket::Recv(void* buf, int len, int waitall)
{
	// 接收
	int flags = waitall ? MSG_WAITALL : 0;

	int n = socket_recv(hSock, (char*) buf, len, flags);
	return n;
}


/* Udp Socket */

int UdpSocket::Open(bool resue)
{
	hSock = socket(	AF_INET, 	SOCK_DGRAM, 	0 );
	if((int)hSock < 0)
	{
		return -1;
	}

	SetOpt_ReuseAddr(resue);
	return 0;
}

int UdpSocket::Open(const SockAddr& addr, bool reuse)
{
	if(Open(reuse) < 0)
	{
		return -1;
	}

	// 绑定端口
	if (socket_bind(hSock, (struct sockaddr*) &addr, sizeof(sockaddr)) < 0)
	{
		socket_close(hSock);
		hSock = -1;
		return -1;
	}

	return 0;
}

void UdpSocket::Close()
{
	if((int)hSock >= 0)
	{
		shutdown(hSock, 2);
		socket_close(hSock);
		hSock = -1;
	}
}

int UdpSocket::SendTo(const void* buf, int len, const SockAddr& peer)
{
	int  addr_len = sizeof(sockaddr_in);
	int n = socket_sendto(hSock, (const char*) buf, len, 0, (struct sockaddr*) &peer, addr_len);
	if(n < 0)
	{
		return -1;
	}

	return 0;
}

int UdpSocket::RecvFrom( void* buf, int max_len, SockAddr& peer)
{
	// 接收数据
	socklen_t  addr_len = sizeof(sockaddr_in);
	int n = socket_recvfrom(hSock, (char*) buf, max_len, 0, (struct sockaddr*) &peer, &addr_len);
	if(n < 0)
	{
		return -1;
	}

	return n;
}

int McastSocket::Open(const char* mcast_ip, int port, const char* local_ip)
{
	hSock = socket(	AF_INET, SOCK_DGRAM, 0 );
	if((int)hSock < 0)
	{
		return -1;
	}

	// 一定需要
	if(SetOpt_ReuseAddr(true) < 0)
	{
		socket_close(hSock);
		hSock = -1;
		return -1;
	}

	// 绑定ip
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
#ifdef _WIN32
	addr.sin_addr.s_addr = inet_addr(local_ip);
#else
	addr.sin_addr.s_addr = inet_addr(mcast_ip);
#endif
	addr.sin_port = htons(port);
	if (socket_bind(hSock, (struct sockaddr*) &addr, sizeof(sockaddr)) < 0)
	{
		socket_close(hSock);
		hSock = -1;
		return -1;
	}

	// 注册组播

	m_McReq.imr_multiaddr.s_addr = inet_addr(mcast_ip); 
	m_McReq.imr_interface.s_addr = inet_addr(local_ip); 
	if (setsockopt(hSock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&m_McReq, sizeof(m_McReq)) < 0)
	{ 
		socket_close(hSock);
		hSock = -1;
		return -1;
	}

	return 0;
}

// socket关闭时自动退出组播组
void McastSocket::Close()
{
	if((int)hSock >= 0)
	{
		shutdown(hSock, 2);
		socket_close(hSock);
		hSock = -1;
	}
}

#if 0
int OS_McastSock::SendTo(const void* buf, int len, const OS_SockAddr& peer)
{
	int  addr_len = sizeof(sockaddr_in);
	int n = socket_sendto(hSock, (const char*) buf, len, 0, (sockaddr*) &peer, addr_len);
	if(n < 0)
	{
		return -1;
	}

	return 0;
}
#endif

int McastSocket::RecvFrom( void* buf, int max_len, SockAddr& peer)
{
	// 接收数据
	socklen_t  addr_len = sizeof(sockaddr_in);
	int n = socket_recvfrom(hSock, (char*) buf, max_len, 0, (struct sockaddr*) &peer, &addr_len);
	if(n < 0)
	{
		return -1;
	}

	return n;
}