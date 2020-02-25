#include<stdio.h>
#include"JThread.h"

int exit_flag = 0;
class myThread:public JThread
{
public:
	virtual int routine()
	{
		while(!exit_flag){
			printf("\r\n myThread is running...\n");
			JThread::Sleep(1);
		}
		printf("\r\n myThread is exiting...\n");
		return 0;
	}
};

int main()
{
	myThread thrd;
	thrd.run(); 
	getchar();
	exit_flag = 1;
	thrd.Join(thrd);	
	//由于Join()是静态函数，因此也可以直接调用JThread::Join(thrd); 
	printf("\r\n main thread is exiting...\n");
	return 0;
}