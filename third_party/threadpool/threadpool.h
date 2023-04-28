/**
 * * Author     : KeeneChen
 * * DateTime   : 2022.08.18-21:39:29
 * * Description: C thread pool declaration
 * * Reference  : https://subingwen.cn/linux/threadpool
 * * Editor     : Visual Studio Code
 * * Platform   : Rocky 9.0 (Blue Onyx) x86_64
 */

#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__

/* 导入头文件,仅适用于Linux */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* 声明任务结构体 */
typedef struct task_t {
    void* (*func)(void* arg);
    void* arg;
} Task;

/* 声明线程池结构体 */
typedef struct threadpool_t {
    Task* task_queue;   // 任务队列
    int queue_capacity; // 任务容量
    int queue_size;     // 当前任务数量
    int queue_front;    // 队列头,用于取数据
    int queue_rear;     // 队列尾,用于放数据

    pthread_t manager_id;  // 管理者线程ID
    pthread_t* thread_ids; // 工作的线程ID
    int min_thread;        // 最小线程数量
    int max_thread;        // 最大线程数量
    int busy_thread;       // 正在工作的线程数量
    int live_thread;       // 当前存活的线程数量
    int exit_thread;       // 要销毁的线程数量
    int shutdown;          // 是否销毁线程池,销毁为1,不销毁为0

    pthread_mutex_t mutex_pool; // 锁整个线程池
    pthread_mutex_t mutex_busy; // 锁busy_thread变量
    pthread_cond_t is_full;     // 条件变量,任务队列是否为满
    pthread_cond_t is_empty;    // 条件变量,任务队列是否为空
} ThreadPool;

/* 线程池API */
/**
 * @brief  创建线程池并初始化
 * @param  min_thread  最小线程数量
 * @param  max_thread  最大线程数量
 * @param  queue_size  当前任务数量
 * @return ThreadPool* 线程池地址
 */
ThreadPool* threadpool_create(int min_thread, int max_thread, int queue_size);

/**
 * @brief  销毁线程池
 * @param  pool 线程池
 * @return int  成功销毁为0,失败为-1
 */
int threadpool_destroy(ThreadPool* pool);

/**
 * @brief  线程池添加任务
 * @param  pool 线程池
 * @param  func 任务回调函数
 * @param  arg  任务参数
 * @return void
 */
int threadpool_add(ThreadPool* pool, void* (*func)(void*), void* arg);

/**
 * @brief  获取线程池中工作线程个数
 * @param  pool 线程池
 * @return int  线程池中工作线程个数
 */
int threadpool_get_busy(ThreadPool* pool);

/**
 * @brief  获取线程池中存活线程个数
 * @param  pool 线程池
 * @return int  线程池中存活线程个数
 */
int threadpool_get_live(ThreadPool* pool);

/* 辅助函数 */
// 工作者线程(消费者线程)任务函数
static void* worker(void* arg);
// 管理者线程任务函数
static void* manager(void* arg);
// 单个线程退出
static void thread_exit(ThreadPool* pool);

#endif // __THREADPOOL_H__