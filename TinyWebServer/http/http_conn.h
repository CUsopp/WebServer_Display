#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "../CGImysql/sql_connection_pool.h"
#include <map>

class http_conn
{
public:
	http_conn(){}
	~http_conn(){}

public:
	void initmysql_result(connection_pool *connPool);

private:
	map<string, string> m_users;
	int m_close_log;
};


#endif
