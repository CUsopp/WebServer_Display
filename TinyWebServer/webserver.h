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
#include <signal.h>
#include "./log/log.h"
#include "./CGImysql/sql_connection_pool.h"
#include "./http/http_conn.h"
#include "./threadpool/threadpool.h"
#include "./timer/lst_timer.h"

using namespace std;

const int MAX_FD = 65535;		//最大文件描述符
const int TIMESLOT = 5;             	//最小超时单位
const int MAX_EVENT_NUMBER = 10000; 	//最大事件数

class WebServer
{
public:
	WebServer();
	~WebServer();

	void init(int port, string user, string password, string databaseName,
			int log_write, int opt_linger, int trigmode, int sql_num,
			int thread_num, int close_log, int actor_model);
	void log_write();
	void sql_pool();
	void thread_pool();	
	void trig_mode();
	void eventListen();

public:
	//基础
	int m_port;
	char* m_root;
	int m_log_write;
	int m_close_log;
	int m_actormodel;
	
	//数据库相关
	connection_pool *m_connPool;	
	string m_user;		//登录数据库用户名
	string m_password;	//登录数据库密码
	string m_databaseName;	//使用数据库名
	int m_sql_num;

	//http相关
	http_conn *users_http;

	//线程池相关
	threadpool<http_conn> *m_pool;
	int m_thread_num;

	int m_OPT_LINGER;
	int m_TRIGMode;
	int m_LISTENTrigmode;
	int m_CONNTrigmode;

	//监听
	int m_listenfd;
	
	//定时器相关
	client_data *users_timer;
	Utils utils;
	int m_pipefd[2];
	int m_epollfd;
};


#endif
