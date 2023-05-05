#pragma once
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
#include <stack>
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
#include <sys/select.h>

#include <thread>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <sys/sendfile.h>
#include <mysql/mysql.h>
#include <sys/time.h>
#include <signal.h>

#include "./option.hh"
#include "./view.hh"
// #include "thread_threadpool.hh"
// #include "filesys.hh"

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

class user_sk;
class ServerConnection_sock;
struct job_in_Work_Queue;
class Work_Queue; // 会很多线程可见
class Ctrl_Data;
class Ctrl_Data_On_Ctrl;
class Ctrl_Data_On_View;
class friend_data;
class group_data;

void Thread_Recv_fr_Net_begin_fun();
void Thread_Talk_to_User_begin_fun();
inline std::string chatid_to_filename(std::string chatid)
{
    if (chatid == "notices")
        chatid = "111111111111111111";
    std::string result(chatid.length(), ' ');

    srand(static_cast<unsigned int>(
        std::stoll((chatid.length() > 18) ? chatid.substr(chatid.length() - 18, chatid.length()) : chatid))); // 初始化随机数种子

    for (int i = 0; i < chatid.length(); i++)
    {
        int rand_num = rand() % 26 + 'A' + ((rand() % 26) % 2 ? ('a' - 'A') : 0); // 随机生成一个大写字母对应的 ASCII 码
        result[i] = static_cast<char>(rand_num);                                  // 将 ASCII 码转换为字符并赋值给字符串对应位置
    }

    return result;
};
std::string &getMillisecondTimestamp(std::string &timestamp);
char *make_timestamp_to_net(char *timestamp);

class user_sk
{
public:
    int _fd() const { return to_client_socket_fd; }

    user_sk(user_sk &&other)
    {
        this->to_client_socket_fd = other.to_client_socket_fd;
        memcpy(&this->client_address, &other.client_address, sizeof(struct sockaddr_in));
        other.to_client_socket_fd = -1; // 将原先的 socket 变成无效（-1）
    }
    user_sk() = default;
    void skt_fd_del_clo()
    {
        close(to_client_socket_fd);
        delete this;
    }
    user_sk(int fd)
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
    user_sk(int fd, struct sockaddr_in *addr) : to_client_socket_fd(fd) { memcpy(&client_address, addr, sizeof(struct sockaddr_in)); }
    void close_fd()
    {
        // std::cout << "user_sk closed " << std::endl;
        close(to_client_socket_fd);
        to_client_socket_fd = -1;
    };

    bool recv_message(std::string &message, size_t &action, char *timestamp)
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
        if ((size_received = recv(to_client_socket_fd, timestamp, strlen("0000-00-00 00:00:00.000000"), MSG_WAITALL)) <= 0)
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
    bool send_message(const std::string &message, const size_t &action, char const *timestamp)
    {

        // 保证 读写操作线程安全
        std::unique_lock<std::mutex> lock(client_write_mutex);

        size_t size = message.size();
        if (!(send(to_client_socket_fd, &size, sizeof(size_t), MSG_MORE) > 0 &&
              send(to_client_socket_fd, timestamp, strlen("0000-00-00 00:00:00.000000"), MSG_MORE) > 0 &&
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

class ServerConnection_sock
{
public:
    std::string ip = "";
    std::string port = "";
    int sk_fd = -1;
    struct sockaddr_in server_address;

    ServerConnection_sock(std::string ip, std::string port) : ip(ip), port(port) { connect(); };
    ~ServerConnection_sock()
    {
        if (sk_fd != -1)
            close(this->sk_fd);
    };

private:
    bool connect()
    {
        // 创建套接字
        this->sk_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (this->sk_fd == -1)
        {
            perror("创建套接字失败");
            return false;
        }

        // 设置服务器的地址信息
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_addr.s_addr = inet_addr(this->ip.c_str());
        server_address.sin_port = htons(atoi(this->port.c_str()));

        // 连接服务器
        int result = ::connect(this->sk_fd, (struct sockaddr *)&server_address, sizeof(server_address));
        if (result != 0)
        {
            perror("连接服务器失败");
            close(this->sk_fd);
            return false;
        }

        return true;
    };
};

// 所有包里的指针全是new的 // 直接调用free free会delete所有的东西
struct job_in_Work_Queue
{
    job_in_Work_Queue(size_t mode, std::string *message) : message(message), mode(mode), self(this) {}
    job_in_Work_Queue(size_t mode, std::string *message, char *timestamp) : message(message), mode(mode), self(this), timestamp(timestamp) {}
    void free(job_in_Work_Queue **job)
    {

        if ((*job) == nullptr)
            return;
        delete (*job)->message;
        delete (*job)->timestamp;
        delete (*job)->self;
        *job = nullptr;
    }
    ~job_in_Work_Queue() = default;
    size_t mode;
    std::string *message = nullptr;
    char *timestamp = nullptr;
    job_in_Work_Queue *self = this;
};
/**
 * pop() 是 非阻塞
 * 有东西返回new的 ptr
 * 没东西/被占用 返回 nullptr
 */
class Work_Queue // 会很多线程可见
{
public:
    Work_Queue(bool i) : read_fd(pipe[0]), write_fd(pipe[1]), is_need_lock(i)
    {
        ::pipe(this->pipe);
        buffer = new char[sizeof(job_in_Work_Queue)];
    };
    ~Work_Queue()
    {
        delete[] buffer;
        close(pipe[0]);
        close(pipe[1]);
    };
    int const &write_fd;
    int const &read_fd;
    job_in_Work_Queue *pop()
    {
        if (is_need_lock)
        {
            {
                std::unique_lock<std::mutex> lock(read_mutex);
                read(read_fd, buffer, sizeof(job_in_Work_Queue));
            }
        }
        else
            read(read_fd, buffer, sizeof(job_in_Work_Queue));
        return ((job_in_Work_Queue *)buffer)->self;
    }
    bool submit(size_t mode, std::string *message)
    {
        char *timestamp = nullptr;
        timestamp = make_timestamp_to_net(timestamp);
        job_in_Work_Queue *job = new job_in_Work_Queue(mode, message);
        job->timestamp = timestamp;
        if (is_need_lock)
        {
            std::unique_lock<std::mutex> lock(write_mutex);
            write(write_fd, job, sizeof(job_in_Work_Queue));
        }
        else
            write(write_fd, job, sizeof(job_in_Work_Queue));
        return true;
    }
    bool submit(job_in_Work_Queue *j)
    {
        if (is_need_lock)
        {
            std::unique_lock<std::mutex> lock(write_mutex);
            write(write_fd, j, sizeof(job_in_Work_Queue));
        }
        else
            write(write_fd, j, sizeof(job_in_Work_Queue));
        return true;
    }
    bool submit(size_t mode, std::string *message, char *timestamp)
    {
        job_in_Work_Queue *j = new job_in_Work_Queue(mode, message, timestamp);
        if (is_need_lock)
        {
            std::unique_lock<std::mutex> lock(write_mutex);
            write(write_fd, j, sizeof(job_in_Work_Queue));
        }
        else
            write(write_fd, j, sizeof(job_in_Work_Queue));
        return true;
    }

private:
    bool is_need_lock = true;
    std::mutex read_mutex;
    std::mutex write_mutex;
    int pipe[2];
    char *buffer;
};

class Ctrl_Data
{
public:
    static std::string user_id;
    static std::string user_name;
    static std::vector<friend_data> friends_set;
    static std::vector<group_data> groups_set;
    // 只有在用户关系变动的时候进行改变
    static bool is_changed;
    static int notices_count;

protected:
    static std::mutex data_lock;
};

class Ctrl_Data_On_Ctrl : public Ctrl_Data
{
public:
    bool lock() { return data_lock.try_lock(); }
    void unlock() { data_lock.unlock(); }
    std::string find_friend_name(std::string const &friend_id)
    {
        lock();
        for (auto &a : friends_set)
        {
            if (a.friend_id() == friend_id)
            {
                unlock();
                return a.friend_name();
            }
        }
        unlock();
        return std::string("");
    }
};

// 访问要加锁!!!!!!!!
class Ctrl_Data_On_View : private Ctrl_Data
{
public:
    std::string const &get_user_id() { return user_id; }
    std::string const &get_user_name() { return user_name; }
    std::vector<friend_data> const &get_friends_set() { return friends_set; }
    std::vector<group_data> const &get_groups_set() { return groups_set; }
    int &get_notices_count() { return this->notices_count; }
    void lock()
    {
        data_lock.lock();
    }
    bool try_lock() { return data_lock.try_lock(); }
    void unlock() { data_lock.unlock(); }
    bool &is_changed() { return std::ref(Ctrl_Data::is_changed); }

private:
    bool is_self_data_lock = false; // 保留 希望是通过^C 刷新std::in 设置信号处理函数
};

class filesystem
{

public:
    filesystem(Work_Queue *a) : wq(a){};
    std::string chatid_to_filename(std::string chatid) // returns left value
    {
        return ::chatid_to_filename(chatid);
    }
    std::string view_on_chatid;
    std::mutex filesys_lock;
    /*进入目标聊天界面时进行*/
    bool do_cache(std::string &chatid)
    {
        filesys_lock.lock();
        view_on_chatid = chatid;

        std::string filename = chatid_to_filename(view_on_chatid);

        fd = open(filename.c_str(), O_RDONLY);
        if (view_on_chatid.empty())
        {
            filesys_lock.unlock();
            return false;
        }
        size_t len = 0;
        chat_data_cache.clear();
        while ((len = read(fd, buffer, 1024)))
        {
            chat_data_cache.write(buffer, len);
        }

        close(fd);
        fd = -1;
        filesys_lock.unlock();
        return true;
        // std::cout << chat_data_cache << std::endl;
    }
    /*退出聊天界面进行*/
    void reset()
    {
        std::unique_lock<std::mutex> lock(filesys_lock);
        view_on_chatid.clear();
        // 清空流
        chat_data_cache.str("");
        chat_data_cache.clear();
        memset(buffer, 0, 1024);
    }
    /**
     * return -1 on error
     * return 0 on success
     * return 1 on need flush
     *
     *
     *
     * 进行 add_enter 后
     * 不用调用 submit(FLUSH_WINDOW, nullptr, nullptr);
     * 函数会自行调用
     */
    int add_enter(std::string chatid, const std::string &name,
                  const std::string &data, char const *timestamp, bool is_flush = true)
    {
        // std::unique_lock<std::mutex> lock(filesys_lock);
        if (chatid == "notices")
        {
            Notices_mutex.lock();
            chatid = "11111111";
        }

        std::string target;
        size_t timestamp_len = strlen("0000-00-00 00:00:00");
        char timestampbuff[timestamp_len + 1];            // 多分配一字节空间
        strncpy(timestampbuff, timestamp, timestamp_len); // 拷贝 timestamp 数据
        timestampbuff[timestamp_len] = '\0';              // 手动添加结束符
        target.append("[");
        target.append(timestampbuff);
        target.append("]");
        target.append(name);
        target.append(": ");
        target.append(data);
        target.append("\n");
        {
            std::unique_lock<std::mutex> lock(filesys_lock);
            std::fstream file("./chat_history/" + chatid_to_filename(chatid), std::ios::app);
            if (file.is_open() == false)
                return -1;
            file << target << std::flush;

            if (is_flush && chatid == view_on_chatid)
            {
                chat_data_cache << target << std::flush;
                wq->submit(FLUSH_WINDOW, nullptr, nullptr);
                return 0;
            }
        }
        if (chatid == "11111111")
            Notices_mutex.unlock();

        return true;
    }

    Work_Queue *wq; // 需要赋值 | view 的消息获取队列 用来通知需要flush
    std::stringstream chat_data_cache;
    std::mutex Notices_mutex;
    int fd = -1;
    char buffer[1024];
};