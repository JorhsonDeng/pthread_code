#include"Thread.h"
/*
*现在一直出现pthread.h文件中的一些变量和函数未定义的情况
*eg:pthread_t, pthread_create,pthread_join, 不知道为什么一直提示找不到！
*原因是在JThread.h头文件中，定以的宏和pthread.h文件中的宏是一样的，
*导致pthread.h文件中的函数无法被引用
*/

struct PThreadhdl
{
	pthread_t pthrdhdl;
};

Thread::Thread()
:hThread(NULL)
{
	
}

Thread::~Thread()
{
	if(hThread!=NULL)
		delete hThread;
}
/*
*这里重新定以一个void*的函数，作为pthread_create函数的参数传递进去
*context作为类对象传进来，然后调用routine函数执行相应的内容
*/
static void* OS_Thread_Proc_Linux(void* param)
{
	Thread* thrd = (Thread*) param;
	thrd->routine();
	return NULL;
}
int Thread::run()
{
	if(this->hThread== NULL)
		this->hThread = new PThreadhdl;
	/* 记得第一个参数是指针的形式*/
	if(pthread_create(&(this->hThread->pthrdhdl),
						NULL,OS_Thread_Proc_Linux,this)<0){
		delete this->hThread;
		this->hThread = NULL;
		printf("\r\n Create thread failed!\n");
		return -1;					
	}
	return 0;
}


void Thread::Join(Thread& thrd)
{
	pthread_join(thrd.hThread->pthrdhdl,NULL);
}

void Thread::Sleep(int s)
{
	::sleep(1);
}

void Thread::Msleep(int ms)
{
	//::usleep(ms * 1000);
	// 好像使用nanosleep更好

	timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

int Thread::routine()
{
	return 0;
}

