#include<stdio.h>
#include"Thread.h"

int exit_flag = 0;
class myThread:public Thread
{
public:
	//virtual int routine() //这里的virtual可加可不加了，也是虚函数！
	int routine()
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
	getchar();
	exit_flag = 1;
	thrd.Join(thrd);	
	//由于Join()是静态函数，因此也可以直接调用Thread::Join(thrd); 
	printf("\r\n main thread is exiting...\n");
	return 0;
}