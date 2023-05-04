/**
 * * Author     : KeeneChen
 * * DateTime   : 2022.08.18-21:42:06
 * * Description: test_threadpool
 * * Editor     : Visual Studio Code
 * * Platform   : Rocky 9.0 (Blue Onyx) x86_64
 * * Command    : gcc test_threadpool.c threadpool.c -lpthread -o test_threadpool
 */

#include "threadpool.h"

const int count = 100;

void* task(void* arg)
{
    int num = *(int*)arg;
    printf("[%ld]:thread is working, number = %d\n", pthread_self(), num);
    sleep(1);
}

int main()
{
    // 创建线程池
    ThreadPool* pool = threadpool_create(2, 5, count);
    for (int i = 0; i < count; ++i) {
        int* num = (int*)malloc(sizeof(int));
        *num     = i;
        threadpool_add(pool, task, num);
    }

    // 让主线程睡眠30s,防止子线程未执行完就关闭线程池
    sleep(30);

    threadpool_destroy(pool);
    return 0;
}