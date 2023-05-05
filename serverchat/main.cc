#include "server.hh"
// #include "MysqlClass.hh"

std::set<class socket_connect_to_client *> *sockets_pointer = nullptr; // 是一个set的指针 保存所有已链接的socket

userlist My_user_list; // 缓存在线用户，使用map 以 id · user 的 kv 形式保存
thread_pool *My_thread_pools_ptr;

int (*funs[FUNCTION_NUMS + 1])(std::string *, socket_connect_to_client *, MySql *, char *) = {
    nullptr,
    check_username_password,
    set_username_password,
    send_message_to_user,
    send_message_to_group,
    request_make_friend,
    answer_make_friend,
    request_join_group,
    answer_join_group,
    request_rename_user,
    request_rename_group,
    download_user_data,
    download_group_data,
    send_file_data,
    recv_file_data,
    manage_group_member,
    remind_user_member,
    remind_group_member,
    dead_notification,
    make_group};

void address_Glog(struct sockaddr_in const *const sa, int flag) // flag 1 LINK , 2 unLINK
{
    char ip_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(sa->sin_addr), ip_str, INET_ADDRSTRLEN);

    std::ostringstream oss;
    oss << ip_str << "::" << ntohs(sa->sin_port);

    // std::string result = oss.str();
    GLOG_INFO(oss.str() << (flag == 1 ? " LINK" : " unLINK"), false);
}
void sigint_handler(int signum); // 信号处理函数

int main(int argc, char **argv)
{
    // 创建服务端监听socket对象
    server_listen_socket listen_socket("10.30.0.170", "52341");

    //  google::InitGoogleLogging(argv[0]); // INIT GLOG  ____ in /tmp/?????.INOF

    mysql_library_init(0, NULL, NULL); // 初始化MySQL库

    // 如果创建失败，打印错误信息并退出程序
    if (listen_socket._fd() == -1)
    {
        perror("server_listen_socket init failed");
        exit(1);
    }

    // 创建 epoll 监听器
    int epfd = epoll_create(100);

    // 如果创建失败，打印错误信息并退出程序
    if (epfd == -1)
    {
        perror("epoll_create failed");
        exit(1);
    }

    // 动态分配内存以保存指向 socket_connect_to_client 对象的指针
    sockets_pointer = new std::set<class socket_connect_to_client *>;

    // 创建 epoll_event 对象并设置为监听指定文件描述符 epfd 上的传入事件。
    epoll_event listen_socket_event;
    // listen_socket_event.data.fd = listen_socket._fd();
    listen_socket_event.events = EPOLLIN;
    listen_socket_event.data.ptr = &listen_socket; // 使用指针在事件处理程序函数中访问 listen_socket 对象
    std::cout << epoll_ctl(epfd, EPOLL_CTL_ADD, listen_socket._fd(), &listen_socket_event);
    std::cout << std::endl;
    // 创建线程池对象
    thread_pool My_thread_pools(MAX_THREAD_NUM);

    My_thread_pools_ptr = &My_thread_pools;
    // 设置信号处理函数
    signal(SIGINT, sigint_handler);

    // 声明一个 epoll_event 对象数组，用于存储从 epoll_wait() 函数返回的事件。
    epoll_event my_epoll_events_arr[EPOLL_EVENTS_NUM];

    // 进入循环，监听 epoll 事件。
    while (true)
    {
        // 清空 epoll_events 数组
        memset(my_epoll_events_arr, 0, EPOLL_EVENTS_NUM * sizeof(struct epoll_event));

        // 等待 epoll 事件，每次最多处理 EPOLL_EVENTS_NUM 个事件
        ssize_t event_num = epoll_wait(epfd, my_epoll_events_arr, EPOLL_EVENTS_NUM, -1);

        // 如果 epoll_wait() 函数返回 -1，则表示发生了错误
        if (event_num == -1)
        {
            perror("epoll_wait error");
            exit(-1);
        }

        // 遍历获取到的事件
        for (ssize_t i = 0; i < event_num; i++)
        {
            // 如果该事件是新连接请求
            if (my_epoll_events_arr[i].data.ptr == &listen_socket)
            {
                // 创建一个新的 socket_connect_to_client 对象，用于处理客户端请求

                socket_connect_to_client *new_skfd = new socket_connect_to_client(
                    ((server_listen_socket *)my_epoll_events_arr[i].data.ptr)->get_client());

                int flags = fcntl(new_skfd->_fd(), F_GETFL, 0);
                flags &= ~O_NONBLOCK;
                fcntl(new_skfd->_fd(), F_SETFL, flags);

                using namespace std;
                cout << "linked" << endl;

                // 在 listen_socket 上获取新的客户端socket对象
                address_Glog(new_skfd->get_address(), 1); // 将链接写到日志

                // 将新对象保存到 sockets_pointer 中，方便后面使用
                sockets_pointer->insert(new_skfd);

                // 将新对象的文件描述符添加到 epoll 监听器中
                epoll_event tmp;
                // tmp.data.fd = new_skfd->_fd();
                memset(&tmp, 0, sizeof(epoll_event));
                tmp.data.ptr = new_skfd;
                tmp.events = EPOLLIN | EPOLLHUP | EPOLLRDHUP | EPOLLERR;
                // 如果添加失败，打印错误信息并退出程序
                if (-1 == epoll_ctl(epfd, EPOLL_CTL_ADD, new_skfd->_fd(), &tmp))
                {
                    perror("epoll_ctl add socket fd failed");
                    exit(-1);
                }
                // GLOG_INFO("epoll_ctl add socket fd",1);
            }

            // 如果对端已经挂断
            else if (my_epoll_events_arr[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                // 从 epoll 监听器中删除该文件描述符
                epoll_ctl(epfd, EPOLL_CTL_DEL,
                          ((socket_connect_to_client *)my_epoll_events_arr[i].data.ptr)->_fd(),
                          nullptr);

                // 从链接的sockets set中删除挂断者
                sockets_pointer->erase(
                    static_cast<socket_connect_to_client *>(my_epoll_events_arr[i].data.ptr));

                // write client unLINK in log
                address_Glog(((socket_connect_to_client *)my_epoll_events_arr[i].data.ptr)->get_address(), 2); // 将链接写到日志

                std::string dead_id;

                // 未登陆状态 --> owner_ptr 是自动构造的 是nullptr
                if (((socket_connect_to_client *)my_epoll_events_arr[i].data.ptr)->owner == nullptr)
                    ;
                else
                {
                    // 登陆状态
                    dead_id = (((socket_connect_to_client *)my_epoll_events_arr[i].data.ptr)->owner->id);
                    // 在全局的在线用户列表中找挂断者
                    if (My_user_list.del(dead_id))
                    { // 通知亲友
                        std::string *tmp = new std::string(dead_id);
                        item_of_works item;
                        item.message = tmp;
                        item.point = NULL;
                        item.action = ACTION_TO_DEAD_NOTIFICATION;
                        item.fun = dead_notification;
                        
                        item.timestamp = (char *)malloc(28);
                        get_current_timestamp(item.timestamp);
                        item.timestamp[27] = '\0';

                        My_thread_pools.submit(item);
                    }

                    // 将这个user类delete掉
                    delete ((socket_connect_to_client *)my_epoll_events_arr[i].data.ptr)->owner;
                }
                // 将connect类delete掉
                static_cast<socket_connect_to_client *>(my_epoll_events_arr[i].data.ptr)->close_fd();
                delete (socket_connect_to_client *)my_epoll_events_arr[i].data.ptr;
            }

            // 处理输入事件
            else if (my_epoll_events_arr[i].events & EPOLLIN)
            {
                // 构造一个新字符串对象，用于存储读取的数据以及 JSON 解析
                std::string *message = new std::string();

                // 获取请求并进行处理
                item_of_works item;
                item.point = static_cast<socket_connect_to_client *>(my_epoll_events_arr[i].data.ptr);

                char *timestamp = (char *)malloc(strlen("0000-00-00 00:00:00.000000") + 1);
                // 读取客户端发送的请求报文，并保存到 message 对象中
                if (item.point->recv_message(*message, item.action, timestamp))
                {
                    // 将接收到的请求提交给线程池进行处理
                    item.message = message;
                    item.timestamp = timestamp;
                    item.fun = funs[item.action];
                    My_thread_pools.submit(item);
                }
            }

            // 处理其他事件
            else
            {
                std::cout << "Unknown event type" << std::endl;
            }
        }
    }

    // 删除并释放 sockets_pointer 「全局的保存所有链接上的socket结构体」占用的内存空间。
    google::ShutdownGoogleLogging();
    mysql_library_end(); // 清理MySQL库
    delete sockets_pointer;
}
