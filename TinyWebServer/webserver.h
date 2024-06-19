#ifndef WEBSERVER_H
#define WEBSERVER_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>
#include <string>

using namespace std;

class WebServer
{
public:
	WebServer();
	~WebServer();

	void init(int port, string user, string password, string databaseName,
			int log_write, int opt_linger, int trigmode, int sql_num,
			int thread_num, int close_log, int actor_model);
	void log_write();
	

public:
	//基础
	int m_port;
	char* m_root;
	int m_log_write;
	int m_close_log;
	int m_actormodel;
	
	//数据库相关
	string m_user;		//登录数据库用户名
	string m_password;	//登录数据库密码
	string m_databaseName;	//使用数据库名
	int m_sql_num;

	//线程池相关
	int m_thread_num;

	int m_OPT_LINGER;
	int m_TRIGMode;

};


#endif
