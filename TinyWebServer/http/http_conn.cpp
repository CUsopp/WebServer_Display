#include "http_conn.h"

void http_conn::initmysql_result(connection_pool *connPool)
{
	//先从连接池中取一个连接
	MYSQL *mysql = NULL;
	connectionRAII mysqlcon(&mysql, connPool);

	//在user表中检索username，password数据，浏览器输入
	if(mysql_query(mysql, "SELECT username,password FROM user"))
	{
		LOG_ERROR("SELECT error:%s\n", mysql_error(mysql));
	}

	//从表中检索完整的结果集
	MYSQL_RES *result = mysql_store_result(mysql);

	//返回结果集中的列数
	int num_fields = mysql_num_fields(result);

	//返回所有字段结构的数组
	MYSQL_FIELD *fields = mysql_fetch_fields(result);

	//从结果集中获取下一行。将对应的用户名和密码，存入map中
	while(MYSQL_ROW row = mysql_fetch_row(result))
	{
		string temp1(row[0]);
		string temp2(row[1]);
		m_users[temp1] = temp2;
	}
	return;
}
