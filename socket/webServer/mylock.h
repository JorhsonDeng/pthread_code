#ifndef _MYLOCK_H
#define _MYLOCK_H
#include<iostream>
#include<list>
#include<cstdio>
#include<semaphore.h>
#include<exception>
#include<pthread.h>
#include"myhttp_coon.h"
using namespace std; /*封装信号量*/
class sem
{
private:    
    sem_t m_sem;
public:    
    sem();    
    ~sem();    
    bool wait();//等待信号量    
    bool post();//增加信号量
};
//创建信号量
sem::sem()
{    
    //create semaphore variable
    if(sem_init(&m_sem,0,0) != 0)    
    {        
        throw std ::exception();    
    }
}
//销毁信号量
sem::~sem()
{    
    sem_destroy(&m_sem);
}
//等待信号量
bool sem::wait()
{    
    return sem_wait(&m_sem) == 0;
}
//增加信号量
bool sem::post()
{    
    return sem_post(&m_sem) == 0;
} 
/*封装互斥锁*/
class locker
{
private:    
    pthread_mutex_t m_mutex;
public:    
    locker();    
    ~locker();    
    bool lock();    
    bool unlock();
}; 
locker::locker()
{    
    //initialize the mutex variable
    if(pthread_mutex_init(&m_mutex, NULL) != 0)    
    {        
        throw std::exception();    
    }
} 
locker::~locker()
{    
    pthread_mutex_destroy(&m_mutex);
}
/*上锁*/
bool locker::lock()
{    
    return pthread_mutex_lock(&m_mutex)==0;
}
/*解除锁*/
bool locker::unlock()
{    
    return pthread_mutex_unlock(&m_mutex) == 0;
} 
/*封装条件变量*/
class cond
{
private:    
    pthread_mutex_t m_mutex;    
    pthread_cond_t m_cond;
public:    
    cond();    
    ~cond();    
    bool wait();    
    bool signal();
}; 
cond::cond()
{    
    if(pthread_mutex_init(&m_mutex,NULL)!=0)    
    {        
        throw std::exception();    
    }    
    if(pthread_cond_init(&m_cond, NULL)!=0)    
    {        
        throw std::exception();    
    }
} 
cond::~cond()
{    
    pthread_mutex_destroy(&m_mutex);    
    pthread_cond_destroy(&m_cond);
} 
/*等待条件变量*/
bool cond::wait()
{    
    int ret;    
    pthread_mutex_lock(&m_mutex);    
    ret = pthread_cond_wait(&m_cond,&m_mutex);    
    pthread_mutex_unlock(&m_mutex);    
    return ret == 0;
} 
/*唤醒等待条件变量的线程*/
bool cond::signal()
{    
    return pthread_cond_signal(&m_cond) == 0;
} 
#endif
