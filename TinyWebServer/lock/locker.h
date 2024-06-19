#ifndef LOCKER_H
#define LOCKER_H

#include <exception>
#include <pthread.h>
#include <semaphore.h>

//互斥锁
class locker
{
public:
	locker()
	{
		if(pthread_mutex_init(&m_mutex, NULL) != 0)
		{
			throw std::exception();
		}
	}
	~locker()
	{
		pthread_mutex_destroy(&m_mutex);
	}
	bool lock()
	{
		return pthread_mutex_lock(&m_mutex) == 0;
	}
	bool unlock()
	{
		return pthread_mutex_unlock(&m_mutex) == 0;
	}
	pthread_mutex_t *get()
	{
		return &m_mutex;
	}

private:
	pthread_mutex_t m_mutex;
};

//条件变量 pthread_cond_t
class cond
{
public:
	cond()
	{
		if(pthread_cond_init(&m_cond, NULL) != 0)
		{
			throw std::exception();
		}
	}
	~cond()
	{
		pthread_cond_destroy(&m_cond);
	}
	bool wait(pthread_mutex_t *m_mutex)
	{
		int ret = 0;
		//先解已有的互斥锁，然后阻塞，等条件变量满足收到唤醒时，解除阻塞并重新上锁
		ret = pthread_cond_wait(&m_cond, m_mutex);
		return ret == 0;
	}
	bool signal()
	{
		return pthread_cond_signal(&m_cond) == 0;
	}
	bool broadcast()
	{
		return pthread_cond_broadcast(&m_cond) == 0;
	}
	
private:
	pthread_cond_t m_cond;

};

//信号量 sem_t
class sem
{
public:
	sem()
	{
		if(sem_init(&m_sem, 0, 0) != 0)
		{
			throw std::exception();
		}
	}
	sem(int num)//信号初始值-资源数
	{
		if(sem_init(&m_sem, 0, num) !+ 0)
		{
			throw std::exception();
		}
	}
	~sem()
	{
		sem_destroy(&m_sem);
	}
	bool wait()
	{
		return sem_wait(&m_sem) == 0;
	}
	bool post()
	{
		return sem_post(&m_sem) == 0;
	}

private:
	sem_t m_sem;

};

#endif
