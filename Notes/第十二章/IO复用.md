# 第十二章：I/O(Multi-plexing)复用

**I/O复用是什么？以及为什么要使用I/O复用？**

复用：“为了提高物理设备的效率，用最少的物理要素传递最多数据时使用的技术”

why？：当服务端需并发对客户端服务时，一种解决办法是利用多进程技术；每当有客户端与服务端进行连接时服务端就开辟一块新的内存空间给新进程，用来实现与新连接的客户端进行通信。这样有个缺点，就是频繁的开辟内存空间，这需要大量的运算和内存空间。所以，使用I/O复用机制可以只需要一个进程就可以并发对客户端服务。

<img src="D:\Typora\Socket编程\第十二章\multi-proc.png" alt="image-20250114083104989" style="zoom:67%;" />

<img src="D:\Typora\Socket编程\第十二章\IO_Plexing.png" alt="image-20250114083232820" style="zoom:67%;" />

#### select调用方法和顺序

<img src="D:\Typora\Socket编程\第十二章\select_order.png" alt="image-20250114105838765" style="zoom:67%;" />

#### 第一步：设置文件描述符

使用**fd_set**数组变量将要监视的文件描述符集中到一起。

<img src="D:\Typora\Socket编程\第十二章\fd_set.png" alt="image-20250114135505262" style="zoom:67%;" />

fd_set数组中，如果某位设置为1，则表示该文件描述符是监视对象，在上图中，很明显是文件描述符1和3。

**fd_set变量中注册或更改值的操作都由下列宏完成：**

```c
FD_ZERO(fd_set* fdset): 将fd_set变量的所有位初始化为0
FD_SET(int fd, fd_set* fdset): 在参数fdset指向的变量中注册文件描述符fd的信息。
FD_CLR(int fd, fd_set* fdset): 从参数fdset指向的变量中清除文件描述符fd的信息。
FD_ISSET(int fd, fd_set* fdset): 若参数fdset指向的变量中包含文件描述符fd的信息，则返回“真”。
```

<img src="D:\Typora\Socket编程\第十二章\fd_set_1.png" alt="image-20250114140238050" style="zoom:67%;" />

#### 设置检查（监视）范围及超时

#### select函数：

```c
#include <sys/select.h>
#include <sys/time.h>
int select(int maxfd, fd_set* readset, fd_set* writeset, fd_set* exceptset, const struct timeval* timeout);
//成功时返回大于0的值，失败时返回-1
/*
maxfd:监视对象文件描述符数量
readset:将所有关注“是否存在待读取数据”的文件描述符注册到fd_set型变量，并传递其地址值。
writeset:将所有关注“是否可传输无阻塞数据”的文件描述符注册到fd_set型变量，并传递其地址值。
exceptset:将所有关注“是否发生异常”的文件描述符注册到fd_set型变量，并传递其地址值。
timeout:调用select函数后，为防止陷入无限阻塞的状态，传递超时（time-out）信息。
返回值:发生错误时返回-1，超时返回时返回0。因发生关注的事件返回时，返回大于0的值，该值是发生事件的文件描述符。
*/
```

