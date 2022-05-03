#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>
#define	MAX_THREAD_COUNT 10

typedef	struct __myarg_t
{
	int	a;
	int	b;
	pthread_mutex_t	mutex1;
}	t_myarg;

typedef	struct __myret_t
{
	int	x;
	int	y;
}	t_myret;

void	*without_mutex(void *arg)
{
	t_myarg	*m;

	m = arg;
	++m->b;
	printf("I'm %dth thread!\n", m->b);
	for (int i = 0; i < 10000; i++)
		++m->a;
	return (NULL);
}
void	*with_mutex(void *arg)
{
	t_myarg	*m;

	m = arg;
	pthread_mutex_lock(&m->mutex1);
	++m->b;
	printf("I'm %dth thread!\n", m->b);
	for (int i = 0; i < 10000; i++)
		++m->a;
	pthread_mutex_unlock(&m->mutex1);
	return (NULL);
}

void	err_quit(char *fmt, ...)
{
	va_list	arg;
	
	va_start(arg, fmt);
	vprintf(fmt, arg);
	va_end(arg);
}

int	main()
{
	pthread_t	thread[MAX_THREAD_COUNT];
	int			err;
	t_myarg		arg;
	t_myret		*ret;
	int			count = 0;

	printf("[Without mutex]\n");
	arg.a = 0;
	arg.b = 0;
	for (int i = 0; i < MAX_THREAD_COUNT; i++)
	{	
		err = pthread_create(&thread[i], NULL, without_mutex, &arg);
		if (err != 0)
			err_quit("thread %d create fail\n", strerror(err));
	}
	for (int i = 0; i < MAX_THREAD_COUNT; i++)
	{	
		err = pthread_join(thread[i], NULL);
		if (err != 0)
			err_quit("thread %d join fail\n", strerror(err));
	}
		//pthread_join(thread[i], NULL);
	printf("Expected count:100000\n");
	printf("Actual   count:%d\n\n", arg.a);

	printf("[With mutex]\n");
	arg.a = 0;
	arg.b = 0;
	pthread_mutex_init(&arg.mutex1, NULL);
	for (int i = 0; i < MAX_THREAD_COUNT; i++)
	{	
		err = pthread_create(&thread[i], NULL, with_mutex, &arg);
		if (err != 0)
			err_quit("thread %d create fail\n", strerror(err));
	}
	for (int i = 0; i < MAX_THREAD_COUNT; i++)
	{	
		err = pthread_join(thread[i], NULL);
		if (err != 0)
			err_quit("thread %d join fail\n", strerror(err));
	}
	//for (int i = 0; i < MAX_THREAD_COUNT; i++)
	//	pthread_create(&thread[i], NULL, with_mutex, &arg);
	//for (int i = 0; i < MAX_THREAD_COUNT; i++)
	//	pthread_join(thread[i], NULL);
	printf("Expected count:100000\n");
	printf("Actual   count:%d\n\n", arg.a);
	
	printf("[Without mutex but join immediately]\n");
	arg.a = 0;
	arg.b = 0;
	for (int i = 0; i < MAX_THREAD_COUNT; i++)
	{	
		err = pthread_create(&thread[i], NULL, without_mutex, &arg);
		if (err != 0)
			err_quit("thread %d create fail\n", strerror(err));
		err = pthread_join(thread[i], NULL);
		if (err != 0)
			err_quit("thread %d join fail\n", strerror(err));
	}
	printf("Expected count:100000\n");
	printf("Actual   count:%d\n\n", arg.a);
	//ret1 = pthread_create(&thread1, NULL, my_routine, &arg);
	//ret1 = pthread_join(thread1, (void **)&ret);
	//sleep(1);
	//printf("Reterned a + b = %d, a - b = %d\n", ret->x, ret->y);
	return (0);
}
