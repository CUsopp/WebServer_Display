#include "config.h"

int main(int argc, char *argv[])
{
	//需要修改的数据库信息，登录名，密码，库名
	string user = "root";
	string passwd = "root";
	string databaseName = "qgydb";

	//命令行解析
	Config config;
	config.parse_arg(argc, argv);

	WebServer server;	

	return 0;
}