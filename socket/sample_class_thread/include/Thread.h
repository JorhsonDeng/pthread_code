#ifndef _Thread_H
#define _Thread_H
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

#endif