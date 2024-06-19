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

