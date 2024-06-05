#include "stdio.h"
#include "wrap.h"
#include "sys/epoll.h"
#include <fcntl.h>
#include <sys/stat.h>
#include "pub.h"
#define PORT 6666

void send_header(int cfd, int code, char *info, char *filetype, int length)
{
	//发送状态行
	char buf[1024] = "";
	int len = 0;
	len = sprintf(buf, "HTTP/1.1 %d %s\r\n", code, info);
	send(cfd, buf, len, 0);
	//发送消息头
	len = sprintf(buf, "Content-Type:%s\r\n", filetype);
	send(cfd, buf, len, 0);
	if(length > 0)
	{
		//发送消息头
		len = sprintf(buf, "Content-Length:%d\r\n", length);
		send(cfd, buf, len, 0);
	}
	//空行
	send(cfd, "\r\n", 2, 0);
}

void send_file(int cfd, char *path, struct epoll_event *ev, int epfd, int flag)
{
	int fd = open(path, O_RDONLY);
	if(fd < 0)
	{
		perror("open");
		return ;
	}
	char buf[1024] = "";
	int len = 0;
	while(1)
	{
		len = read(fd, buf, sizeof(buf));
		if(len < 0)
		{
			perror("read");
			break;
		}
		else if(len == 0)
		{
			break;
		}
		else
		{
			int n = 0;
			n = send(cfd, buf, len, 0);
			//printf("len=%d\n", n);
		}
	}
	close(fd);
	//关闭cfd，下树
	if(flag == 1)
	{
		close(cfd);
		epoll_ctl(epfd, EPOLL_CTL_DEL, cfd, ev);
	}
}

void read_client_request(int epfd, struct epoll_event *ev)
{
	//读取请求（先读取一行，再把其它行读取，扔掉）
	char buf[1024] = "";
	char tmp[1024] = "";
	int n = Readline(ev->data.fd, buf, sizeof(buf));
	if(n <= 0)
	{
		printf("close or error\n");
		epoll_ctl(epfd, EPOLL_CTL_DEL, ev->data.fd, ev);
		close(ev->data.fd);
		return;
	}
	printf("[%s]\n", buf);
	int ret =0;
	while(  (ret = Readline(ev->data.fd, tmp, sizeof(tmp))) >0);
	//printf("read ok\n");
	//解析请求 GET /a.txt  HTTP/1.1\R\N
	char method[256] = "";
	char content[256] = "";
	char protocol[256] = "";
	sscanf(buf, "%[^ ] %[^ ] %[^ \r\n]", method, content, protocol);
	printf("[%s]  [%s]  [%s]\n", method, content, protocol);
	//判断是否为get请求 GET
	if(strcasecmp(method, "get") == 0)
	{
		char *strfile = content + 1;
		if(*strfile == 0)//如果没有请求文件，默认请求当前目录
		{
			strfile = "./";
		}
		//判断请求的文件在不在
		struct stat s;
		if(stat(strfile, &s) < 0)//文件不存在
		{
			printf("file not found\n");
			//先发送 报头（状态行 消息头 空行）
			send_header(ev->data.fd, 404, "NOT FOUND", get_mime_type("*.html"), 0);
			//发送文件 error.html
			send_file(ev->data.fd, "error.html", ev, epfd, 1);		
		}
		else
		{
			//请求的是一个普通的文件
			if(S_ISREG(s.st_mode))
			{
				printf("file\n");
				//先发送 报头（状态行 消息头 空行）
				send_header(ev->data.fd, 200, "OK", get_mime_type(strfile), s.st_size);
				//发送文件
				send_file(ev->data.fd, strfile, ev, epfd, 1);		
			}
			else if(S_ISDIR(s.st_mode))//请求的是一个目录
			{
				printf("dir\n");
			}
		}
	}
	//得到浏览器请求的文件,如果对方没有请求文件，默认请求./当前目录
	//判断文件是否存在，如果存在（普通文件，目录）
	//不存在，发送error.html
}

int main()
{
	//切换工作目录
	//获取当前目录的工作路径
	char pwd_path[256] = "";
	char * path = getenv("PWD");
	strcpy(pwd_path, path);
	strcat(pwd_path, "/web-http");
	chdir(pwd_path);

	//创建套接字
	int lfd = tcp4bind(PORT, NULL);
	//监听
	Listen(lfd, 128);
	//创建树
	int epfd = epoll_create(1);
	//将lfd上树
	struct epoll_event ev, evs[1024];
	ev.data.fd = lfd;
	ev.events = EPOLLIN;
	epoll_ctl(epfd, EPOLL_CTL_ADD, lfd, &ev);
	//循环监听
	while(1)
	{
		int nready = epoll_wait(epfd, evs, 1024, -1);
		if(nready < 0)
		{
			perror("epoll_wait");
			break;
		}
		else
		{
			for(int i = 0; i < nready; ++i)
			{
				//判断是否是lfd
				if(evs[i].data.fd == lfd && evs[i].events & EPOLLIN)
				{
					struct sockaddr_in cliaddr;
					char ip[16];
					socklen_t len = sizeof(cliaddr);
					int cfd = Accept(lfd, (struct sockaddr*)&cliaddr, &len);
					printf("new client ip=%s port=%d\n", 
						inet_ntop(AF_INET, &cliaddr.sin_addr.s_addr, ip, 16),
						ntohs(cliaddr.sin_port));
					//设置cfd为非阻塞
					int flag = fcntl(cfd, F_GETFL);
					flag |= O_NONBLOCK;
					fcntl(cfd, F_SETFL, flag);
					//上树
					ev.data.fd = cfd;
					ev.events = EPOLLIN;
					epoll_ctl(epfd, EPOLL_CTL_ADD, cfd, &ev);
				}
				else if(evs[i].events & EPOLLIN)//cfd 变化
				{
					read_client_request(epfd, &evs[i]);
				}
			}
		}
	}
	//

	return 0;
}
