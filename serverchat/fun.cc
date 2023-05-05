#include "server.hh"

void Print_Socket_addr(const struct sockaddr *address, FILE *stream = stdin)
{
    // const struct sockaddr *address = (const struct sockaddr *)&address;
    // 如果是 IPv4 地址
    if (address->sa_family == AF_INET)
    {
        char buffer[INET_ADDRSTRLEN];
        const struct sockaddr_in *addr_in = (const struct sockaddr_in *)address;
        // 把 IP 地址转换为字符串形式
        if (inet_ntop(AF_INET, &(addr_in->sin_addr), buffer, INET_ADDRSTRLEN) != NULL)
        {
            // 输出 IP 地址和端口号
            fprintf(stream, "IPv4 address %s:%d\n", buffer, ntohs(addr_in->sin_port));
        }
    }
    // 如果是 IPv6 地址
    else if (address->sa_family == AF_INET6)
    {
        char buffer[INET6_ADDRSTRLEN];
        const struct sockaddr_in6 *addr_in6 = (const struct sockaddr_in6 *)address;
        // 把 IP 地址转换为字符串形式
        if (inet_ntop(AF_INET6, &(addr_in6->sin6_addr), buffer, INET6_ADDRSTRLEN) != NULL)
        {
            // 输出 IP 地址和端口号
            fprintf(stream, "IPv6 address %s:%d\n", buffer, ntohs(addr_in6->sin6_port));
        }
    }
    // 如果是未知地址类型
    else
    {
        fprintf(stream, "Unknown address type\n");
    }
}
int distinguish(size_t action, std::string &message)
{
    switch (action)
    {
    case ACTION_TO_CHECK_USERNAME_PASSWORD:

        break;
    case ACTION_TO_SET_USERNAME_PASSWORD:

        break;
    case ACTION_TO_SEND_MESSAGE_TO_USER:

        break;
    case ACTION_TO_SEND_MESSAGE_TO_GROUP:

        break;
    case ACTION_TO_REQUEST_MAKE_FRIEND:

        break;
    case ACTION_TO_ANSWER_MAKE_FRIEND:

        break;
    case ACTION_TO_REQUEST_JOIN_GROUP:

        break;
    case ACTION_TO_ANSWER_JOIN_GROUP:

        break;
    case ACTION_TO_REQUEST_RENAME_USER:

        break;
    case ACTION_TO_REQUEST_RENAME_GROUP:

        break;
    case ACTION_TO_DOWNLOAD_USER_DATA:

        break;
    case ACTION_TO_DOWNLOAD_GROUP_DATA:

        break;
    case ACTION_TO_SEND_FILE_DATA:

        break;
    case ACTION_TO_RECV_FILE_DATA:

        break;
    case ACTION_TO_MANAGE_GROUP_MEMBER:

        break;
    case ACTION_TO_REMIND_USER_MEMBER:

        break;
    case ACTION_TO_REMIND_GROUP_MEMBER:

        break;
    default:
        // Handle unknown action
        return -1;
    }
    return 0;
}

// 返回一个由时间决定的(毫秒级)序列
std::string &getMillisecondTimestamp(std::string &timestamp)
{
    struct timeval tv;
    gettimeofday(&tv, NULL); // 获取当前时间
    time_t nowtime = tv.tv_sec;
    struct tm *nowtm = localtime(&nowtime);
    char tmbuf[64], buf[64];

    strftime(tmbuf, sizeof(tmbuf), "%Y%m%d%H%M%S", nowtm); // 格式化年月日时分秒

    snprintf(buf, sizeof(buf), "%s%06ld", tmbuf, tv.tv_usec); // 添加微秒，构建最终时间戳

    std::stringstream ss;
    ss << buf;

    timestamp = ss.str(); // 将字符串时间戳存储在传递的string变量中

    return timestamp;
}

// 时间戳
char* get_current_timestamp(char * timestamp) {
    // static char timestamp[28];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t nowtime = tv.tv_sec;
    struct tm *nowtm = localtime(&nowtime);
    int microseconds = tv.tv_usec;
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", nowtm);
    sprintf(timestamp + strlen(timestamp), ".%06d", microseconds);
    return timestamp;
}

extern thread_pool *My_thread_pools_ptr;
extern std::set<class socket_connect_to_client *> *sockets_pointer; // 是一个set的指针 保存所有已链接的socket


void sigint_handler(int signum)
{
    std::cout << "\nSEREVER EXIT..." << std::endl;

    LOG(INFO) << "Signal fun() done , SEREVER EXIT" << std::endl;

    // std::cout << "SIGINT: " << signum << std::endl;
    My_thread_pools_ptr->stop();
    for (auto tmp_sktocl : *sockets_pointer)
    {
        if (tmp_sktocl->owner != nullptr) // 登陆了 // 有user_class
        {
            delete tmp_sktocl->owner;
        }
        tmp_sktocl->close_fd();
        delete tmp_sktocl;
        tmp_sktocl = nullptr;
    }
    mysql_library_end(); // 清理MySQL库
    delete sockets_pointer;

    // google::ShutdownGoogleLogging();
    
    exit(EXIT_SUCCESS);
    std::string a;
    // a.insert() 
}