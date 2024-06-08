#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <event.h>
#include <event2/listener.h>

//提取套接字时的回调函数
//链接侦听器的地址 | cfd | 客户端的地址信息 | 侦听器传来的参数
void listen_cb(struct evconnlistener *listener, evutil_socket_t fd, struct sockaddr *cliaddr, int socklen, void *arg)
{
	
}

int main()
{
	char workdir[256] = {0};
	strcpy(workdir, getenv("PWD"));//获取当前工作目录
	//printf("%s\n", workdir);
	chdir(workdir);//更改当前工作路径--为获取目录、资源等做准备

	//创建根节点
	struct event_base *base = event_base_new();

	//创建侦听器-一键完成套接字的创建、绑定、监听、提取
	//准备侦听器参数：绑定的地址信息
	struct sockaddr_in serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(9999);
	serv.sin_addr.s_addr = htonl(INADDR_ANY);
	//根节点 | 套接字创建、绑定、监听后，提取时的回调函数 | 回调函数参数 | flag：释放资源时自动关闭套接字、端口复用 | 
	//-1 | 绑定的服务端地址结构 | 绑定的地址结构的长度
	struct evconnlistener *listener = evconnlistener_new_bind(
		base, listen_cb, base, LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1,
							(struct sockaddr*)&serv, sizeof(serv));//连接侦听器
	
	//设置循环监听
	event_base_dispatch(base);

	//释放根节点
	event_base_free(base);

	//释放链接侦听器
	evconnlistener_free(listener);

	return 0;
}
