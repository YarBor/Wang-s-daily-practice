#include "client.hh"
std::vector<std::thread> threads;

// user_socket_pointer
user_sk *u_sk_ptr = NULL;

Ctrl_Data_On_View *Data_On_View_ptr;
Ctrl_Data_On_Ctrl *Data_On_Ctrl_ptr;

// 保留字段 (我忘了这是干啥的了 但是不敢删)
nlohmann::json *json_cache = nullptr;

// extern Ctrl_Data_On_Ctrl Data_On_Ctrl;

// 这个线程对这个指针只读 不修改指针
// (要在ctrl处理好指针指向)

Work_Queue *Ctrl_Work_Queue_ptr = nullptr;
Work_Queue *queue_btw_Ctrl_View_ptr = nullptr;
filesystem *filesys = nullptr;

// 防止 ctrl 和 view 同时对一个chatid 映射的文件 进行操作
// std::mutex view_using_chatid_mutex;
// std::string view_using_chatid;

// 主线程 进行 数据处理
// 发送 | 写入文件

void Thread_Recv_fr_Net_begin_fun();
void Thread_Talk_to_User_begin_fun();

int ctrl_handle(job_in_Work_Queue *);

extern std::stack<Window> windows;

int main(int argc, char **argv)

{

    ServerConnection_sock sock_connection("10.30.0.170", "52341");
    user_sk u_sk(sock_connection.sk_fd);
    u_sk_ptr = &u_sk;

    Work_Queue Ctrl_Work_Queue(true);
    // Ctrl_Work_Queue.is_need_lock = true;
    Work_Queue queue_btw_Ctrl_View(true);
    Ctrl_Data_On_View Data_On_View;
    Ctrl_Data_On_Ctrl Data_On_Ctrl;

    Data_On_Ctrl_ptr = &Data_On_Ctrl;
    Data_On_View_ptr = &Data_On_View;
    queue_btw_Ctrl_View_ptr = &queue_btw_Ctrl_View;
    Ctrl_Work_Queue_ptr = &Ctrl_Work_Queue;
    filesys = new filesystem(queue_btw_Ctrl_View_ptr);

    using nlohmann::json;

    std::unique_ptr<json> json_cache = std::make_unique<json>();

    // std::thread thread_Recv_fr_Net(Thread_Recv_fr_Net_begin_fun, Ctrl_Work_Queue);
    // std::thread thread_Talk_to_User(Thread_Talk_to_User_begin_fun, Ctrl_Work_Queue, queue_btw_Ctrl_View);

    threads.emplace_back(Thread_Recv_fr_Net_begin_fun);
    threads.emplace_back(Thread_Talk_to_User_begin_fun);

    bool is_logIn = false;
    // bool is_load = false;
    bool loading = false;
    bool ready = false;
    while (true)

    {
        job_in_Work_Queue *job = Ctrl_Work_Queue.pop();
        if (!is_logIn && job->mode == ANSWER_TO_CHECK_USERNAME_PASSWORD_SUCCESS || job->mode == SET_USERNAME_PASSWORD_SUCCESS)
        {
            init(job->message, *Data_On_Ctrl_ptr);
            is_logIn = 1;

            loading = true;
            char *time = nullptr;
            get_timestamp_from_file(time);
            json j = {{"time", time}, {"id", Data_On_Ctrl_ptr->user_id.c_str()}};
            char *time2 = nullptr;
            make_timestamp_to_net(time2);
            u_sk_ptr->send_message(std::move(j.dump()), ACTION_TO_DOWNLOAD_USER_DATA, time2);
            delete[] time;
            time = nullptr;
            delete[] time2;
            time2 = nullptr;
            job->free(&job);
            continue;
        }
        else if (loading && (job->mode == ACTION_TO_DOWNLOAD_USER_DATA_SUCCESS || job->mode == SQL_ERROR_IN_DOWNLOAD_USER_DATA))
        {
            if (job->mode == SQL_ERROR_IN_DOWNLOAD_USER_DATA)
            {
                queue_btw_Ctrl_View_ptr->submit(job);
                exit(EXIT_FAILURE);
            }
            else
            {
                to_down_load_history_set(*(job->message));
                queue_btw_Ctrl_View_ptr->submit(FLUSH_WINDOW, nullptr, nullptr);
            }
            job->free(&job);
            ready = true;
        }
        else if (ready)
        {
            // if ()
            // queue_btw_Ctrl_View_ptr->submit(FLUSH_WINDOW, nullptr, nullptr)
            handle_job(job);
            job->free(&job);
        }
        else
        {
            Ctrl_Work_Queue.submit(job);
            continue;
        }

        // job->free(&job);
    }
    delete filesys;
    // delete json_cache;
}
// inline std::string chatid_to_filename(std::string chatid)
// {
//     std::string result(chatid.length(), ' ');

//     srand(static_cast<unsigned int>(
//         std::stoll((chatid.length() > 18) ? chatid.substr(chatid.length() - 18, chatid.length()) : chatid))); // 初始化随机数种子

//     for (int i = 0; i < chatid.length(); i++)
//     {
//         int rand_num = rand() % 26 + 'A' + ((rand() % 26) % 2 ? ('a' - 'A') : 0); // 随机生成一个大写字母对应的 ASCII 码
//         result[i] = static_cast<char>(rand_num);                                  // 将 ASCII 码转换为字符并赋值给字符串对应位置
//     }

//     return result;
// }

/**
 * message 是 发回来的data
 * Work_Queue & Ctrl_Work_Queue 是进行 读其他的的包?
 *#include <filesystem>
 */
// #include <filesystem>
#include <boost/filesystem.hpp>
// namespace fs = boost::filesystem;

bool find_basic_file(std::string user_file_name, bool &);
bool init(std::string *message, Ctrl_Data_On_Ctrl &Data_On_Ctrl);
bool init(std::string *message, Ctrl_Data_On_Ctrl &Data_On_Ctrl)
{
    using namespace std;
    using nlohmann::json;
    json j = json::parse(*message);
    string name = j.at("name");
    // string user_file_name = name + ".json";
    bool if_set = false;
    bool a = find_basic_file(name, if_set);
    if (a && if_set) // 成功后进程的工作目录已经改变
    {
        std::fstream file("user.json");
        if (file.is_open())
            file << j.dump(4);
        else
            return false;
        file.close();
    }
    else
    {
        if (if_set)
        {
            mkdir(name.c_str(), 0755);
            chdir(name.c_str());
        }
        mkdir("chat_history", 0755);
        // mkdir("group", 0755);
        fstream file("user.json");
        fstream timestamp("timestamp.txt");
        file << j.dump(4);
        timestamp << "0001-01-01 00:00:00.000000";
        file.close();
        timestamp.close();
        return 1;
    }
    Data_On_Ctrl.lock();
    Data_On_Ctrl.user_name = j.at("name");
    Data_On_Ctrl.user_id = j.at("id");
    json friends = j.at("friends");
    if (friends.empty() == false)
        for (json &jj : j)
        {
            std::string str1 = jj.at("friend_id");
            std::string str2 = jj.at("friend_name");
            std::string str3 = jj.at("chatid");
            std::string str4 = jj.at("stat");

            Data_On_Ctrl.friends_set.emplace_back(str1, str2, str3, str4);
        }
    // else{}
    json group = j.at("groups");
    if (group.empty() == false)
    {
        // group_data tmp()
        for (json &jj : group)
        {
            std::vector<user_data> tmp;
            for (json &jjj : group.at("members"))
            {
                std::string str1 = jjj.at("id");
                std::string str3 = jjj.at("name");
                std::string str4 = jjj.at("stat");
                tmp.emplace_back(str1, str3, str4);
            }
            Data_On_Ctrl.groups_set.emplace_back(jj.at("id"), jj.at("name"), jj.at("chatid"), std::move(tmp));
        }
    }
    Data_On_Ctrl.unlock();
    return true;
}

bool find_basic_file(std::string user_name, bool &a)
{
    std::string target_dir = "./" + user_name;
    if (chdir(target_dir.c_str()) == -1)
        return false;
    a = true;
    bool found_friend_dir = false;
    // bool found_group_dir = false;
    bool found_timestamp_file = false;
    bool found_user_json_file = false;
    bool found_log_file = false;
    namespace fs = boost::filesystem;

    for (const auto &entry : fs::directory_iterator("./"))
    {
        const auto &path = entry.path();
        if (fs::is_directory(path) && path.filename() == "chat_history")
            found_friend_dir = true;
        // else if (fs::is_directory(path) && path.filename() == "group")
        // found_group_dir = true;
        else if (fs::is_regular_file(path))
        {
            const auto &filename = path.filename().string();
            if (filename == "timestamp")
                found_timestamp_file = true;
            else if (filename == "user.json")
                found_user_json_file = true;
            else if (filename == "log")
                found_log_file = true;
        }
    }
    return found_friend_dir && found_timestamp_file && found_user_json_file && found_log_file;
}

std::string &getMillisecondTimestamp(std::string &timestamp)
{
    struct timeval tv;
    gettimeofday(&tv, NULL); // 获取当前时间
    time_t nowtime = tv.tv_sec;
    struct tm *nowtm = localtime(&nowtime);
    char tmbuf[64], buf[64];

    strftime(tmbuf, sizeof(tmbuf), "%Y%m%d%H%M%S", nowtm);    // 格式化年月日时分秒
    snprintf(buf, sizeof(buf), "%s%06ld", tmbuf, tv.tv_usec); // 添加微秒，构建最终时间戳

    timestamp.clear();
    timestamp.append(buf);
    return timestamp;
}
/*时间戳切割*/
char *make_timestamp_to_net(char *timestamp)
{
    // static char timestamp[28];
    if (timestamp == nullptr)
        timestamp = new char[strlen("0000-00-00 00:00:00.000000") + 1];
    struct timeval tv;
    gettimeofday(&tv, NULL);
    time_t nowtime = tv.tv_sec;
    struct tm *nowtm = localtime(&nowtime);
    int microseconds = tv.tv_usec;
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", nowtm);
    sprintf(timestamp + strlen(timestamp), ".%06d", microseconds);
    return timestamp;
}

// 将时间戳 写到时间戳文件
int flush_timestamp(char *timestamp)
{
    int fd = open("./timestamp", O_RDWR);
    write(fd, timestamp, strlen(timestamp));
    close(fd);
    return true;
}
/**
 * 传入的哈 size 要大于 27
 *     if (timestamp == nullptr || timestamp == NULL)
        timestamp = new char[size];

*/
char *get_timestamp_from_file(char *timestamp)
{
    int size = strlen("0000-00-00 00-00-00-000000") + 1;
    if (timestamp == nullptr || timestamp == NULL)
        timestamp = new char[size];
    int fd = open("./timestamp", O_RDONLY);
    read(fd, timestamp, size);
    timestamp[size - 1] = '\0';
    close(fd);
    return timestamp;
}
// 返回是要不哟啊进行flush操作
bool handle_job(job_in_Work_Queue *job)
{
    nlohmann::json j;
    std::string id;
    switch (job->mode)
    {
    case ACTION_TO_SEND_MESSAGE_TO_USER:
        // 操作：发送消息给用户
    case ACTION_TO_SEND_MESSAGE_TO_GROUP:
        // 操作：发送消息给群组
        {
            j = nlohmann::json::parse(job->message);
            filesys->add_enter(std::move(j.at("chat_id").get<std::string>()),
                               Data_On_Ctrl_ptr->user_name,
                               std::move(j.at("data").get<std::string>()),
                               job->timestamp);
            u_sk_ptr->send_message(*job->message, job->mode, job->timestamp);
        }
        break;
    case ACTION_TO_REQUEST_MAKE_FRIEND:
        // 操作：请求添加好友
    case ACTION_TO_REQUEST_JOIN_GROUP:
        // 操作：请求加入群组
    case ACTION_TO_ANSWER_JOIN_GROUP: // 回复前 判断 该成员在不在群组内!
                                      // 操作：回复群组加入请求
    case ACTION_TO_REQUEST_RENAME_USER:
        // 操作：请求更改用户名
    case ACTION_TO_REQUEST_RENAME_GROUP:
        // 操作：请求更改群名
    case ACTION_TO_DOWNLOAD_USER_DATA:
        // 操作：下载用户数据
    case ACTION_TO_DOWNLOAD_GROUP_DATA:
        // 操作：下载群组数据
    case ACTION_TO_SEND_FILE_DATA:
        // 操作：发送文件数据
    case ACTION_TO_RECV_FILE_DATA:
        // 操作：接收文件数据
    case ACTION_TO_MANAGE_GROUP_MEMBER:
        // 操作：管理群成员
    case ACTION_TO_REMIND_USER_MEMBER:
        // 操作：提醒用户成员
    case ACTION_TO_DEAD_NOTIFICATION:
        // 操作：死亡通知
    case ACTION_TO_MAKE_GROUP:
        // 操作：创建群组
    case ACTION_TO_INIT_DATA:
        // 操作：初始化数据
        u_sk_ptr->send_message(*job->message, job->mode, job->timestamp);
        // job->free(&job);
        break;
    case ACTION_TO_ANSWER_MAKE_FRIEND: // 在 view 处理字段中 将json 填好
        // 操作：回复好友请求
        {
            j = nlohmann::json::parse(*(job->message));
            if (j.at("result").get<std::string>() == "1")
            {
                Data_On_Ctrl_ptr->lock();
                Data_On_Ctrl_ptr->friends_set.emplace_back(j.at("receiver_id").get<std::string>(),
                                                           j.at("receiver_name").get<std::string>(),
                                                           j.at("chatid").get<std::string>(),
                                                           "1");
                std::sort(Data_On_Ctrl_ptr->friends_set.begin(), Data_On_Ctrl_ptr->friends_set.end());
                Data_On_Ctrl_ptr->is_changed = true;
                queue_btw_Ctrl_View_ptr->submit(FLUSH_WINDOW, nullptr, nullptr);
                Data_On_Ctrl_ptr->unlock();
            }
            u_sk_ptr->send_message(*job->message, job->mode, job->timestamp);
        }
        break;
        /****转发包*************************************************************************************/

    case ACTION_TO_SEND_MSG_TO_USER_FALSE:
    case HAS_REMIND_IN_GRUOP:
        // 被拉hei
        {
            j = nlohmann::json::parse(*(job->message));
            filesys->add_enter(std::move(j.at(job->mode == HAS_REMIND_IN_GRUOP ? "own_id" : "chat_id").get<std::string>()),
                               Data_On_Ctrl_ptr->user_name,
                               std::move("------------(" + j.at("data").get<std::string>() + ")------------\n------------ send false 被拉黑啦 ------------ "),
                               job->timestamp);
        }
        break;
    case ACTION_TO_RECV_USER_MESSAGE:
    case ACTION_TO_RECV_GROUP_MESSAGE:
        // 收到chat信息
        {
            j = nlohmann::json::parse(*(job->message));
            std::string id(j.at("from_id").get<std::string>());
            std::string name;
            Data_On_Ctrl_ptr->lock();
            for (auto &item : Data_On_Ctrl_ptr->friends_set)
            {
                if (item.friend_id() == id)
                {
                    name = item.friend_name();
                    item.is_new_chat = true;
                }
            }

            Data_On_Ctrl_ptr->unlock();
            filesys->add_enter(std::move(j.at("chat_id").get<std::string>()),
                               name,
                               std::move(j.at("data").get<std::string>()),
                               job->timestamp,
                               1);

        } // job->free(&job);
        break;
    case NOTIFY_FRIEND_NEW_NAME:
        // 朋友改名 // 不通知
        {
            j = nlohmann::json::parse(*(job->message));
            id = j.at("id").get<std::string>();
            Data_On_Ctrl_ptr->lock();
            for (auto &a : Data_On_Ctrl_ptr->friends_set)
            {
                if (a.friend_id() == id)
                {
                    a.set_friend_name(std::move(j.at("friend_name").get<std::string>()));
                    Data_On_Ctrl_ptr->is_changed = true;
                    queue_btw_Ctrl_View_ptr->submit(DATA_UPDATE, nullptr, nullptr);
                }
            }
            Data_On_Ctrl_ptr->unlock();
        }
        break;
    // case ANSWER_TO_CHECK_USERNAME_PASSWORD:

    /*他的申请被接受或拒绝*/
    case ANSWER_TO_ANSWER_MAKE_FRIEND:
        // 好友申请的回复
        {
            j = nlohmann::json::parse(*(job->message));   // 解析json数据
            if ("0" == j.at("result").get<std::string>()) // 拒绝
                filesys->add_enter("notices",
                                   "::NOTICE::",
                                   std::move("id:>(" + j.at("receiver_id").get<std::string>() + ") refuse to be friends :data:: " + std::move(j.at("data").get<std::string>())),
                                   job->timestamp,
                                   false); //
            else                           // result is "1" agreement
            {
                Data_On_Ctrl_ptr->lock();
                // Data_On_Ctrl_ptr->friends_set.emplace_back(j.at("receiver_id").get<std::string>(),j.at("receiver_name").get<std::string>(),);
                std::string initiator_id(j.at("initiator_id").get<std::string>()); // 解析出请求者id
                // std::string result(j.at("result").get<std::string>());             // 解析出请求结果
                std::string data(j.at("data").get<std::string>()); // 解析出需要发送的消息内容
                std::string receiver_id(j.at("receiver_id").get<std::string>());
                std::string receiver_name(j.at("receiver_name").get<std::string>());
                std::string chatid = (initiator_id > receiver_id ? std::string(receiver_id + initiator_id) : std::string(initiator_id + receiver_id));
                // filesys->add_enter(std::move(initiator_id > receiver_id ? std::string(receiver_id + initiator_id) : std::string(initiator_id + receiver_id)),
                // );
                Data_On_Ctrl_ptr->friends_set.emplace_back(receiver_id, receiver_name, chatid, "1");
                Data_On_Ctrl_ptr->is_changed = true;
                ++Data_On_Ctrl_ptr->notices_count;
                Data_On_Ctrl_ptr->unlock();
                if (filesys->add_enter("notices", "::NOTICE::", receiver_name + "accpeted to be friend", job->timestamp, 1) == true)
                {
                    queue_btw_Ctrl_View_ptr->submit(FLUSH_WINDOW, nullptr, nullptr);
                }
            }
        }
        break;
    case RECV_MAKE_FRIEND_REQUEST:
        // 收到好友申请
        {
            j = nlohmann::json::parse(*(job->message)); // 解析json数据
            {
                Data_On_Ctrl_ptr->lock();
                ++Data_On_Ctrl_ptr->notices_count;
                Data_On_Ctrl_ptr->unlock();
            }
            filesys->add_enter("notices", "::NOTICES::", j.at("initiator_name").get<std::string>() + "(" + j.at("initiator_id").get<std::string>() + ")  request to be friends DATA:: " + j.at("data").get<std::string>(), job->timestamp, 1);
        }
        break;

    case RECV_REQUEST_TO_ANSWER_JOIN_GROUP:
        // 收到加群申请
        {
            j = nlohmann::json::parse(*(job->message)); // 解析json数据
            {
                Data_On_Ctrl_ptr->lock();
                ++Data_On_Ctrl_ptr->notices_count;
                Data_On_Ctrl_ptr->unlock();
            }
            filesys->add_enter("notices", "::NOTICES::", j.at("name").get<std::string>() + "(" + j.at("id").get<std::string>() + ")  request to join your group(" + j.at("group_name").get<std::string>() + ") DATA:: " + j.at("data").get<std::string>(), job->timestamp, 1);
        }
        break;

    case NEW_MEMBER_JOIN_GROUP:
        // 新成员加入 / 加入失败通知
        {
            j = nlohmann::json::parse(*job->message); //
            if (j.at("result").get<std::string>() != "1")
                break;
            // 加入

            Data_On_Ctrl_ptr->lock();
            for (auto &i : Data_On_Ctrl_ptr->groups_set)
            {
                if (i.group_id() == j["group_id"].get<std::string>())
                {
                    i.group_members.emplace_back(j["id"].get<std::string>(), j["name"].get<std::string>());
                    Data_On_Ctrl_ptr->is_changed = true;
                    filesys->add_enter("notices", "::NOTICE::", "new member join group[" + j["group_name"].get<std::string>() + "]  " + j["name"].get<std::string>() + "(" + j["id"].get<std::string>() + ")", job->timestamp, false);
                    ++Data_On_Ctrl_ptr->notices_count;
                    queue_btw_Ctrl_View_ptr->submit(DATA_UPDATE, nullptr, nullptr);
                    break;
                }
            }
            Data_On_Ctrl_ptr->unlock();
        }
        break;
    case RECV_ANSWER_JOIN_GROUP:
        break;
    case ANSWER_TO_ANSWER_JOIN_GROUP:
        break;
    case FRIEND_SIGN_OUT:
        break;
    case ANSWER_TO_REQUEST_JOIN_GROUP:
        break;
    case NOTIFY_GROUP_NEW_NAME:
        break;

    case ACTION_TO_SEND_MSG_TO_USER_SUCCESS:
    case ACTION_TO_SEND_MSG_TO_GROUP_SUCCESS:
    case REQUEST_MAKE_FRIEND_SUCCESS:
    case ANSWER_MAKE_FRIEND_REQUEST_SUCCESS:
        GLOG_INFO(*job->message, 0);
        break;
    default:

        if (job->mode > 100)
            if (job->mode % 100 == 99)
            {
                GLOG_ERROR(*job->message, 1);
                std::string tmp(" server SQL dead ");
                queue_btw_Ctrl_View_ptr->submit(99, &tmp, job->timestamp);
            }
            else
                ;
        else
        {
            using namespace std;
            cout << " unknown mode :> " << job->mode << std::endl;
            assert(0);
        }
        break;
    }
}
int to_down_load_history_set(std::string &message)
{
    using nlohmann::json;
    Data_On_Ctrl_ptr->lock();
    json j = json::parse(message);
    json his_friend = j.at("user_history");
    for (auto &jj : his_friend.items())
    {
        std::string user_id = jj.key();
        std::string chatid("");
        std::string user_name("");
        for (auto &a : Data_On_Ctrl_ptr->friends_set)
        {
            if (a.friend_id() == user_id)
            {
                chatid = a.friend_chatid();
                user_name = a.friend_name();
                a.is_new_chat = true;
                break;
            }
        }
        if (chatid.empty())
            continue;
        for (auto &i : jj.value())
        {

            std::string time = i.at("time").get<std::string>();
            filesys->add_enter(chatid, user_name, std::move(i.at("data").get<std::string>()), time.c_str(), false);
        }
    }
    // json set = j.at("set");
    json his_group = j.at("group_history");
    for (auto &jj : his_group.items())
    {
        std::string group_id = jj.key();
        std::string chatid("");
        std::string group_name(""); //////////////

        //////////////////////////////////////
        for (auto &a : Data_On_Ctrl_ptr->groups_set)
        {
            if (a.group_id() == group_id)
            {
                chatid = a.group_chatid();
                group_name = a.group_name();
                a.is_new_chat = true;
                break;
            }
        }
        if (chatid.empty())
            continue;
        for (auto &i : jj.value())
        {
            std::string time = i.at("time").get<std::string>();
            filesys->add_enter(chatid, group_name, std::move(i.at("data").get<std::string>()), time.c_str(), false);
        }
    }

    json setchange = j.at("set_change");
    for (auto &a : setchange)
    {
        std::string time = a.at("time").get<std::string>();
        char *timestamp = new char[time.length() + 1];
        strcpy(timestamp, time.c_str());

        Ctrl_Work_Queue_ptr->submit(static_cast<size_t>(std::stoi(a.at("mode").get<std::string>())), new std::string(a.at("data").get<std::string>()), timestamp);
    }
    // Data_On_Ctrl_ptr->is_changed = true;
    Data_On_Ctrl_ptr->unlock();
}