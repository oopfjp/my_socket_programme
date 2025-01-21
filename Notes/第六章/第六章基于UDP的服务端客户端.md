

# 第六章基于UDP的服务端/客户端

### UDP套接字的特点

它是一种不可靠的数据传输服务。UDP在结构上比TCP更简洁；UDP不会发送类似ACK的应答消息，也不会向SEQ那样给数据包分配序号。因此，**UDP的性能有时比TCP高出很多**，编程中实现UDP也比TCP简单。另外，UDP的可靠性虽比不上tcp，但也不会像想象中那么频繁地发生数据损毁。因此，在更重视性能而非可靠性的情况下，UDP是一种很好的选择。**为了提供可靠的数据传输服务，TCP在不可靠的IP层进行流控制，而UDP就缺少这种流控制机制。**

### 实现基于UDP的服务端/客户端

UDP只有创建套接字的过程和数据交换的过程。

#### UDP服务端和客户端均只需1个套接字

TCP中，套接字之间是一对一的关系。若要像n个客户端提供服务，则除了守门的服务器套接字外，还需要n个服务器端套接字。**但在UDP中，不管是服务端还是客户端都只需要1个套接字。**

<img src="D:\Typora\Socket编程\第六章\UDP_socket_communication.png" alt="image-20250101191159967" style="zoom:67%;" />

上图展示了仅需一个套接字就可以和多个主机进行通信。

#### 基于UDP的数据I/O函数

由于UDP套接字不像TCP套接字会保持与对方套接字的连接，因此每次传输数据都要添加目标地址信息。这相当于寄信前在信件中填写地址。

```c
/*UDP套接字发送数据函数*/
#include <sys/socket.h>
ssize_t sendto(int sock, void * buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen);
//成功时返回传输的字节数，失败时返回-1
//其中to存有目标地址信息的sockaddr结构体变量的地址值。
//addrlen表示传递给参数to的地址值结构体变量的长度。


```

****

**关于sendto函数**

用于在无连接的协议（如 UDP）中发送数据报。它需要指定目标地址，因为 UDP 数据包是独立发送的，没有像 TCP 那样的连接状态。

sendto函数在调用时，系统会自动为该主机绑定ip地址和端口号。所以在客户端中无需手动的绑定ip地址和端口号。

****

****

**关于recvfrom函数**

用于在无连接的协议（如 UDP）中接收数据报。它除了返回数据外，还需要提供发送方的地址信息，以便程序知道数据是从哪里来的。

****

```c
/*UDP套接字接收数据函数*/
#include <sys/socket.h>
ssize_t recvfrom(int sock, void * buff, size_t nbytes, int flags, struct sockaddr *from, socklen_t* addrlen);
//成功时返回传输的字节数，失败时返回-1
//其中from存有发送端地址信息的sockaddr结构体变量的地址值。
//addrlen表示保存参数from的结构体变量长度的变量地址值。
```

#### UDP总体的流程如下：

1. **客户端发送数据到服务端**：
   客户端调用sendto，地址参数填写服务端的地址192.168.1.100:8080

2. **服务端接收数据：**
   服务端调用recvfrom，地址参数填写一个本地struct sockaddr_in变量的指针（这个变量可以预先设置为服务端的地址，但不是必须的）。

   recvfrom成功返回后，该变量会被填充为客户端的地址和端口号。

3. **服务端发送数据给客户端**：
   服务端调用sendto，地址参数填写recvfrom函数获取到的客户端地址信息。

4. **客户端接收数据：**
   客户端调用recvfrom，地址参数填写一个本地struct sockaddr_in变量的地址。
   recvfrom成功返回后，该变量会被填充为服务端的地址192.168.1.100:8080。

#### UDP客户端的connect和非connect状态：

不管是用TCP还是UDP来传输数据，传输数据前都需要绑定自身的ip地址和端口号。服务端通过调用bind函数可以绑定，而客户端的绑定地址在TCP和UDP是有差异的；TCP的客户端在调用connect函数后系统会自动给客户端绑定ip地址和端口号；而**UDP客户端是通过在sendto函数调用后，系统会自动给客户端分配ip地址和端口号。**

```c
//UDP客户端的connect调用
connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
//成功返回0，失败返回-1
```

在非connect的客户端中，每次调用sendto函数时，系统都会重新为客户端分配ip地址和端口号，其特点是端口号每次是随机分配的；

而在connect的客户端中，初次调用sendto函数时，系统分类给客户端对应的ip地址和端口号，然后系统会将客户端的IP地址和端口号进行注册，以后再调用sendto时就直接使用注册好的地址信息，而不用重新分配了，这样加快了传输的速率。
