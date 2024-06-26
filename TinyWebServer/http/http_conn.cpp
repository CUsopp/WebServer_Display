#include "http_conn.h"

//初始化新接受的连接
//check_state默认为分析请求行状态
void http_conn::init()
{
    //mysql = NULL;
    //bytes_to_send = 0;
    //bytes_have_send = 0;
    //m_check_state = CHECK_STATE_REQUESTLINE;
    //m_linger = false;
    //m_method = GET;
    //m_url = 0;
    //m_version = 0;
    //m_content_length = 0;
    //m_host = 0;
    //m_start_line = 0;
    //m_checked_idx = 0;
    m_read_idx = 0;
    //m_write_idx = 0;
    //cgi = 0;
    //m_state = 0;
    //timer_flag = 0;
    improv = 0;

    memset(m_read_buf, '\0', READ_BUFFER_SIZE);
    //memset(m_write_buf, '\0', WRITE_BUFFER_SIZE);
    //memset(m_real_file, '\0', FILENAME_LEN);
}

//sqlpool
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

//threadpooli
//循环读取客户数据，直到无数据可读或对方关闭连接
//边缘触发和水平触发两种方式读数据
bool http_conn::read_once()
{
	if(m_read_idx >= READ_BUFFER_SIZE)
	{
		return false;
	}
	int bytes_read = 0;
	
	//LT读取数据 水平触发
	if(0 == m_TRIGMode)
	{
		bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
		m_read_idx += bytes_read;

		if(bytes_read <= 0)
		{
			return false;
		}
		return true;
	}
	else//边缘触发
	{
		while(true)
		{
			bytes_read = recv(m_sockfd, m_read_buf + m_read_idx, READ_BUFFER_SIZE - m_read_idx, 0);
			if(bytes_read == -1)
			{
				if(errno == EAGAIN || errno == EWOULDBLOCK)
					break;
				return false;
			}
			else if(bytes_read == 0)
			{
				return false;
			}
			m_read_idx += bytes_read;
		}
		return true;
	}
}


