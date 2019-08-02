#include "foobar.h"

pthread_mutex_t foo_mutex;
pthread_cond_t foo_cond;
pthread_cond_t bar_cond;

int flag_foo;
int flag_bar;

int main(int argc, char** argv)
{
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] foobar print testing started\n", pthread_self(), __func__, __LINE__);
#endif
	if(argc != 2)
	{
#ifdef _ENABLE_LOGS_ERR_
		printf("thread: [%ld] function: [%s] line: [%d] foobar usage: ./foobar <foo bar print frequency>\n", pthread_self(), __func__, __LINE__);
#endif
		return -1;
	}
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] arguments passed: [%s] [%s]\n", pthread_self(), __func__, __LINE__, argv[0], argv[1]);
#endif
	int freq = atoi(argv[1]);
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] print frequency: [%d]\n", pthread_self(), __func__, __LINE__, freq);
#endif
	pthread_t pthread[2];

	pthread_mutex_init(&foo_mutex, NULL);
	pthread_cond_init(&foo_cond, NULL);
	pthread_cond_init(&bar_cond, NULL);
	
	flag_foo = 1;
	flag_bar = 0;
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] threads started\n", pthread_self(), __func__, __LINE__);
#endif
	
	int pret1, pret2;
	pret1 = pthread_create(&pthread[0], NULL, (void*)print_foo, (void*)&freq);
	if(pret1)
	{
#ifdef _ENABLE_LOGS_THREAD_ERR_
		printf("thread: [%ld] function: [%s] line: [%d] thread 1 creation failed\n", pthread_self(), __func__, __LINE__);
#endif
		return -1;	
	}
	pret2 = pthread_create(&pthread[1], NULL, (void*)print_bar, (void*)&freq);
	if(pret2)
	{
#ifdef _ENABLE_LOGS_THREAD_ERR_
		printf("thread: [%ld] function: [%s] line: [%d] thread 1 creation failed\n", pthread_self(), __func__, __LINE__);
#endif
		return -1;
	}

	pthread_join(pthread[0], NULL);
	pthread_join(pthread[1], NULL);
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] foobar print testing completed\n", pthread_self(), __func__, __LINE__);
#endif
	return 0;

}

void* print_foo(void* ptr)
{
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] enter\n", pthread_self(), __func__, __LINE__);
#endif
	int* freq = (int*)ptr;
	int printcount = 0;
	
	while(1)
	{
		pthread_mutex_lock(&foo_mutex);

#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex lock\n", pthread_self(), __func__, __LINE__);
#endif
		while(flag_foo != 1 && flag_bar == 1)
			pthread_cond_wait(&foo_cond, &foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after foo cond wait\n", pthread_self(), __func__, __LINE__);
#endif
		pthread_mutex_unlock(&foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex unlock\n", pthread_self(), __func__, __LINE__);
#endif
		
		printcount++;
		printf("Line %d: foo ", printcount);
	
		pthread_mutex_lock(&foo_mutex);
		flag_bar = 1;
		flag_foo = 0;
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex lock\n", pthread_self(), __func__, __LINE__);
#endif
		pthread_cond_signal(&bar_cond);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after bar cond signal \n", pthread_self(), __func__, __LINE__);
#endif
		pthread_mutex_unlock(&foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex unlock\n", pthread_self(), __func__, __LINE__);
#endif

		if(printcount == *freq)
			break;
	}
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] exit\n", pthread_self(), __func__, __LINE__);
#endif	
	return NULL;
}

void* print_bar(void* ptr)
{
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] enter\n", pthread_self(), __func__, __LINE__);
#endif
	int* freq = (int*)ptr;
	int printcount = 0;

	while(1)
	{
		pthread_mutex_lock(&foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex lock\n", pthread_self(), __func__, __LINE__);
#endif
		while(flag_bar != 1 && flag_foo == 1)
			pthread_cond_wait(&bar_cond, &foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after bar cond wait\n", pthread_self(), __func__, __LINE__);
#endif
		pthread_mutex_unlock(&foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex unlock\n", pthread_self(), __func__, __LINE__);
#endif
		printcount++;
		printf("bar\n");

		pthread_mutex_lock(&foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex lock\n", pthread_self(), __func__, __LINE__);
#endif
		flag_bar = 0;
		flag_foo = 1;
		pthread_cond_signal(&foo_cond);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after foo cond signal \n", pthread_self(), __func__, __LINE__);
#endif
		pthread_mutex_unlock(&foo_mutex);
#ifdef _ENABLE_LOGS_THREAD_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] after mutex unlock\n", pthread_self(), __func__, __LINE__);
#endif

		if(printcount == *freq)
			break;
	}
#ifdef _ENABLE_LOGS_INFO_
	printf("thread: [%ld] function: [%s] line: [%d] exit\n", pthread_self(), __func__, __LINE__);
#endif
	return NULL;
}




