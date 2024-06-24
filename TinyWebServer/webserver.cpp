#include "webserver.h"

WebServer::WebServer()
{

}

WebServer::~WebServer()
{

}


void WebServer::init(int port, string user, string password, string databaseName,
		int log_write, int opt_linger, int trigmode, int sql_num,
		int thread_num, int close_log, int actor_model)
{
	m_port = port;
	m_user = user;
	m_password = password;
	m_databaseName = databaseName;
	m_log_write = log_write;
	m_OPT_LINGER = opt_linger;
	m_TRIGMode = trigmode;
	m_sql_num = sql_num;
	m_thread_num = thread_num;
	m_close_log = close_log;
	m_actormodel = actor_model;
}

void WebServer::log_write()
{
	if(0 == m_close_log)
	{
		//初始化日志
		if(1 == m_log_write)
		{
			Log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 800);	
		}
		else
		{
			Log::get_instance()->init("./ServerLog", m_close_log, 2000, 800000, 0);
		}
	}
}

void WebServer::sql_pool()
{
	//初始化数据库连接池
	m_connPool = connection_pool::GetInstance();
	m_connPool->init("localhost", m_user, m_password, m_databaseName, 3306, m_sql_num, m_close_log);

	//初始化数据库读取表
	
}
