#ifndef _JTHREAD_H
#define _JTHREAD_H
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct PThreadhdl;
class JThread
{
public:
	JThread();
	~JThread();
	
	//创建并启动
	virtual int run();
	
	//等待和收回资源
	static void Join(JThread& thrd);
	
	//sleep函数
	static void Msleep(int ms);
	static void Sleep(int s);
	
public:
	virtual int routine() = 0;
	
private:
//这里PThreadhdl是一个未定义的结构体，因此只能用其指针的形式定以变量
//如果定以引用或者其他形式的变量，则会出现incomplete type的错误！
	struct PThreadhdl* hThread;
	
};

#endif