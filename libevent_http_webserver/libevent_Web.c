#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

	return 0;
}
