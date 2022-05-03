#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdarg.h>
#include <string.h>

typedef	struct __myarg_t
{
	int	a;
	int	b;
}	t_myarg;

typedef	struct __myret_t
{
	int	x;
	int	y;
}	t_myret;

void	*thread_routine(void *arg)
{
	char	*msg;

	msg = (char *)arg;
	printf("%s routine executed\n", msg);
	//pthread_exit(NULL);
	return (NULL);
}

void	*my_routine(void *arg)
{
	t_myarg	*my_arg;
	t_myret *ret;

	my_arg = (t_myarg *)arg;
	printf("Passed %d and %d\n", my_arg->a, my_arg->b);
	ret = malloc(sizeof(t_myret));
	ret->x = my_arg->a + my_arg->b;
	ret->y = my_arg->a - my_arg->b;
	return ((void *)ret);
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
	pthread_t	thread1, thread2;
	char		*msg1 = "Thread 1";
	char		*msg2 = "Thread 2";
	int			ret1, ret2;
	t_myarg		arg;
	t_myret		*ret;

	arg.a = 100;
	arg.b = 20;
	ret1 = pthread_create(&thread1, NULL, thread_routine, (void *)msg1);
	ret2 = pthread_create(&thread1, NULL, thread_routine, (void *)msg2);

	//ret1 = pthread_join(thread1, NULL);
	//if (ret1 != 0)
	//	err_quit("join thread1 failed :%s\n", strerror(ret1));
	//ret2 = pthread_join(thread2, NULL);
	//if (ret2 != 0)
	//	err_quit("join thread2 failed :%s\n", strerror(ret2));
	//sleep(1);
	printf("Thread 1 returns: %d\n", ret1);
	printf("Thread 2 returns: %d\n", ret2);

	ret1 = pthread_create(&thread1, NULL, my_routine, &arg);
	ret1 = pthread_join(thread1, (void **)&ret);
	//sleep(1);
	printf("Reterned a + b = %d, a - b = %d\n", ret->x, ret->y);
	return (0);
}
