#include "config.h"

int main(int argc, char *argv[])
{
	//需要修改的数据库信息，登录名，密码，库名
	//CREATE USER 'tourist'@'%' IDENTIFIED BY 'tour666';
	string user = "tourist";
	string passwd = "tour666";
	string databaseName = "forya";

	//命令行解析
	Config config;
	config.parse_arg(argc, argv);

	WebServer server;	
	
	//初始化
	server.init(config.PORT, user, passwd, databaseName, config.LOGWrite, 
			config.OPT_LINGER, config.TRIGMode, config.sql_num, config.thread_num,
			config.close_log, config.actor_model);

	//日志
	server.log_write();

	//数据库
	server.sql_pool();

	//线程池
	server.thread_pool();

	return 0;
}
