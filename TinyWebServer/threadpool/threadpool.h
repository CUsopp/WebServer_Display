#ifndef THREADPOOL_H
#define THREADPOOL_H

#include "../lock/locker.h"
#include "../CGImysql/sql_connection_pool.h"

template<typename T>
class threadpool
{
public:
	threadpool(int actor_model, connection_pool *connPool, int thread_number = 8, int max_request = 10000);
	~threadpool();
	bool append(T *request, int state);
	bool append_p(T *request);

private:
	//工作线程运行的函数，它不断从工作队列中取出任务并执行
	static void *worker(void *arg);
	void run();

private:
	int m_thread_number;		//线程池中的线程数
	int m_max_requests;		//请求队列中允许的最大请求数
	pthread_t *m_threads;		//描述线程池的数组，其大小为 m_thread_number
	std::list<T *> m_workqueue;	//请求队列
	locker m_queuelocker;		//保护请求队列的互斥锁
	sem m_queuestat;		//是否有任务需要处理
	connection_pool *m_connPool;	//数据库
	int m_actor_model;		//模型切换
};



#endif
