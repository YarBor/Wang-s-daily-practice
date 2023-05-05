#include "client.hh"
#include "view.hh"

/**
 * 对于用户的输入
 * ": "开始的是命令
 * 通过命令操控程序界面的切换
 * 功能的实现
 *
 * 对于纯文本 输入
 * 一律认为是 chat内容
 * ":: "--> ": "
 * 在聊天中 开头的":: " 将被识别为 ': '"
 */

std::stack<Window> windows;

using namespace std;
using nlohmann::json;
#define success true

// 保留字段 (我忘了这是干啥的了 但是不敢删)
extern nlohmann::json *cache;

extern Ctrl_Data_On_View *Data_On_View_ptr;
// extern Ctrl_Data_On_Ctrl Data_On_Ctrl;

// 这个线程对这个指针只读 不修改指针
// (要在ctrl处理好指针指向)
extern Work_Queue *queue_btw_Ctrl_View_ptr;
extern Work_Queue *Ctrl_Work_Queue;
// Work_Queue *queue_btw_Ctrl_View_ptr = nullptr;

// 防止 ctrl 和 view 同时对一个chatid 映射的文件 进行操作
extern filesystem *filesys;
// extern std::mutex view_using_chatid_mutex;
// extern std::string view_using_chatid;

bool login(Work_Queue &Ctrl_Work_Queue, Work_Queue &queue_btw_Ctrl_View)
{
    while (1)
    {
        clear_window;
        string id;
        string password;

        // 提示用户输入用户名和密码
        cout << "请输入您的用户ID：";
        cin >> id;
        if (!id.empty() && std::all_of(id.begin(), id.end(), [](unsigned char c)
                                       { return std::isdigit(c); }))
            continue;
        cout << "请输入您的密码：";
        cin >> password;

        json j = {{"id", id.c_str()}, {"password", password.c_str()}};
        string *message = new string(j.dump());
        Ctrl_Work_Queue.submit(ACTION_TO_CHECK_USERNAME_PASSWORD, message);
        job_in_Work_Queue *job; // new 的
        bool is_success = false;

        cout << "loading..." << endl;
        while ((job = queue_btw_Ctrl_View_ptr->pop()) != nullptr)
        {
            json j_tmp;
            string str_tmp;
            switch (job->mode)
            {
            case ANSWER_TO_CHECK_USERNAME_PASSWORD_SUCCESS:
                is_success = true;
                cout << Data_On_View_ptr->get_user_name() << "(" << Data_On_View_ptr->get_user_id() << ")  \t"
                     << "Log In SUCCESS" << endl;
                job->free(&job);
                delete job;
                break;

            case SQL_ERROE_IN_CHECK:
                j_tmp = json::parse(*((std::string *)job->message));
                str_tmp = j_tmp.at("error");
                cout << "Log In FALSE" << endl;
                cout << str_tmp << endl;
                job->free(&job);
                delete job;
                break;

            case NOT_FIND_IN_CHECK:
            case WRONG_PASSWORD:
                j_tmp = json::parse(*((std::string *)job->message));
                str_tmp = j_tmp.at("false");
                cout << "Log In FALSE" << endl;
                cout << str_tmp << endl;
                job->free(&job);
                delete job;
                break;

            default:
                assert(false);
            }
        }
        system("pause");
        if (is_success)
            break;
    }
    return true;
}
bool setup(Work_Queue &Ctrl_Work_Queue, Work_Queue &queue_btw_Ctrl_View)
{
    clear_window;
    while (1)
    {
        string name;
        string password;

        // 提示用户输入用户名和密码
        cout << "请输入您的用户名：";
        cin >> name;
        if (name.end() != (std::find_if(name.begin(), name.end(), [&](char c)
                                        { return !(std::isalnum(c) || c == '_'); })))
            continue;
        cout << "请输入您的密码：";
        cin >> password;

        json j = {{"name", name.c_str()}, {"password", password.c_str()}};
        string *message = new string(j.dump());
        Ctrl_Work_Queue.submit(ACTION_TO_SET_USERNAME_PASSWORD, message);
        job_in_Work_Queue *job; // new 的
        bool is_success = false;
        cout << "loading..." << endl;
        while ((job = queue_btw_Ctrl_View_ptr->pop()) != nullptr)
        {
            json j_tmp;
            string str_tmp;
            switch (job->mode)
            {
            case SET_USERNAME_PASSWORD_SUCCESS:
                cout << Data_On_View_ptr->get_user_name() << "(" << Data_On_View_ptr->get_user_id() << ")  \t"
                     << "Set Up SUCCESS" << endl;
                job->free(&job);
                delete job;
                break;

            case SQL_ERROR_IN_SET:
                j_tmp = json::parse(*((std::string *)job->message));
                str_tmp = j_tmp.at("error");
                cout << "Log In FALSE" << endl;
                cout << str_tmp << endl;
                job->free(&job);
                delete job;
                break;
            default:
                assert(false);
            }
        }
        system("pause");
        clear_window;
        if (is_success)
            break;
    }
    return true;
}
bool before_begin(Work_Queue &Ctrl_Work_Queue, Work_Queue &queue_btw_Ctrl_View)
{
    std::string choice;
    do
    {
        cout << "========== 菜单 ==========" << endl;
        cout << "1. 注册" << endl;
        cout << "2. 登录" << endl;
        cout << "3. 退出" << endl;
        cout << "请输入您的选择（1-3）: ";
        cin >> choice;

        if (choice == "1")
        {
            cout << "您已选择注册" << endl;
            // Your code for registration
            if (setup(Ctrl_Work_Queue, queue_btw_Ctrl_View))
                break;
        }

        else if (choice == "2")
        {
            cout << "您已选择登录" << endl;
            if (login(Ctrl_Work_Queue, queue_btw_Ctrl_View))
                break;
        }
        else if (choice == "3")
        {

            cout << "感谢使用我们的服务，再见！" << endl;
            return false;
        }
        else
        {
            cout << "无效的选择，请重新输入。" << endl;
        }
    } while (choice != "3");
    return true;
}
// bool load_data(string *message, char const *name, char const *chatid, char *timestamp);
bool is_command(std::string *message) { return ((*message).size() >= 2 && (*message)[0] == ':' && (*message)[1] == ' '); }

int Handle_User_Input(std::string *message)
{
    win_stat const status = windows.top().stat_ref;
    if (is_command(message))
    {
        /**
         * 跳到下一个屏幕
         * 判断需不需要进行
         * 文件缓存
         * 缓存文件流进行 reset()
         */
    }
    else
    {
        json j;
        // string data_To_Net;
        switch (status)
        {
        case Friend_Chat:
        {
            char *timestamp = nullptr;
            timestamp = make_timestamp_to_net(timestamp);
            // load_data(message, windows.top().user_name_ref.c_str(), windows.top().friend_chat_now.friend_chatid().c_str(), timestamp);
            ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
            j = {{"from_id", windows.top().user_id_ref.c_str()},
                 {"chat_id", windows.top().friend_chat_now.friend_chatid().c_str()},
                 {"data", (*message).c_str()},
                 {"target_id", windows.top().friend_chat_now.friend_id().c_str()}};
            *message = j.dump();
            Ctrl_Work_Queue->submit(SEND_USER_MESSAGE_TO_SERVER, message, timestamp);
            /*把信息交到ctrl线程中 让ctrl进行处理
              让ctrl线程通知 view线程进行 flush*/
            return true;
        }
        case Group_Chat:
        {
            char *timestamp = nullptr;
            timestamp = make_timestamp_to_net(timestamp);

            j = {{"chat_id", windows.top().group_chat_now.group_chatid().c_str()},
                 {"group_id", windows.top().group_chat_now.group_id().c_str()},
                 {"own_id", windows.top().user_id_ref.c_str()},
                 {"own_name", windows.top().user_name_ref.c_str()},
                 {"data", (*message).c_str()},
                 {"time", timestamp}};
            *message = j.dump();
            Ctrl_Work_Queue->submit(SEND_USER_MESSAGE_TO_SERVER, message, timestamp);

            return true;
        }
        default:
            return false;
        }
    }
    delete message;
    return true;
}
/**
 * 数据段更新时 传data_update
 *
 * 在调用处free 这里不用管
 */
int Handle_Ctrl_Job(job_in_Work_Queue *job)
{
    //    Data_On_View_ptr->lock();
    //    if (Data_On_View_ptr->is_changed())
    //    {
    //        windows.top().re_init(*cache, Data_On_View_ptr->get_user_id(), Data_On_View_ptr->get_user_name(), Data_On_View_ptr->get_friends_set(),
    //                              Data_On_View_ptr->get_groups_set(), Data_On_View_ptr->is_changed());
    //        Data_On_View_ptr->is_changed() = false;
    //    }
    //    Data_On_View_ptr->unlock();
    switch (job->mode)
    {
    case FLUSH_WINDOW:
        filesys->filesys_lock.lock();
        windows.top().flush_out(filesys->chat_data_cache, Data_On_View_ptr);
        filesys->filesys_lock.unlock();
        break;
    case DATA_UPDATE: // update 不 flush
        Data_On_View_ptr->lock();
        if (Data_On_View_ptr->is_changed())
            windows.top().re_init(*cache, Data_On_View_ptr->get_user_id(), Data_On_View_ptr->get_user_name(), Data_On_View_ptr->get_friends_set(), Data_On_View_ptr->get_groups_set(), Data_On_View_ptr->is_changed());
        Data_On_View_ptr->is_changed() = false;
        Data_On_View_ptr->unlock();
        // windows.top().Window::flush_out(filesys->chat_data_cache,Data_On_View_ptr);
        queue_btw_Ctrl_View_ptr->submit(FLUSH_WINDOW,nullptr,nullptr);
        break;
    }

    return true;
}

// Window &window_now ;
void Thread_Talk_to_User_begin_fun()
{
    // Ctrl_Data_On_View Data;// !! 不应该在这里创建
    // data = &Data;// !! 不应该在这里创建
    if (!before_begin(*Ctrl_Work_Queue, *queue_btw_Ctrl_View_ptr))
        return;
    // 在这里之前 因为是值创建 所以一定是初始化好了之后 before_begin()才返回
    windows.emplace(*cache, Data_On_View_ptr->get_user_id(), Data_On_View_ptr->get_user_name(), Data_On_View_ptr->get_friends_set(), Data_On_View_ptr->get_groups_set(), false);

    fd_set fdsInSelect;
    FD_ZERO(&fdsInSelect);
    FD_SET(STDIN_FILENO, &fdsInSelect);
    FD_SET(queue_btw_Ctrl_View_ptr->read_fd, &fdsInSelect);
    // FD_SETSIZE();
    int returnfd = -1;
    /**
     * 监听两个文件描述符
     * 标准输入和队列的管道    */
    while ((returnfd = select(max(STDIN_FILENO, queue_btw_Ctrl_View_ptr->read_fd) + 1, &fdsInSelect, NULL, NULL, NULL)) != -1)
    {
        if (returnfd == STDIN_FILENO)
        {
            // std::string *message = new std::string;
            // std::cin >> *message;
            string *message = windows.top().get_input();
            int rv;
            if ((rv = Handle_User_Input(message)) == success) // message 是new出来的
                continue;
            else if (rv == false)
            {
                windows.top().is_wrong_lasttime = true;
                windows.top().Window::flush_out(filesys->chat_data_cache, Data_On_View_ptr);
            }
        }
        else if (returnfd == queue_btw_Ctrl_View_ptr->read_fd)
        {
            job_in_Work_Queue *job = queue_btw_Ctrl_View_ptr->pop();
            Handle_Ctrl_Job(job);
            job->free(&job);
        }
    }
    return;
}
/**
 * message 是 从view 拿到的用户输入
 * name 是要求记录的目标名
 * chatid 要映射到文件名
 * timestamp 在这里之前new出来的 之后还要发给server 作为包的时间戳
 *
 * 工作被 class filesys 替代
 */
// bool load_data(string *message, char const *name, char const *chatid, char *timestamp)
// {

// }
