#include"JThread.h"
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

JThread::JThread()
{
	
}

JThread::~JThread()
{
	
}

int JThread::run()
{
#if 1
	if(pthread_create(NULL,
						NULL,
						NULL,
						NULL)){
		printf("\r\n Create thread failed!\n");
		return -1;					
	}
#endif
	return 0;
}


void JThread::Join(JThread& thrd)
{
	pthread_join(NULL,NULL);
}

void JThread::Sleep(int s)
{
	::sleep(1);
}

void JThread::Msleep(int ms)
{
	//::usleep(ms * 1000);
	// 好像使用nanosleep更好

	timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = (ms % 1000) * 1000000;
	nanosleep(&ts, NULL);
}

int JThread::routine()
{
	return 0;
}

