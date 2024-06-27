#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

#include "../CGImysql/sql_connection_pool.h"
#include <map>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/uio.h>

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

	HTTP_CODE process_read();
	LINE_STATUS parse_line();
	char *get_line() { return m_read_buf + m_start_line; };
	HTTP_CODE parse_request_line(char *text);
	HTTP_CODE parse_headers(char *text);
	HTTP_CODE parse_content(char *text);
	HTTP_CODE do_request();
	bool add_response(const char *format, ...);
	bool add_status_line(int status, const char *title);
	bool add_headers(int content_length);
	bool add_content_length(int content_length);
	bool add_linger();
	bool add_blank_line();
	bool add_content(const char *content);
	bool process_write(HTTP_CODE ret);
	void close_conn(bool real_close = true);

private:
	void init();
	void unmap();	//取消内存映射

public:
	static int m_epollfd;
	static int m_user_count;
	MYSQL *mysql;
	int m_state;//读为0，写为1

private:
	map<string, string> m_users;
	int m_close_log;
	int m_TRIGMode;//1-ET-边缘触发 0-LT-水平触发

private:
	int m_sockfd;
	char m_read_buf[READ_BUFFER_SIZE];
	long m_read_idx;
	long m_checked_idx;
	int m_start_line;
	char m_write_buf[WRITE_BUFFER_SIZE];
	int m_write_idx;
	int bytes_to_send;
	int bytes_have_send;
	CHECK_STATE m_check_state;
	METHOD m_method;
	char m_real_file[FILENAME_LEN];
	char *m_url;
	char *m_version;
	char *m_host;
	long m_content_length;
	bool m_linger;
	int cgi;	//是否启用的POST
	char *m_string;	//存储请求头数据
	char *m_file_address;
	struct stat m_file_stat;
	struct iovec m_iv[2];
	int m_iv_count;
	char *doc_root;
	locker m_lock;
	map<string, string> users;
};


#endif
