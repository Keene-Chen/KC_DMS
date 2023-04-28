/**
 * * Author     : KeeneChen
 * * DateTime   : 2022.08.18-21:40:05
 * * Description: C thread pool implementation
 * * Reference  : https://subingwen.cn/linux/threadpool
 * * Editor     : Visual Studio Code
 * * Platform   : Rocky 9.0 (Blue Onyx) x86_64
 */

#include "threadpool.h"

// 管理者线程每次添加线程个数
const int NUMBER = 2;

/**
 * @brief  创建线程池并初始化
 * @param  min_thread  最小线程数量
 * @param  max_thread  最大线程数量
 * @param  queue_size  当前任务数量
 * @return ThreadPool* 线程池地址
 */
ThreadPool* threadpool_create(int min_thread, int max_thread, int queue_size)
{
    // 定义线程池
    ThreadPool* pool = (ThreadPool*)malloc(sizeof(ThreadPool));

    // ? 为什么使用do while循环控制. 可以使用break跳出,优化初始化操作
    do {
        if (pool == NULL) {
            printf("malloc threadpool failed\n");
            break;
        }

        pool->thread_ids = (pthread_t*)malloc(sizeof(pthread_t) * max_thread);
        if (pool->thread_ids == NULL) {
            printf("malloc threadpool failed\n");
            break;
        }

        // 初始化线程ID
        memset(pool->thread_ids, 0, sizeof(pthread_t) * max_thread);
        pool->min_thread  = min_thread;
        pool->max_thread  = max_thread;
        pool->busy_thread = 0;
        pool->live_thread = min_thread;
        pool->exit_thread = 0;

        // 初始化锁和条件变量
        if (pthread_mutex_init(&pool->mutex_pool, NULL) != 0
            || pthread_mutex_init(&pool->mutex_busy, NULL) != 0
            || pthread_cond_init(&pool->is_full, NULL) != 0
            || pthread_cond_init(&pool->is_empty, NULL) != 0) {
            printf("mutex or condition init failed\n");
            return 0;
        }

        // 初始化任务队列
        pool->task_queue     = (Task*)malloc(sizeof(Task) * queue_size);
        pool->queue_capacity = queue_size;
        pool->queue_size     = 0;
        pool->queue_front    = 0;
        pool->queue_rear     = 0;

        // 初始化是否销毁线程池
        pool->shutdown = 0;

        // 创建线程
        pthread_create(&pool->manager_id, NULL, manager, pool);
        for (int i = 0; i < min_thread; ++i) {
            pthread_create(&pool->thread_ids[i], NULL, worker, pool);
        }
        return pool;
    } while (0);

    // 释放资源
    if (pool && pool->thread_ids)
        free(pool->thread_ids);
    if (pool && pool->task_queue)
        free(pool->task_queue);
    if (pool)
        free(pool);

    return NULL;
}

// 管理者线程回调函数
static void* manager(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;

    while (!pool->shutdown) {
        // 每隔3s监测工作线程
        sleep(3);

        // 取出线程池中任务的数量和当前线程的数量
        pthread_mutex_lock(&pool->mutex_pool);
        int queue_size  = pool->queue_size;
        int live_thread = pool->live_thread;
        pthread_mutex_unlock(&pool->mutex_pool);

        // 取出忙的线程的数量
        pthread_mutex_lock(&pool->mutex_busy);
        int busy_thread = pool->busy_thread;
        pthread_mutex_unlock(&pool->mutex_busy);

        /**
         * ! 添加线程
         * ? 何时添加线程,当任务个数>存活线程个数 && 存活线程数<最大线程数
         */
        if (queue_size > live_thread && live_thread < pool->max_thread) {
            pthread_mutex_lock(&pool->mutex_pool);
            int count = 0;

            // ! 未理解for循环条件
            for (int i = 0; (i < pool->max_thread) && (count < NUMBER)
                            && (pool->live_thread < pool->max_thread);
                 i++) {
                if (pool->thread_ids[i] == 0) {
                    pthread_create(&pool->thread_ids[i], NULL, worker, pool);
                    count++;
                    pool->live_thread++;
                }
            }

            pthread_mutex_unlock(&pool->mutex_pool);
        }

        /**
         * ! 销毁线程
         * ? 何时销毁线程,忙的线程*2 < 存活的线程数 && 存活的线程>最小线程数
         */
        if (busy_thread * 2 < live_thread && live_thread > pool->min_thread) {
            pthread_mutex_lock(&pool->mutex_pool);
            pool->exit_thread = NUMBER;
            pthread_mutex_unlock(&pool->mutex_pool);

            // 让工作线程自杀
            for (int i = 0; i < NUMBER; i++) {
                // 在worker函数中wait
                pthread_cond_signal(&pool->is_empty);
            }
        }
    }

    return NULL;
}

// 工作者线程回调函数
static void* worker(void* arg)
{
    ThreadPool* pool = (ThreadPool*)arg;

    while (1) {
        // 加锁
        pthread_mutex_lock(&pool->mutex_pool);

        // 当前任务队列是否为空
        while (pool->queue_size == 0 && !pool->shutdown) {
            // 为空就阻塞工作线程
            pthread_cond_wait(&pool->is_empty, &pool->mutex_pool);

            // 判断是否销毁线程,管理者线程让工作线程自杀手段
            if (pool->exit_thread > 0) {
                pool->exit_thread--;

                // 当存活线程数量大于线程池最小数量时,才杀死线程
                if (pool->live_thread > pool->min_thread) {
                    pool->live_thread--;
                    pthread_mutex_unlock(&pool->mutex_pool);

                    // 单个线程退出
                    thread_exit(pool);
                }
            }
        }

        // 判断线程池是否关闭,防止死锁
        if (pool->shutdown) {
            pthread_mutex_unlock(&pool->mutex_pool);
            thread_exit(pool);
        }

        // 从任务队列取任务
        Task task;
        task.func = pool->task_queue[pool->queue_front].func;
        task.arg  = pool->task_queue[pool->queue_front].arg;
        // ?为什么移动队头,形成循环队列(数组实现)
        pool->queue_front = (pool->queue_front + 1) % pool->queue_capacity;
        pool->queue_size--;

        // 唤醒生产者线程
        pthread_cond_signal(&pool->is_full);
        // 解锁
        pthread_mutex_unlock(&pool->mutex_pool);

        // 对正在工作的线程加锁，统计正在工作线程
        printf("[%lu]:thread start working\n", pthread_self());
        pthread_mutex_lock(&pool->mutex_busy);
        pool->busy_thread++;
        pthread_mutex_unlock(&pool->mutex_busy);
        // 执行任务
        task.func(task.arg);
        // 释放参数
        free(task.arg);
        task.arg = NULL;
        printf("[%lu]:thread end working\n", pthread_self());
        pthread_mutex_lock(&pool->mutex_busy);
        pool->busy_thread--;
        pthread_mutex_unlock(&pool->mutex_busy);
    }

    return NULL;
}

// 单个线程退出
static void thread_exit(ThreadPool* pool)
{
    pthread_t tid = pthread_self();
    for (int i = 0; i < pool->max_thread; i++) {
        if (pool->thread_ids[i] == tid) {
            pool->thread_ids[i] = 0;
            printf("thread_exit() called %ld exiting\n", tid);
            break;
        }
    }
    pthread_exit(NULL);
}

/**
 * @brief  销毁线程池
 * @param  pool 线程池
 * @return int  成功销毁为0,失败为-1
 */
int threadpool_destroy(ThreadPool* pool)
{
    if (pool == NULL) {
        return -1;
    }

    // 关闭线程池
    pool->shutdown = 1;

    // 阻塞回收管理者线程
    pthread_join(pool->manager_id, NULL);

    // 唤醒阻塞的消费者线程
    for (int i = 0; i < pool->live_thread; i++) {
        pthread_cond_signal(&pool->is_empty);
    }

    // 释放堆内存
    if (pool->task_queue) {
        free(pool->task_queue);
        pool->task_queue = NULL;
    }
    if (pool->thread_ids) {
        free(pool->thread_ids);
        pool->thread_ids = NULL;
    }
    pthread_mutex_destroy(&pool->mutex_pool);
    pthread_mutex_destroy(&pool->mutex_busy);
    pthread_cond_destroy(&pool->is_empty);
    pthread_cond_destroy(&pool->is_full);
    free(pool);
    pool = NULL;

    return 0;
}

/**
 * @brief  线程池添加任务
 * @param  pool 线程池
 * @param  func 任务回调函数
 * @param  arg  任务参数
 * @return void
 */
int threadpool_add(ThreadPool* pool, void* (*func)(void*), void* arg)
{
    pthread_mutex_lock(&pool->mutex_pool);
    while (pool->queue_size == pool->queue_capacity && !pool->shutdown) {
        // 阻塞生产者线程
        pthread_cond_wait(&pool->is_full, &pool->mutex_pool);
    }
    if (pool->shutdown) {
        pthread_mutex_unlock(&pool->mutex_pool);
        return 0;
    }

    // 添加任务
    pool->task_queue[pool->queue_rear].func = func;
    pool->task_queue[pool->queue_rear].arg  = arg;
    pool->queue_rear                        = (pool->queue_rear + 1) % pool->queue_capacity;
    pool->queue_size++;

    pthread_cond_signal(&pool->is_empty);

    pthread_mutex_unlock(&pool->mutex_pool);
    return 1;
}

/**
 * @brief  获取线程池中工作线程个数
 * @param  pool 线程池
 * @return int  线程池中工作线程个数
 */
int threadpool_get_busy(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutex_busy);
    int busy_thread = pool->busy_thread;
    pthread_mutex_unlock(&pool->mutex_busy);

    return busy_thread;
}

/**
 * @brief  获取线程池中存活线程个数
 * @param  pool 线程池
 * @return int  线程池中存活线程个数
 */
int threadpool_get_live(ThreadPool* pool)
{
    pthread_mutex_lock(&pool->mutex_pool);
    int live_thread = pool->live_thread;
    pthread_mutex_unlock(&pool->mutex_pool);

    return live_thread;
}