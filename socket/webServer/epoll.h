//epoll模型的API

//函数epoll_create
#include <sys/epoll.h>
int epoll_create(int size);
//返回值：若成功，返回一个非负的文件描述符，若出错，返回-1。
/*
该函数返回一个文件描述符，用来唯一标示内核中这个事件表，
size参数提示内核要监听的文件描述符个数，这与内存大小有关
*/


//函数epoll_ctl
#include <sys/epoll.h>
//该函数用来操作epoll的内核事件表
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
//返回值：若成功，返回0，若出错返回-1。
//epfd就是函数epoll_create创建的句柄
/*
epfd就是函数epoll_create创建的句柄
op是指定操作类型，有一下三种:
EPOLL_CTL_ADD，向epfd注册fd的上的event
EPOLL_CTL_MOD，修改fd已注册的event
EPOLL_CTL_DEL，从epfd上删除fd的event
fd是操作的文件描述符
*/
//epoll_event定义
struct epoll_event {
    uint32_t     events;      /* Epoll events */
    epoll_data_t data;        /* User data variable */
};
/*
events成员描述事件类型，将以下宏定义通过位或方式组合
EPOLLIN :表示对应的文件描述符可以读(包括对端SOCKET正常关闭)
POLLOUT:表示对应的文件描述符可以写
EPOLLPRI:表示对应的文件描述符有紧急的数据可读(这里应该表示有带外数据到来)
EPOLLERR:表示对应的文件描述符发生错误
EPOLLHUP:表示对应的文件描述符被挂断;
EPOLLET: 将EPOLL设为边缘触发(Edge Triggered)模式,
这是相对于水平触发(Level Triggered)来说的
EPOLLONESHOT:只监听一次事件,当监听完这次事件之后,如果还需要继续监听这个socket的话,需要再次把这个socket加入到EPOLL队列里
*/
//data用于存储用户数据，是epoll_data_t结构类型，该结构定义如下：
typedef union epoll_data {
    void        *ptr;
    int          fd;
    uint32_t     u32;
    uint64_t     u64;
} epoll_data_t;
/*
epoll_data_t是一个联合体，fd指定事件所从属的目标文件描述符。
ptr可以用来指定fd相关的用户数据，但两者不能同时使用。
*/

//函数epoll_wait
//函数epoll_wait用来等待所监听文件描述符上有事件发生
#include <sys/epoll.h>
int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
//返回值：若成功，返回就绪的文件描述符个数，若出错，返回-1，时间超时返回0
/*
epfd就是函数epoll_create创建的句柄
timeout是超时事件，-1为阻塞，0为立即返回，非阻塞，大于0是指定的微妙
events是一个 传入传出参数，它是epoll_event结构的指针，用来从内核得到事件的集合
maxevents告知内核events的大小，但不能大于epoll_create()时创建的size
*/


//LT和ET模式
/*
LT（Level Triggered，电平触发）：LT模式是epoll默认的工作模式，
也是select和poll的工作模式，在LT模式下，epoll相当于一个效率较高的poll。
采用LT模式的文件描述符，当epoll_wait检测到其上有事件发生并将此事件通知应用程序后，
应用程序可以不立即处理此事件，当下一次调用epoll_wait是，
epoll_wait还会将此事件通告应用程序。

ET（Edge Triggered，边沿触发）:当调用epoll_ctl，向参数event注册EPOLLET事件时，
epoll将以ET模式来操作该文件描述符，ET模式是epoll的高效工作模式.
对于采用ET模式的文件描述符，当epoll_wait检测到其上有事件发生并将此通知应用程序后，
应用程序必须立即处理该事件，因为后续的epoll_wait调用将不在向应用程序通知这一事件。
ET模式降低了同意epoll事件被触发的次数，效率比LT模式高。
*/
 









