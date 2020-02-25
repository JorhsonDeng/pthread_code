#include<stdio.h>
#include<stdlib.h>

#include<unistd.h>
#include<pthread.h>

struct thread_para{
	int pid;
	int num;
};
int exit_flag = 0;

void *Main_thread(void* context)
{
	thread_para *para = (thread_para*)context;
	while(!exit_flag){
		para->num++;
		printf("\r\n Main_thread running...pthreadID = %d, number = %d",para->pid,para->num);	
		::sleep(1);
	}
	printf("\r\n threadID:%d is exiting!!!",(*para).pid);
}

int main()
{	
	pthread_t h1,h2;
	
	//创建第一个线程
	thread_para* para1 = (thread_para*)malloc(sizeof(thread_para));
	para1->pid = 1;
	para1->num = 0;
	if(pthread_create(&h1,NULL,Main_thread,para1)<0){
		printf("\r\n create thread failed!!!");	
	}
	
	//创建第一个线程
	thread_para* para2 = (thread_para*)malloc(sizeof(thread_para));
	para2->pid = 2;
	para2->num = 0;
	if(pthread_create(&h2,NULL,Main_thread,para2)<0){
		printf("\r\n create thread failed!!!");	
	}
	
	getchar();
	exit_flag = 1;
	pthread_join(h1,NULL);
	pthread_join(h2,NULL);
	
	//记得释放之前malloc的资源
	free(para1);
	free(para2);
	//主函数在其他线程退出之后在进行退出！！！
	printf("\r\n main thread is exiting...\n");
	return 0;
}
