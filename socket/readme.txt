release 1.0
sample文件夹下面主要添加的是一个server和client socket的程序
用于演示socket编程是如何进行的

release 2.0
将socket中的一些变量和函数发封装成类，进行调用

release 3.0
在release 2.0的基础上，增加pthread.h,将server编程多线程进行编程

release4.0
添加一个web server代码

release5.0
修改sample_class_thread文件夹下的内容
利用多线程和信号量/互斥量来处理客户端的连接
基本原理：在一个类对象中创建多个线程，将client的连接放到
对象中的链表中，用信号量和互斥量进行保护。
不足之处在于：每个线程现在只能处理一个连接，不能处理多个
连接，后续可能需要像release4.0中的代码，用epoll机制，这样
在一个线程中处理完一个client端的请求之后，不用再一直等待
这个client是否释放，可以回到互斥量部分继续等待其他的连接。
