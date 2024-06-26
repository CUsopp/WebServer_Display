#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "../CGImysql/sql_connection_pool.h"
#include <map>
#include <sys/socket.h>

class http_conn
{
public:
    static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;
    enum METHOD
    {
        GET = 0,
        POST,
        HEAD,
        PUT,
        DELETE,
        TRACE,
        OPTIONS,
        CONNECT,
        PATH
    };
    enum CHECK_STATE
    {
        CHECK_STATE_REQUESTLINE = 0,
        CHECK_STATE_HEADER,
        CHECK_STATE_CONTENT
    };
    enum HTTP_CODE
    {
        NO_REQUEST,
        GET_REQUEST,
        BAD_REQUEST,
        NO_RESOURCE,
        FORBIDDEN_REQUEST,
        FILE_REQUEST,
        INTERNAL_ERROR,
        CLOSED_CONNECTION
    };
    enum LINE_STATUS
    {
        LINE_OK = 0,
        LINE_BAD,
        LINE_OPEN
    };

public:
	http_conn(){}
	~http_conn(){}

public:
	//sqlconnpool
	void initmysql_result(connection_pool *connPool);

	//threadpool
	//void init(int sockfd, const sockaddr_in &addr, char *, int, int, string user, string passwd, string sqlname);
	bool read_once();
	bool write();
	void process();
	int timer_flag;
	int improv;

private:
	void init();

private:
	map<string, string> m_users;
	int m_close_log;
	int m_TRIGMode;//1-ET-边缘触发 0-LT-水平触发

private:
	int m_sockfd;
	char m_read_buf[READ_BUFFER_SIZE];
	long m_read_idx;
	
};


#endif
