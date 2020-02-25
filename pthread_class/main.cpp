#include<stdio.h>
#include"Thread.h"

int exit_flag = 0;
class myThread:public Thread
{
public:
	virtual int routine()
	{
		while(!exit_flag){
			printf("\r\n myThread is running...\n");
			Thread::Sleep(1);
		}
		printf("\r\n myThread is exiting...\n");
		return 0;
	}
};

int main()
{
	myThread thrd;
	thrd.run();
	thrd.Join(thrd);
	printf("\r\n main thread is exiting...\n");
	return 0;
}