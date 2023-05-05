// #include "thread_threadpoll.hh"
#include "server.hh"
// #include <pthread.h>
void thread_begin(std::queue<item_of_works> *work_queue,
                  std::mutex *mtx,
                  std::condition_variable *cond_var,
                  bool *is_stop)
{
    // 它从工作队列中取出顶部条目，
    // 然后对其执行一些操作。它通过获取互斥锁并在条件变量上有条件地等待来实现这一点，直到队列中至少有一个项。
    // 使用unique_lock确保在锁离开范围时自动解锁互斥锁。
    // MYSQL *mysql = mysql_init(NULL);
    //
    // if ((mysql = mysql_real_connect(mysql, "localhost", "root", "wangyibodemima",
    // "test", 0, NULL, 0)) == NULL)
    // perror("mysql connect failed");

    // 子线程屏蔽信号
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGINT);
    pthread_sigmask(SIG_BLOCK, &set, NULL);

    MySql mysql;
    while (true)
    {
        // 获取一个互斥锁
        {
            std::unique_lock<std::mutex> lock(*mtx);
            GLOG_INFO("      GET LOCKED in POOL", 0);

            // 在条件变量上等待，直到工作队列中至少有一个项。
            // lambda函数检查等待的条件。
            (*cond_var).wait(lock, [&]() -> bool // true 不阻塞
                             { GLOG_INFO("THREAD Try Get_work OR Search IS_STOP" ,0);
                        //   std::cout << ((!(*work_queue).empty())) << std::endl; 
                        //   std::cout << ( is_stop) << std::endl; 
                            // std::cout << &is_stop << std::endl;
                            return ((!(*work_queue).empty()) || (*is_stop)); });               // 队列不为空或者停止标识为真

            // 唤醒后 如果工作队列中没有更多的项，is_stop 为 true
            if ((*work_queue).empty())
            {
                break;
            }
            else // 从工作队列中拿任务
            {
                // 从工作队列中获取顶部项并将其从队列中删除
                item_of_works work = (*work_queue).front();
                GLOG_INFO("      THREAD Get Work  " << work.action, 0);
                (*work_queue).pop();

                // 对工作项执行操作
                work.fun(work.message, work.point, &mysql , work.timestamp);

                // 释放消息对象的内存
                delete work.message;
                free(work.timestamp);
                GLOG_INFO("      THREAD Work Done  " << work.action, 0);
                // work.message = nullptr;
            }
        }
        GLOG_INFO("     RELASE LOCKED in POOL" << std::endl, 0);
    }
    GLOG_INFO("****************        thread join      ********************", 0);
    mysql_thread_end(); // 清理MySQL线程
}