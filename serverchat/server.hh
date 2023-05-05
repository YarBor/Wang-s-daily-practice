#ifndef SERVER_HEAD
#define SERVER_HEAD

#include <string>
#include <queue>

#include <glog/logging.h>

#include <vector>
#include <map>
#include <set>
#include <sstream>
#include <algorithm>
#include <cstdio>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <sys/stat.h>
#include <iostream>
#include <fcntl.h>
#include <ctime>
#include <list>
#include <nlohmann/json.hpp>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mutex>
#include <shared_mutex>
#include <condition_variable>
#include <thread>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/sendfile.h>
#include <mysql/mysql.h>
#include <sys/time.h>
#include <signal.h>
#include "./option.hh"
// #include "thread_threadpool.hh"

#define MAX_THREAD_NUM 1
#define EPOLL_EVENTS_NUM 5
#define BUFFER_SIZE 1024

#define GLOG_INFO(MESSAGE, X)                                                                           \
    if (0)                                                                                              \
    {                                                                                                   \
        LOG(INFO) << " \t"                                                                              \
                  << MESSAGE << "\n \t"                                                                 \
                  << __PRETTY_FUNCTION__ << "  LINE:>" << __LINE__ << " IN FILE:>" << __FILE__ << '\n'; \
    }                                                                                                   \
    else                                                                                                \
    {                                                                                                   \
        LOG(INFO) << " \t"                                                                              \
                  << MESSAGE << "\n \t";                                                                \
    }

#define GLOG_WARNING(MESSAGE, X)                                                                           \
    if (0)                                                                                                 \
    {                                                                                                      \
        LOG(WARNING) << " \t"                                                                              \
                     << MESSAGE << "\n \t"                                                                 \
                     << __PRETTY_FUNCTION__ << "  LINE:>" << __LINE__ << " IN FILE:>" << __FILE__ << '\n'; \
    }                                                                                                      \
    else                                                                                                   \
    {                                                                                                      \
        LOG(WARNING) << " \t"                                                                              \
                     << MESSAGE << "\n \t";                                                                \
    }

#define GLOG_ERROR(MESSAGE, X)                                                                           \
    if (0)                                                                                               \
    {                                                                                                    \
        LOG(ERROR) << " \t"                                                                              \
                   << MESSAGE << "\n \t"                                                                 \
                   << __PRETTY_FUNCTION__ << "  LINE:>" << __LINE__ << " IN FILE:>" << __FILE__ << '\n'; \
    }                                                                                                    \
    else                                                                                                 \
    {                                                                                                    \
        LOG(ERROR) << " \t"                                                                              \
                   << MESSAGE << "\n \t";                                                                \
    }

#define GLOG_FATAL(MESSAGE, X)                                                                           \
    if (0)                                                                                               \
    {                                                                                                    \
        LOG(FATAL) << " \t"                                                                              \
                   << MESSAGE << "\n \t"                                                                 \
                   << __PRETTY_FUNCTION__ << "  LINE:>" << __LINE__ << " IN FILE:>" << __FILE__ << '\n'; \
    }                                                                                                    \
    else                                                                                                 \
    {                                                                                                    \
        LOG(FATAL) << " \t"                                                                              \
                   << MESSAGE << "\n \t";                                                                \
    }

class socket_connect_to_client; // 声明
struct item_of_works;           // 在创建时就将 fun 定义好    // 声明
class server_listen_socket;     // 声明
class thread_pool;              // 声明
class My_shared_mutex;          // 声明
class userlist;                 // 声明
class MySql;                    // 声明
struct user;

void thread_begin(std::queue<item_of_works> *work_queue,
                  std::mutex *mtx,
                  std::condition_variable *cond_var,
                  bool *is_stop);
class socket_connect_to_client
//      {
//          friend class server_listen_socket;
//
//      public:
//          int _fd() const;
//          socket_connect_to_client();
//          socket_connect_to_client(int fd);
//          socket_connect_to_client(int fd, struct sockaddr_in *addr);
//          void close_fd();
//          size_t recv_message(std::string &message, size_t &action);
//          bool send_message(const std::string &message, const size_t &action);
//          struct sockaddr_in *get_address();
//
//      private:
//          int to_client_socket_fd = -1;
//          struct sockaddr_in client_address;
//      };
//
// 这是一个 `socket_connect_to_client` 类的定义，它具有以下成员函数：
//
//- `_fd()`：获取与客户端连接的套接字文件描述符。
//- `socket_connect_to_client()`：构造函数，创建一个未初始化的 `socket_connect_to_client` 对象。
//- `socket_connect_to_client(int fd)`：构造函数，使用给定的文件描述符和客户端地址结构体创建 `socket_connect_to_client` 对象。
//- `socket_connect_to_client(int fd, struct sockaddr_in *addr)`：构造函数，使用给定的文件描述符和客户端地址结构体创建 `socket_connect_to_client` 对象。
//- `close_fd()`：关闭与客户端的连接套接字。
//- `recv_message(std::string &message, size_t &action)`：从客户端接收消息并返回其大小和操作类型。
//- `send_message(const std::string &message, const size_t &action)`：向客户端发送消息并返回是否成功。
//- `get_address()`：获取客户端地址结构体。

// 此外，该类还有一个私有成员变量 `to_client_socket_fd`，表示与客户端的连接套接字文件描述符，以及一个私有成员变量 `client_address`，表示客户端地址结构体。

// 需要注意的是，该类声明了一个 friend class `server_listen_socket`，说明只有 `server_listen_socket` 可以访问该类的私有成员。
//
{
    friend class server_listen_socket; // 通过server_listen_socket设置 socket_connect_to_client的fd

public:
    int _fd() const { return to_client_socket_fd; }

    socket_connect_to_client(socket_connect_to_client &&other)
    {
        this->to_client_socket_fd = other.to_client_socket_fd;
        memcpy(&this->client_address, &other.client_address, sizeof(struct sockaddr_in));
        other.to_client_socket_fd = -1; // 将原先的 socket 变成无效（-1）
    }
    socket_connect_to_client() = default;
    void skt_fd_del_clo()
    {
        close(to_client_socket_fd);
        delete this;
    }
    socket_connect_to_client(int fd)
    {
        // Convert file descriptor to socket
        to_client_socket_fd = fd;
        struct sockaddr_storage peer_address;
        socklen_t peer_len = sizeof(peer_address);
        if (getpeername(to_client_socket_fd, (struct sockaddr *)&peer_address, &peer_len) != 0)
        {
            perror("Error in getpeername");
            exit(EXIT_FAILURE);
        }

        if (peer_address.ss_family == AF_INET)
        {
            struct sockaddr_in *peer_address_v4 = (struct sockaddr_in *)&peer_address;
            memcpy(&client_address, peer_address_v4, sizeof(client_address));
        }
    };
    socket_connect_to_client(int fd, struct sockaddr_in *addr) : to_client_socket_fd(fd) { memcpy(&client_address, addr, sizeof(struct sockaddr_in)); }
    void close_fd()
    {
        // std::cout << "socket_connect_to_client closed " << std::endl;
        close(to_client_socket_fd);
        to_client_socket_fd = -1;
    };

    bool recv_message(std::string &message, size_t &action, char *buffer_time_stamp) // size buffer_time_stamp = 28
    {
        // 保证 读写操作线程安全
        std::unique_lock<std::mutex> lock(client_read_mutex);

        message.clear();
        size_t size = 0;
        ssize_t size_received;
        if ((size_received = recv(to_client_socket_fd, &size, sizeof(size_t), MSG_WAITALL)) <= 0)
        {
            if (size_received == 0)
            {
                puts("Connection closed by client");
                return false;
            }
            else
            {
                perror("Error in recv");
                return false;
            }
        }
        if ((size_received = recv(to_client_socket_fd, buffer_time_stamp, strlen("0000-00-00 00:00:00.000000"), MSG_WAITALL)) <= 0)
        {
            if (size_received == 0)
            {
                puts("Connection closed by client");
                return false;
            }
            else
            {
                perror("Error in recv");
                return false;
            }
        }
        buffer_time_stamp[strlen("0000-00-00 00:00:00.000000")] = '\0';
        if ((size_received = recv(to_client_socket_fd, &action, sizeof(size_t), MSG_WAITALL)) <= 0)
        {
            if (size_received == 0)
            {
                puts("Connection closed by client");
                return false;
            }
            else
            {
                perror("Error in recv");
                return false;
            }
        }

        char *buff = new char[size + 1];
        if (recv(to_client_socket_fd, buff, size, MSG_WAITALL) <= 0)
        {
            puts("recv message fail");
            delete[] buff;
            return false;
        }
        buff[size] = 0;
        message.append(buff);
        delete[] buff;
        LOG(INFO) << message.c_str();
        return true;
    }

    // 加了一个char[27]的块
    bool send_message(const std::string &message, const size_t &action, char *time_stamp)
    {
        // 保证 读写操作线程安全
        std::unique_lock<std::mutex> lock(client_write_mutex);

        size_t size = message.size();
        if (!(send(to_client_socket_fd, &size, sizeof(size_t), MSG_MORE) > 0 &&
              send(to_client_socket_fd, time_stamp, strlen("0000-00-00 00:00:00.000000"), MSG_MORE) > 0 &&
              send(to_client_socket_fd, &action, sizeof(size_t), MSG_MORE) > 0 &&
              send(to_client_socket_fd, message.c_str(), message.size(), MSG_NOSIGNAL) > 0))
        {
            GLOG_INFO("send message fail _ MESSAGE:> " << message, 0);
            return false;
        }
        return true;
    }
    struct sockaddr_in *get_address() { return &client_address; }
    struct user *owner = nullptr;

    int to_client_socket_fd = -1;

private:
    struct sockaddr_in client_address;
    std::mutex client_read_mutex;  // 防止多个线程对同一socket进行 读
    std::mutex client_write_mutex; // 防止多个线程对同一socket进行 写
};

class server_listen_socket
//      {
//      public:
//          server_listen_socket(std::string IP, std::string port);
//          server_listen_socket() = default;
//          ~server_listen_socket();
//
//          bool INIT(const char *IP, const char *port);
//
//          const int _fd() const;
//          const struct sockaddr_in *server_address_() const;
//
//          int get_client(class socket_connect_to_client &a);
//          int get_client();
//      private:
//          bool init(const std::string &ip_address, const std::string &port);
//
//          int server_listen_socket_fd = -1;
//          struct sockaddr_in server_address;
//      };

// 这是一个 `server_listen_socket` 类的定义，它具有以下成员函数：

//- `server_listen_socket(std::string IP, std::string port)`：构造函数，使用给定的 IP 和端口号初始化 `server_listen_socket` 对象。
//- `server_listen_socket() = default`：默认构造函数，创建一个未初始化的 `server_listen_socket` 对象。
//- `~server_listen_socket()`：析构函数，释放与服务器套接字相关的资源。
//- `INIT(const char *IP, const char *port)`：初始化服务器套接字对象，并绑定到指定的 IP 地址和端口上。
//- `_fd() const`：获取服务器套接字文件描述符。
//- `server_address_() const`：获取服务器地址结构体。
//- `get_client(class socket_connect_to_client &a)`：接受客户端连接请求，并返回一个 `socket_connect_to_client` 对象。
//- `get_client()`：接受客户端连接请求，并返回与其连接的套接字文件描述符。

// 此外，该类还有一个私有成员函数 `init(const std::string &ip_address, const std::string &port)`，用于初始化服务器套接字对象。该类声明了两个私有成员变量 `server_listen_socket_fd`，表示服务器监听套接字文件描述符，以及一个私有成员变量 `server_address`，表示服务器地址结构体。

// 需要注意的是，该类的构造函数、析构函数和部分成员函数都是处理与服务器套接字相关的操作，用于管理服务器的连接和客户端的请求。

{
public:
    server_listen_socket(std::string IP, std::string port)
    {
        if (false == init(IP, port))
            puts("Server listen socket created false");
    };
    // 自己的IP && 开放的端口
    server_listen_socket() = default;

    ~server_listen_socket() { close(server_listen_socket_fd); };

    bool INIT(const char *IP, const char *port)
    {
        if (this->server_listen_socket_fd != -1)
        {
            close(this->server_listen_socket_fd);
            memset(&this->server_address, 0, sizeof(sockaddr_in));
        }
        if (false == init(IP, port))
        {
            puts("Server listen socket created false");
            return false;
        }
        return true;
    }

    const int _fd() const { return server_listen_socket_fd; };

    const struct sockaddr_in *server_address_() const { return &server_address; }

    int get_client(class socket_connect_to_client &a)
    {
        memset(a.get_address(), 0, sizeof(sockaddr_in));

        socklen_t sock_len = sizeof(struct sockaddr_in);

        a.to_client_socket_fd = accept(this->server_listen_socket_fd, (struct sockaddr *)a.get_address(), &sock_len);

        return a.to_client_socket_fd;
    }
    int get_client() { return accept(this->server_listen_socket_fd, NULL, NULL); }

private:
    bool init(const std::string &ip_address, const std::string &port)
    {
        // 创建socket
        server_listen_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_listen_socket_fd == -1)
        {
            perror("Failed to create socket");
            goto fail_do;
        }

        // 设置服务器地址
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(ip_address.c_str());
        server_address.sin_port = htons(std::stoi(port));

        // 绑定地址和端口号
        if (bind(server_listen_socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
        {
            perror("Failed to bind socket");
            goto fail_do;
        }

        // 监听客户端连接
        if (listen(server_listen_socket_fd, SOMAXCONN) == -1)
        {
            perror("Failed to listen for incoming connections");
            goto fail_do;
        }
        return true;
        // 初始化client_address
        // memset(&client_address, 0, sizeof(client_address));

    fail_do:
        puts("init server listen socket fail");
        return false;
    }

    int server_listen_socket_fd = -1;
    struct sockaddr_in server_address;
    // struct sockaddr_in client_address;
};

struct item_of_works // 在创建时就将 fun 定义好
{
    size_t action;
    int (*fun)(std::string *, socket_connect_to_client *, MySql *, char *); // 这里的sql指针 指向已经链接好的 每一个线程不同的 链接字段
    std::string *message;                                                   // need delete
    socket_connect_to_client *point;                                        // 指向信息所属的 socket_fd :> class socket_connect_to_client
    char *timestamp = NULL;                                                 // size = 28 strlen("0000-00-00 00:00:00.000000")+1
};                                                                          // 用来塞进消息队列

class thread_pool
{
public:
    thread_pool(size_t threads_num) : my_condition(), my_mutex(), is_stop(false)
    {
        for (size_t i = 0; i < threads_num; i++)
        {
            my_threads.emplace_back(thread_begin,
                                    &my_work_queue,
                                    &my_mutex,
                                    &my_condition,
                                    &is_stop);
        }
    }

    ~thread_pool() { stop(); }

    void submit(item_of_works work)
    {
        std::lock_guard<std::mutex> lock(my_mutex);
        my_work_queue.push(work);
        my_condition.notify_one();
    }

    void stop()
    {
        {
            GLOG_INFO("\tMain_THREAD GET Pool_LOCKED" << std::endl, 1);
            std::unique_lock<std::mutex> lock(my_mutex);
            is_stop = true;
            // std::cout << &is_stop << std::endl;
            GLOG_INFO("\tMain_THREAD RELASE Pool_LOCKED" << std::endl, 1);
        }
        try
        {
            my_condition.notify_all();
        }
        catch (const std::exception &ex)
        {
            GLOG_ERROR("notify_all() ERROE", 1);
        }

        for (auto &thread : my_threads)
        {
            if (thread.joinable())
                thread.join();
            // std::cout << "Thread " << thread.get_id() << " is stopped";
        }
    }

private:
    std::vector<std::thread> my_threads;
    std::queue<item_of_works> my_work_queue;
    std::mutex my_mutex;
    std::condition_variable my_condition;
    bool is_stop = false;
};

struct user
{
    user(socket_connect_to_client *a, std::string name, std::string id)
        : client(a), name(name), id(id){};
    user() = default;
    const std::string &get_id() const { return id; };

    std::string name = "";
    std::string id = "";
    bool is_alive = false;
    std::set<std::string> unfriend;
    std::mutex unfriend_set_lock;
    socket_connect_to_client *client = nullptr;
};

class My_shared_mutex
{
public:
    // 共享锁的加锁操作
    void lock_shared()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        while (writer_) // 如果有写者，则等待
            cv_.wait(lk);
        ++readers_; // 增加读者数量
    }

    // 共享锁的解锁操作
    void unlock_shared()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        --readers_;        // 减少读者数量
        if (readers_ == 0) // 如果没有读者了，则唤醒一个等待的写者线程
            cv_.notify_one();
    }

    // 排他锁的加锁操作
    void lock()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        while (writer_ || readers_ > 0) // 如果有写者或者有读者，则等待
            cv_.wait(lk);
        writer_ = true; // 设置为有写者
    }

    // 排他锁的解锁操作
    void unlock()
    {
        std::unique_lock<std::mutex> lk(mtx_);
        writer_ = false;  // 设置为无写者
        cv_.notify_all(); // 唤醒所有等待的线程（包括读者和写者）
    }

private:
    std::mutex mtx_;             // 互斥锁
    std::condition_variable cv_; // 条件变量
    bool writer_ = false;        // 是否有写者
    int readers_ = 0;            // 读者数量
};

class userlist
{
public:
    userlist() = default;
    // nullptr too not found
    // 返回应有指针
    user *search(std::string id)
    {
        user_list_shared_mutex.lock_shared();
        if (users.find(id) == users.end())
        {
            user_list_shared_mutex.unlock_shared();
        }
        else
        {
            user *a = users[id];
            user_list_shared_mutex.unlock_shared();
            return a;
        }
        return nullptr;
    }
    int insert(std::string &id, user *a)
    {
        user_list_shared_mutex.lock();
        users.emplace(id, a);
        user_list_shared_mutex.unlock();
        return true;
    }
    int del(std::string const &id)
    {
        user_list_shared_mutex.lock();
        auto it = users.find(id);
        if (it != users.end())
        {
            users.erase(it);

            user_list_shared_mutex.unlock();
            return true;
        }

        // 如果未找到对应元素，也要释放互斥锁
        user_list_shared_mutex.unlock();
        return false;
    }

private:
    class My_shared_mutex user_list_shared_mutex;
    std::map<std::string, class user *> users;
};

class MySql
{
public:
    // 构造函数
    MySql() : buffer_size(BUFFER_SIZE), buffer(NULL), result(NULL), row_count_now(0), row_size(0), row(NULL)
    {
        mysql = mysql_init(NULL); // 初始化MySQL连接对象
        if (mysql == NULL)        // 处理连接对象初始化失败的情况
        {
            fprintf(stderr, "Error initializing MySQL: %s\n", mysql_error(mysql));
            exit(1);
        }
        mysql = mysql_real_connect(mysql, "localhost", "root", "wangyibodemima", "chatroom", 0, NULL, 0); // 连接到指定的数据库
        if (mysql == NULL)                                                                                // 处理连接到数据库失败的情况
        {
            fprintf(stderr, "Error connecting to MySQL: %s\n", mysql_error(mysql));
            exit(1);
        }
        buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE); // 分配用于存储转义后字符串的缓冲区
        if (buffer == NULL)                                  // 处理分配缓冲区失败的情况
        {
            fprintf(stderr, "Error allocating memory for buffer.\n");
            exit(1);
        }
    }
    // 析构函数
    ~MySql()
    {
        if (mysql != NULL) // 关闭MySQL连接
            mysql_close(mysql);
        if (buffer != NULL) // 释放缓冲区内存
            free(buffer);
        if (result != NULL)
            free_result();
    }
    // 转义字符串并返回指向缓冲区的指针
    char const *const to_escape_text(std::string &command) { return escapeString(command); }
    // 执行SQL指令并返回执行结果
    int do_(std::string &command)
    {
        GLOG_WARNING(command.c_str(), 0);
        return mysql_query(mysql, command.c_str());
    }
    // 获取查询结果
    MYSQL_RES *get_result()
    {
        if (result == NULL)                     // return mysql_result
            result = mysql_store_result(mysql); // 从MySQL连接对象获取查询结果并存储
        return result;
    }
    // 释放查询结果集 结果集指针指向 NULL
    void free_result()
    {
        if (result != NULL)
        {                              // return mysql_
            mysql_free_result(result); // 去除已存储的查询结果集并释放相关内存
            result = NULL;
        }
    }
    // 获取查询结果的一行数据
    MYSQL_ROW get_row()
    {
        row = mysql_fetch_row(result); // 从查询结果集中获取下一行数据
        return row;
    }
    // 获取最后插入数据的ID值 // 需要reset
    char *get_last_insert_id()
    {
        mysql_query(mysql, "SELECT LAST_INSERT_ID()"); // 执行SELECT LAST_INSERT_ID()指令
        result = mysql_store_result(mysql);            // 获取执行结果
        return mysql_fetch_row(result)[0];             // 返回最后插入数据的ID值
    }

    MYSQL_ROW get_rows_from_result() // 循环调用 到末尾返回 NULL
    {
        if (row_size == 0)
            row_size = mysql_num_rows(get_result());
        if (row_count_now < row_size)
        {
            ++row_count_now;
            return (row = mysql_fetch_row(result));
        }
        else
            return NULL;
    };
    bool reset() // 需要显示调用
    {
        memset(buffer, 0, strlen(buffer) + 1);
        free_result();
        row = NULL;
        row_size = 0;
        row_count_now = 0;
        return true;
    }

    MYSQL *mysql = NULL; // MySQL连接对象指针
    // 私有变量
    MYSQL_RES *result;  // 查询结果集指针
    MYSQL_ROW row;      // 存储查询结果的一行数据
    uint row_size;      // 结果集合行数
    uint row_count_now; // 结果集合 遍历到哪了
private:
    char *buffer;       // 用于存储转义后字符串的缓冲区指针
    size_t buffer_size; // 缓冲区大小
    // 私有方法：将传入的字符串进行转义，并返回指向缓冲区的指针
    char *escapeString(std::string &str)
    {
        if ((str.size() * 2 + 1) > buffer_size) // 如果缓冲区大小不足以保存转义后的字符串
        {
            buffer_size *= (str.size() * 2 + 1) * 2;       // 扩大缓冲区大小
            buffer = (char *)realloc(buffer, buffer_size); // 重新分配内存
        }
        mysql_real_escape_string(mysql, buffer, str.c_str(), str.length()); // 转义字符串并将结果存储到缓冲区中
        return buffer;
    }
};

extern userlist My_user_list;

void Print_Socket_addr(const struct sockaddr *address, FILE *stream);

int check_username_password(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int set_username_password(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp); // 初始化 用户列表

int send_message_to_user(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int send_message_to_group(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int request_make_friend(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int answer_make_friend(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int request_join_group(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int answer_join_group(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int request_rename_user(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int request_rename_group(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int download_user_data(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int download_group_data(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int send_file_data(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int recv_file_data(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int manage_group_member(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int remind_user_member(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int remind_group_member(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int dead_notification(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

int make_group(
    std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp);

// void thread_begin(std::queue<item_of_works> &work_queue, std::mutex &mtx, std::condition_variable &cond_var, bool is_stop); // thread start function

// 设置一个由时间决定的(毫秒级)序列
std::string &getMillisecondTimestamp(std::string &timestamp);

char *get_current_timestamp(char *timestamp);

extern int (*funs[FUNCTION_NUMS + 1])(std::string *, socket_connect_to_client *, MySql * , char *); // 函数指针数组
#endif
