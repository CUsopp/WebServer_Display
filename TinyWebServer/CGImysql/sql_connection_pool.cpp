#include "sql_connection_pool.h"

using namespace std;

connection_pool::connection_pool()
{
	m_CurConn = 0;
	m_FreeConn = 0;
}

connection_pool::~connection_pool()
{
	DestroyPool();
}

connection_pool *connection_pool::GetInstance()
{
	static connection_pool connPool;
	return &connPool;
}

//构造初始化
void connection_pool::init(string url, string User, string Password, string DataBaseName,
		 int Port, int MaxConn, int close_log)
{
	m_url = url;
	m_User = User;
	m_Password = Password;
	m_DatabaseName = DataBaseName;
	m_Port = Port;
	m_close_log = close_log;

	for(int i = 0; i < MaxConn; ++i)
	{
		MYSQL *con = NULL;
		con = mysql_init(con);
	
		if(con == NULL)
		{
			LOG_ERROR("MYSQL Error");
			exit(1);
		}
		con = mysql_real_connect(con, url.c_str(), User.c_str(), Password.c_str(),
						 DataBaseName.c_str(), Port, NULL, 0);

		if(con == NULL)
		{
			LOG_ERROR("MYSQL Error");
			exit(1);
		}
		connlist.push_back(con);
		++m_FreeConn;
	}
	reserve = sem(m_FreeConn);
	m_MaxConn = m_FreeConn;
}

//当有请求时，从数据库连接池中返回一个可用连接，更新使用和空闲连接数
MYSQL *connection_pool::GetConnection()
{
	MYSQL *con = NULL;
	
	if(0 == connlist.size())
	{
		return NULL;
	}

	reserve.wait();

	lock.lock();

	con = connlist.front();
	connlist.pop_front();

	--m_FreeConn;
	++m_CurConn;
	
	lock.unlock();
	return con;
}

//释放当前使用的连接
bool connection_pool::ReleaseConnection(MYSQL *con)
{
	if(NULL == con)
	{
		return false;
	}

	lock.lock();

	connlist.push_back(con);
	++m_FreeConn;
	--m_CurConn;

	lock.unlock();

	reserve.post();
	return true;
}

//获取当前空闲的连接数
int connection_pool::GetFreeConn()
{
	return this->m_FreeConn;
}

//销毁数据库连接池
void connection_pool::DestroyPool()
{
	lock.lock();
	if(connlist.size() > 0)
	{
		list<MYSQL *>::iterator it;
		for(it = connlist.begin(); it != connlist.end(); ++it)
		{
			MYSQL *con = *it;
			mysql_close(con);
		}
		m_CurConn = 0;
		m_FreeConn = 0;
		connlist.clear();
	}
	lock.unlock();
}

connectionRAII::connectionRAII(MYSQL **SQL, connection_pool *connPool)
{
	*SQL = connPool->GetConnection();

	conRAII = *SQL;
	poolRAII = connPool;
}

connectionRAII::~connectionRAII()
{
	poolRAII->ReleaseConnection(conRAII);
}
