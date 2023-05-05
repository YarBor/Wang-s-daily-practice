#include "server.hh"
// #include <mysql/mysql.h>
// #include "MysqlClass.hh"
//---  mysql_num_fields(result) 返回结果集中字段的数量，即每一行的列数。
//---  mysql_num_rows(result) 返回结果集中的行数

/*timestamp 是   '0000-00-00 00:00:00.000000'  */
#define COMMAND(string, ...) \
    do                       \
    {                        \
        std::stringstream i; \
        i << __VA_ARGS__;    \
        string = i.str();    \
                             \
    } while (0)

#define MYSQL_FAL_AND_RESET(message_point, mode)                           \
    do                                                                     \
    {                                                                      \
        *message_point = " {\"error\":\"chat_data check in SQL failed\"}"; \
        point->send_message(std::ref(*message_point), mode, timestamp);    \
        mysql->reset();                                                    \
        return false;                                                      \
    } while (0)
// extern std::map<std::string, class user> *users; // 用户列表 <id,user>

using nlohmann::json;

std::string find_user_name(std::string id, MYSQL *mysql);
std::string find_group_name(std::string id, MYSQL *mysql);

/**
 * test_done
 * check 成功 则
 * 在 server 初始化 user
 *
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 */
int check_username_password(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
// 解析 JSON 格式的字符串
//{
//    "id":"12345",
//    "password":"passwords_data"
//}
// return
//{
//    "name":"username"
//}
{
    try
    {
        json j = json::parse((*message));        // 解析字符串为 JSON 对象
        std::string id_str = j.at("id");         // 获取 JSON 中的 id 字段值
        std::string password = j.at("password"); // 获取 JSON 中的 password 字段值

        std::string command("select name, password from user where id = ");
        command += mysql->to_escape_text(id_str);
        if (0 != mysql->do_(command)) // 执行 // 若出错
        {
            std::cout << mysql_errno(mysql->mysql) << std::endl;
            *message = "{\"error\":\"mysql return fail , return message != 1 , IN check_username_password\"}"; // 构造错误信息并发送到客户端
            point->send_message(*message, SQL_ERROE_IN_CHECK, timestamp);

            GLOG_WARNING("SQL ERROR _ command:> " << command, true);

            mysql->free_result();
            return false; // 返回错误
        }

        MYSQL_ROW row;
        MYSQL_RES *result = mysql->get_result(); // 从 MySQL 返回的结果中获取数据

        if (mysql_num_rows(result) > 1) // 检查查询结果是否符合预期 // 不会 id 是主建
        {
            *message = "{\"error\":\"mysql return fail , return message != 1 , IN check_username_password\"}"; // 构造错误信息并发送到客户端
            point->send_message(*message, ACTION_TO_ERROR, timestamp);

            GLOG_WARNING("SQL RETURN too much _ command:> " << command, true);

            mysql->free_result();
            return false; // 返回错误
        }
        else if (mysql_num_rows(result) == 0) // not found
        {
            *message = "{\"false\":\"not find user\"}";
            point->send_message(*message, NOT_FIND_IN_CHECK, timestamp);

            mysql->free_result();
            return false; // 返回错误
        }

        row = mysql->get_row();

        if (strcmp(row[1], password.c_str()) != 0) // 检查输入的密码是否正确 // 不成功
        {
            *message = "{\"false\":\"wrong password\"}"; // 发送密码错误信息
            point->send_message(*message, WRONG_PASSWORD, timestamp);
            mysql->free_result();
            return false; // 返回错误
        }
        else // 如果用户名和密码都能够匹配成功，表示登录信息验证通过
        {
            std::string name(row[0]);                  // 获取用户名
            user *tmp = new user(point, name, id_str); // 利用用户名和 id 创建一个 user 对象 !!!!!!!!/// new 的 !!!
            // 优化 user 的 构造 string 这里是值拷贝

            GLOG_INFO("user" << name << " " << id_str << "login", false);

            // 在全局的用户列表中 添加unfriend id (包括 user 和 group)
            tmp->is_alive = true;
            // std::string command("select id from user where id = " + id_str + " and stat <> 1");
            std::string command("select friend from friend_relationship where own = " + id_str + " and stat <> 1");

            mysql->reset();
            mysql->do_(command);

            // 向user的类中的set 里加入 是好友 但 无法通信的id
            char **a = NULL; // 存 临时结果集（一行的）指针
            while ((a = mysql->get_rows_from_result()) != NULL)
                tmp->unfriend.insert(a[0]);
            mysql->reset();

            // 无发通信的 group_id

            COMMAND(command, "select id from chatroommember where id = " << mysql->to_escape_text(id_str) << " and mode = 0 ");
            // COMMAND(command, "select id from chatroommember where member_id = " << mysql->to_escape_text(id_str) << " and mode <> 1 ");
            mysql->do_(command);
            while ((a = mysql->get_rows_from_result()) != NULL)
                tmp->unfriend.insert(a[0]);
            mysql->reset();

            // 向全局的在线列表添加用户信息
            My_user_list.insert(id_str, tmp);
            point->owner = tmp;

            // 构造登录成功的消息
            // *message = "{\"name\": \"" + name + "\"}";
            json jtmp = json::object();
            jtmp["id"] = id_str;
            jtmp["name"] = name;
            std::string command1;
            COMMAND(command1, "select user.name, friend_relationship.friend ,friend_relationship.stat,friend_relationship.chatid from friend_relationship,user where own = " << id_str << " and friend = user.id order by name");
            mysql->reset();
            if (mysql->do_(command1))
            {
                // std::cout << mysql_errno(mysql->mysql) << std::endl;
                *message = "{\"error\":\"mysql return fail , in get user data\"}"; // 构造错误信息并发送到客户端
                point->send_message(*message, SQL_ERROE_IN_CHECK, timestamp);
                GLOG_WARNING("SQL ERROR _ command:> " << command, true);
            }
            char **line = NULL;
            json arr = json::array();
            while ((line = mysql->get_rows_from_result()))
            {
                json j = {
                    {"friend_id", line[1]},
                    {"friend_name", line[0]},
                    {"stat", line[2]},
                    {"chatid", line[3]},
                };
                arr.push_back(j);
            }

            jtmp["friends"] = arr;

            arr.clear();
            mysql->reset();

            COMMAND(command1, "select chatroom.id, chatroom.name, chatroom.owner_id, chatroom.own_name ,chatroom.chatid from chatroom where chatroom.id in (select id from chatroommember where chatroommember.member_id = " << id_str << ")");
            MYSQL *sql = mysql->mysql;
            // sql.
            mysql_query(sql, command1.c_str());
            auto result = mysql_store_result(sql);
            MYSQL_ROW row;

            json chatrooms = json::array();
            while ((row = mysql_fetch_row(result)))
            {
                json chatroom = {{"id", row[0]},
                                 {"name", row[1]},
                                 {"owner_id", row[2]},
                                 {"own_name", row[3]},
                                 {"chatid", row[4]}};
                std::string command2;
                COMMAND(command2, "select mode , member_id , member_name from chatroommember where id = '" << row[0] << "' order by mode , member_name");
                mysql_query(sql, command2.c_str());
                auto result2 = mysql_store_result(sql);
                MYSQL_ROW row2;
                json members = json::array();
                while (row2 = mysql_fetch_row(result2))
                {
                    json j = {{"id", row2[1]}, {"stat", row2[0]}, {"name", row2[2]}};
                    members.push_back(j);
                }

                chatroom["members"] = members;
                mysql_free_result(result2);
                chatrooms.push_back(chatroom);
            }
            mysql_free_result(result);

            jtmp["groups"] = chatrooms;

            *message = jtmp.dump();
            point->send_message(*message, ANSWER_TO_CHECK_USERNAME_PASSWORD_SUCCESS, timestamp);

            // 清空sql链接
            mysql->reset();
        }
        // notify user online

        return true; // 返回验证通过标志 true
    }
    catch (const std::exception &ex) // 捕获 JSON 解析异常
    {
        std::cerr << "Error parsing JSON: " << ex.what() << std::endl;
        GLOG_ERROR(message << "  JSON PRASE FAIL!", true);
        *message = "{\"error\":\"message prase false\"}"; // 构造错误信息并发送到客户端
        point->send_message(*message, ACTION_TO_ERROR, timestamp);
        mysql->reset();
        return false; // 返回解析失败标志
    }
    // 最后再次返回 true，这里可以考虑将这一句删去
    return true;
}
int set_username_password(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
// message 是 json --> 需要处理的字段
// {
//     "name":"name",
//     "password":"passwords_data"
// }
// point 指向 发来请求的 client
// mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
// return
// {
//    "id":"12345"
// }
{
    try
    {
        // 解析 JSON 字符串，提取必需的用户名和密码字段
        json j = json::parse(*message);
        std::string name = j.at("name");
        std::string password = j.at("password");

        // 构造 SQL 查询语句，并使用 MYSQL_CLIENT 的 escape 函数来防止 SQL 注入攻击
        std::string command("insert into user (name , password) values( '" + std::string(mysql->to_escape_text(name)) + "', '" + std::string(mysql->to_escape_text(password)) + "')");

        // 执行 SQL 查询，如果失败则发送错误消息给客户端
        if (mysql->do_(command) != 0)
        {
            GLOG_WARNING("SQL ERROR _ command:> " << command, true);
            *message = "{\"error\":\"set user failed to sql , may sql error\"}";
            point->send_message(std::ref(*message), SQL_ERROR_IN_SET, timestamp);
            mysql->reset();
            return false;
        }
        else
        {
            // 如果执行成功，则向客户端返回用户 ID
            *message += "{\"id\" : \"";
            *message += mysql->get_last_insert_id();
            *message += "\"}";
            point->send_message(std::ref(*message), SET_USERNAME_PASSWORD_SUCCESS, timestamp);
            mysql->reset();
            return true;
        }
    }
    catch (const std::exception &ex)
    {
        // 如果出错，则发送错误消息给客户端，并记录日志
        std::cerr << "Error parsing JSON: " << ex.what() << std::endl;
        GLOG_ERROR(message << "  JSON PRASE FAIL!", true);
        *message = "{\"error\":\"Error parsing JSON::prase in set user\"}";
        point->send_message(std::ref(*message), ACTION_TO_ERROR, timestamp);
        std::cout << message << std::endl;
        mysql->reset();
        return false;
    }
}
int send_message_to_user(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    // message 是 json --> 需要处理的字段
    // point 指向 发来请求的 client
    // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
    //{
    //    "from_id": "22432",
    //    "chat_id": "12345",
    //    "data" : "Hello world"
    //    "target_id": "12345"
    //}
     // 关于时间 发出者 自行记录时间 server 存塞如数据库的时间 \
接收者 在线接收则自行记录 从数据库下载则得到 加入数据库时间
    //
    json j = json::parse(*message);
    std::string chat_id = j.at("chat_id");
    std::string data = j.at("data");
    std::string target_id = j.at("target_id");
    user *target = NULL;

    // 是否拉黑？
    bool a;
    {
        std::unique_lock<std::mutex> lock(point->owner->unfriend_set_lock);
        a = point->owner->unfriend.find(target_id) != point->owner->unfriend.end();
    }
    if (a)
    {
        std::string tmp_message = "{\"false\":\"被拉黑\" , \"message\" : \"" + data + "\" , \"chat_id\":\"" + chat_id + "\"}";
        // std::string tmp_message = "{\"false\":\"被拉黑\"}";
        point->send_message(std::ref(tmp_message), ACTION_TO_SEND_MSG_TO_USER_FALSE, timestamp);
    }
    // 是否在线？
    else if ((target = My_user_list.search(target_id)))
    {
        target->client->send_message(std::ref(*message), ACTION_TO_RECV_USER_MESSAGE, timestamp);
    }
    mysql->reset();

    std::string command("insert into chathistory( chatid , message , messagetime ) values(");
    command += chat_id;
    command += ", '";
    command += mysql->to_escape_text(data); // char *
    command += "','";
    command += timestamp;
    command += "')";

    if (mysql->do_(command) != 0) // 把 data 塞到 数据库中 // 不成功
    {
        GLOG_WARNING("SQL ERROR _ command:> " << command, true);
        *message = "{\"error\":\"message set in sql failed\"}";
        point->send_message(std::ref(*message), SQL_ERROE_IN_SEND_MSG_TO_FRIEND, timestamp);
        mysql->reset();
        return false;
    }
    mysql->reset();

    json return_data = {{"result", "done"}, {"message", data.c_str()}};
    std::string tmp_message = return_data.dump();
    point->send_message(std::ref(tmp_message), ACTION_TO_SEND_MSG_TO_USER_SUCCESS, timestamp);
    return true;
}
/**
 * @brief 用于将消息发送到群组中的所有成员
 * @param message: json格式的需要处理的消息字段
 * @param point: 指向发来请求的client对象的指针
 * @param mysql: 每个线程不同的与MYSQL客户端链接的对象
 * @return 发送成功则返回true，否则返回false并向client发送错误信息
 *
 * {
 *     "chat_id":"12345"
 *     "group_id":"12345",
 *     "own_id":"12345",
 *
 *     "own_name":"wang",
 *     "data":"hello world",
 *     "time":"%Y-%m-%d %H:%M:%S.6"
 * }
 * select id from chatroommember where id = group_id and id <> own_id
 * 拿所有member_id 然后在user_list里找有没有 有 用id通信 没有 continue
 * insert into chathistory value ('chat_id','data', "2022-02-11 15:15:15");
 * 进行插入
 */
int send_message_to_group(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{

    // 解析json字段
    json j = json::parse(*message);
    std::string chat_id = j.at("chat_id");   // 聊天室id
    std::string group_id = j.at("group_id"); // 群组id
    std::string own_name = j.at("own_name"); // 发送者姓名
    std::string own_id = j.at("own_id");     // 发送者id
    std::string time = j.at("time");         // 发送时间
    std::string data = j.at("data");         // 发送的消息内容

    // 查有没有被群组拉黑
    bool a;
    {
        user *tmp_user_cl = My_user_list.search(own_id);

        std::unique_lock<std::mutex> lock(tmp_user_cl->unfriend_set_lock);
        a = tmp_user_cl->unfriend.find(group_id) != tmp_user_cl->unfriend.end();
    }
    if (a)
    {
        std::string tmp_message = "{\"chat_id\":\"" + chat_id + "\" \"data\" : \"" + data + "\" }";
        point->send_message(std::ref(tmp_message), HAS_REMIND_IN_GRUOP, timestamp);
        return false;
    };

    // 在chatroommember表中查询除发送者外的所有群组成员的id
    std::string command = "select id from chatroommember where id = ";
    command += mysql->to_escape_text(group_id);
    command += " and mode > 0 and member_id <> ";
    command += mysql->to_escape_text(own_id);
    if (mysql->do_(command)) // 执行查询失败，向client发送错误信息
    {
        GLOG_WARNING("SQL search (ID) FROM (chatroommber) failed __ COMMAND:>" << command, true);
        *message = "{\"error\":\"message search(friends ID) in sql failed\"}";
        point->send_message(std::ref(*message), SQL_ERROE_IN_SEND_MSG_TO_GROUP, timestamp);
        mysql->reset();
        return false;
    }

    char **row;
    user *tmp;
    // 遍历所有群组成员的id，如果在线用户列表存在该成员，则向其发送消息
    while ((row = mysql->get_rows_from_result()))
    {
        if ((tmp = My_user_list.search(std::string(row[0]))) != nullptr)
            tmp->client->send_message(std::ref(*message), ACTION_TO_RECV_GROUP_MESSAGE, timestamp);
    }
    mysql->reset();

    // 将聊天数据插入chathistory表中
    command = "insert into chathistory (chatid,message,messagetime) values( '";
    command += mysql->to_escape_text(chat_id);
    command += "','";
    command += mysql->to_escape_text(data);
    command += "','";
    command += timestamp;
    command += "')";
    if (mysql->do_(command)) // 执行插入失败，向client发送错误信息
    {
        GLOG_WARNING("chat_data set in SQL failed __ COMMAND:>" << command, true);
        *message = "{\"error\":\"chat_data set in SQL failed\"}";
        point->send_message(std::ref(*message), SQL_ERROE_IN_SEND_MSG_TO_GROUP, timestamp);
        mysql->reset();
        return false;
    }
    mysql->reset();

    // return ACTION_TO_SEND_MSG_TO_USER_SUCCESS
    json return_data = {{"result", "done"}, {"message", data.c_str()}};
    std::string tmp_message = return_data.dump();
    point->send_message(std::ref(tmp_message), ACTION_TO_SEND_MSG_TO_GROUP_SUCCESS, timestamp);
    return true;
}
/**
 * @brief 将json数据解析后，找到在线用户列表中的指定接收者，并向其发送消息
 * @param message: json格式的需要处理的消息字段
 * @param mysql: 每个线程不同的与MYSQL客户端链接的对象
 * @param My_user_list: 存储在线用户信息的容器
 * @param point: 指向发来请求的client对象的指针
 * @return 如果插入成功，则返回true；如果插入失败则向client发送错误信息，并返回false
 * json -->
 * {
 *    "initiator_id": "12345",
 *    "initiator_name": "name",
 *    "receiver_id" : "12344" ,
 *    "receiver_name" : "name" ,
 *
 *     "data" : "data  data  data"
 * }
 * ACTION_TO_REQUEST_MAKE_FRIEND
 */
int request_make_friend(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{

    json j = json::parse(*message);                // 解析json数据
    std::string receiver_id = j.at("receiver_id"); // 解析出接收者id
    std::string data = j.at("data");               // 解析出需要发送的消息内容

    user *tmp = My_user_list.search(receiver_id); // 在在线用户列表中查找是否存在该接收者
    if (tmp != nullptr)
    { // 如果存在，则向其发送消息
        tmp->client->send_message(std::ref(*message), RECV_MAKE_FRIEND_REQUEST, timestamp);
    }

    // 将聊天数据插入User_set_change表中
    std::string command("INSERT INTO User_set_change (id, mode, data , time ) VALUES (");
    command += mysql->to_escape_text(receiver_id);
    command += "," + std::to_string(RECV_MAKE_FRIEND_REQUEST) + ",'";
    command += mysql->to_escape_text(data);
    command += "','";
    command += timestamp;
    command += "')";

    if (mysql->do_(command) != 0) // 执行插入失败，向client发送错误信息
    {
        GLOG_WARNING("chat_data set in SQL failed __ COMMAND:>" << command, true);
        *message = "{\"error\":\"chat_data set in SQL failed\"}";
        point->send_message(std::ref(*message), SQL_ERROR_IN_REQUEST_MAKE_FRIEND, timestamp);
        mysql->reset();
        return false;
    }
    mysql->reset();
    // 执行插入成功，返回true
    json tmp_j = {{"mode", "done"}, {"id", receiver_id.c_str()}};
    std::string message_tmp = tmp_j.dump();
    point->send_message(message_tmp, REQUEST_MAKE_FRIEND_SUCCESS, timestamp);
    return true;
}
/**
 * @brief 将json数据解析后，找到在线用户列表中的指定请求者，并向其发送消息
 *        同时将该消息插入到mysql数据库中以备下载
 * @param message: json格式的需要处理的消息字段
 * @param mysql: 每个线程不同的与MYSQL客户端链接的对象
 * @param My_user_list: 存储在线用户信息的容器
 * @param point: 指向发来请求的client对象的指针
 * @return 如果插入成功，则返回true；如果插入失败则向client发送错误信息，并返回false
 * json -->
 *  "data" : "Hello world",
 *  "result" : "0" / "1" ,
 *  "initiator_id": "12345",
 *  "initiator_name": "name",
 *  "receiver_id" : "12344" ,
 *  "receiver_name" : "name" ,
 *
 *
 *  * INSERT ignore INTO friend_relationship (own, friend, chatid, lastchattime)
 * SELECT a.id AS own, b.id AS friend, '12345' AS chatid, NOW() AS lastchattime
 * FROM user AS a, user AS b
 * WHERE (a.id = 10023 AND b.id = 10024) OR (b.id = 10023 AND a.id = 10024);
 *
 * chatid = (initiator_id > receiver_id ? std::string(receiver_id + initiator_id) : std::string(initiator_id + receiver_id))
 * ACTION_TO_ANSWER_MAKE_FRIEND
 */
int answer_make_friend(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    json j = json::parse(*message);                  // 解析json数据
    std::string initiator_id = j.at("initiator_id"); // 解析出请求者id
    std::string result = j.at("result");             // 解析出请求结果
    std::string data = j.at("data");                 // 解析出需要发送的消息内容
    std::string receiver_id = j.at("receiver_id");   //

    user *tmp = My_user_list.search(initiator_id); // 在在线用户列表中查找是否存在该请求者
    if (tmp != nullptr)
    { // 如果存在，则向其发送消息
        tmp->client->send_message(*message, ANSWER_TO_ANSWER_MAKE_FRIEND, timestamp);
    }

    // 将数据插入User_set_change表中
    std::string command(std::string("INSERT INTO User_set_change (id, mode, data , time) VALUES (") +
                        mysql->to_escape_text(initiator_id) +                                        // 插入请求者id
                        "," + std::to_string(ANSWER_TO_ANSWER_MAKE_FRIEND) + ",'" +                  // 插入请求者操作码
                        mysql->to_escape_text(std::ref(*message)) + "','" + std::string(timestamp) + // 插入请求信息
                        "')");

    if (mysql->do_(command) != 0) // 执行插入失败，向client发送错误信息
    {
        GLOG_WARNING("chat_data set in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        *message = "{\"false\":\"chat_data set in SQL failed\"}";
        point->send_message(std::ref(*message), SQL_ERROR_IN_ANSWER_MAKE_FRIEND, timestamp);
        mysql->reset();
        return false;
    }

    mysql->reset();
    // 插入好友关系
    if (result == "1")
    {
        command = std::string("INSERT ignore INTO friend_relationship (own, friend, chatid, lastchattime) SELECT a.id AS own, b.id AS friend, '") +
                  (initiator_id > receiver_id ? std::string(receiver_id + initiator_id) : std::string(initiator_id + receiver_id)) +
                  "' AS chatid, NOW() AS lastchattime FROM user AS a,user AS b  WHERE(a.id = " +
                  mysql->to_escape_text(receiver_id) + " AND b.id = " +
                  mysql->to_escape_text(initiator_id) + ") OR(b.id = " +
                  mysql->to_escape_text(receiver_id) + " AND a.id = " +
                  mysql->to_escape_text(initiator_id) + ")";

        // 插入成功，则返回true
        /*
        INSERT ignore INTO friend_relationship (own, friend, chatid, lastchattime)
        SELECT a.id AS own, b.id AS friend, '1000310019' AS chatid, NOW() AS lastchattime
        FROM user AS a,user AS b
        WHERE(a.id = 10019 AND b.id = 10003) OR(b.id = 10019 AND a.id = 10003)
        */
        mysql->reset();
        if (mysql->do_(command))
        {
            GLOG_WARNING("chat_data set in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
            *message = "{\"false\":\"chat_data set in SQL failed\"}";
            point->send_message(std::ref(*message), SQL_ERROR_IN_ANSWER_MAKE_FRIEND, timestamp);
            mysql->reset();
            return false;
        }
        mysql->reset();
        // mysql
    }

    json tmp_j = {{"mode", "done"}, {"id", initiator_id.c_str()}};
    std::string message_tmp = tmp_j.dump();
    point->send_message(message_tmp, ANSWER_MAKE_FRIEND_REQUEST_SUCCESS, timestamp);
    return true;

    return true;
}
/**
 * message 是 json --> 需要处理的字段
 * point 指向 发来请求的 client
 * mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * json-->
 * {
 *   "group_id" : "12345",
 *   "group_name" : "group name",
 *   "name" : "name",
 *   "id" : "12345",
 *  "description" : "description_data",
 * }
 *
 *
 *ACTION_TO_ANSWER_JOIN_GROUP
 *
 *
 */
int request_join_group(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    json j = json::parse(*message);          // 解析JSON
    std::string group_id = j.at("group_id"); // 解析group_id

    std::string group_name = find_group_name(group_id, mysql->mysql);
    j["group_name"] = group_name;
    std::string id = j.at("id");                                                                                                              // 解析group_name
    std::string command;                                                                                                                      // SQL查询命令字符串变量
    COMMAND(command, "select member_id from chatroommember where id = " << mysql->to_escape_text(group_id) << " and mode > 1 order by mode"); // SQL查询命令字符串
    if (mysql->do_(command) != 0)                                                                                                             // 查询失败
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_REQUEST_JOIN_GROUP);
    }
    char **row = NULL;
    bool flag = false;                            // 从结果集中获取行
    while ((row = mysql->get_rows_from_result())) // 如果有结果--找到群主/ mamager id
    {
        flag = true;
        std::string GROUP_owner_id(row[0]);                                 // 获取群主ID
        user *group_owner;                                                  // 定义user指针变量，用于判定群主是否在线
        if ((group_owner = My_user_list.search(GROUP_owner_id)) != nullptr) // 如果群主在线
            if (group_owner->client->send_message(j.dump(), RECV_REQUEST_TO_ANSWER_JOIN_GROUP, timestamp) != true)
            {
                GLOG_WARNING("send_message failed" << j.dump(), 1);
            }
    }
    if (flag) // 找到群主
    {
        COMMAND(command, "insert into User_set_change(id,mode,data,time) values((select member_id from chatroommember where id = " << group_id << " and mode > 1) , " << std::to_string(ACTION_TO_ANSWER_JOIN_GROUP) << " , ' " << mysql->to_escape_text(*message) << "','" << timestamp << "')");
        // COMMAND(command, "INSERT into User_set_change(id,mode,data) values(" << GROUP_owner_id << "," << std::to_string(ACTION_TO_ANSWER_JOIN_GROUP) << ",'" << mysql->to_escape_text(*message) << "'"); // SQL插入命令字符串
        mysql->reset();
        if (mysql->do_(command) != 0)
        {
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
            MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_REQUEST_JOIN_GROUP);
        }
        json tmp_j = {{"mode", "done"}, {"group_id", group_id.c_str()}, {"target_id", id.c_str()}};
        // json tmp_j = {{"mode", "done"}, {"group_id", group_id.c_str()}};
        // std::string message_tmp = ;
        point->send_message(tmp_j.dump(), REQUEST_JOIN_GROUP_SUCCESS, timestamp);
        return true;
    }
    else // 没找见 群主...
    {
        mysql->reset(); // 重置mysql状态
        *message = "{\"false\":\"group not found\"}";
        point->send_message(std::ref(*message), NOT_FIND_IN_JOIN_GROUP, timestamp);
    }
    return true;
}

/**
 * 处理用户申请加入群组的请求
 *
 * @param message 要处理的JSON数据字段
 * @param point 发送请求的客户端指针
 * @param mysql 线程本地的MySQL客户端链接字段
 * @param json_data_IN
 * {
 *     "group_id" : "12345",
 *     "group_name" : "群组名称",
 *     "name" : "用户名称",
 *     "id" : "12345", // 申请者ID
 *     "description" : "群组描述",
 *     "result" : "0"/"1" , // 请求结果：0代表拒绝，1代表同意
 *     "group_chat_id" : "123"
 *
 *      "manager_id" : "1243",   ?
 * "manager_name" : "xzvc,mn"   ?
 * }
 * @return true 表示处理成功
 *
 * ACTION_TO_ANSWER_JOIN_GROUP
 */
int answer_join_group(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    json j = json::parse(*message);
    std::string id = j.at("id");
    std::string name = j.at("name");
    std::string group_id = j.at("group_id");
    std::string group_name = find_group_name(group_id, mysql->mysql);
    j["group_name"] = group_name;
    std::string result = j.at("result");

    std::string command;
    user *tmp = My_user_list.search(id);
    if (tmp)
        tmp->client->send_message(*message, ANSWER_TO_REQUEST_JOIN_GROUP, timestamp);
    if (result == "1") // 同意加入群组：向chatroommember表中添加成员
    {
        COMMAND(command, "insert into chatroommember(id,member_id,member_name) value (" << group_id << "," << id << ", '" << mysql->to_escape_text(name) << "')");
        if (mysql->do_(command) != 0)
        {
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
            MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_ANSWER_JOIN_GROUP);
        }
        mysql->reset();
        // COMMAND(command, );
    }

    // COMMAND(command, "INSERT into User_set_change(id,mode,data,time) values(" << id << "," << std::to_string(ACTION_TO_ANSWER_JOIN_GROUP) << ",'" << mysql->to_escape_text(*message) << "','" << timestamp << "')"); // SQL插入命令字符串
    // if (mysql->do_(command))
    // {
    //     GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
    //     MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_ANSWER_JOIN_GROUP);
    // }
    user *target_ptr;
    if ((target_ptr = My_user_list.search(id)) != nullptr) // 如果s申请者在线
        if (target_ptr->client->send_message(j.dump(), RECV_ANSWER_JOIN_GROUP, timestamp) != true)
        {
            GLOG_WARNING("send_message failed" << *message, 1);
        }

    COMMAND(command, "select member_id from chatroommember where id = " << mysql->to_escape_text(group_id) << " and mode >= 1 order by mode"); // SQL查询命令字符串
    if (mysql->do_(command) != 0)                                                                                                              // 查询失败
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_ANSWER_JOIN_GROUP);
    }
    char **row = NULL;
    bool flag = false;                            // 从结果集中获取行
    while ((row = mysql->get_rows_from_result())) // 拿到所有成员id
    {
        flag = true;
        std::string GROUP_member_id(row[0]);                                 // 获取ID
        user *group_owner;                                                   // 定义user指针变量，用于判定群主是否在线
        if ((group_owner = My_user_list.search(GROUP_member_id)) != nullptr) // 如果成员在线
            if (group_owner->client->send_message(j.dump(), NEW_MEMBER_JOIN_GROUP, timestamp) != true)
            {
                GLOG_WARNING("send_message failed" << *message, 1);
            }
    }
    if (flag) // 找到群主
    {
        COMMAND(command, "insert into User_set_change(id,mode,data,time) values((select member_id from chatroommember where id = " << group_id << " and mode >= 1) , " << std::to_string(NEW_MEMBER_JOIN_GROUP) << " , '" << mysql->to_escape_text(*message) << "','" << timestamp << "')");
        // COMMAND(command, "INSERT into User_set_change(id,mode,data) values(" << GROUP_member_id << "," << std::to_string(ACTION_TO_ANSWER_JOIN_GROUP) << ",'" << mysql->to_escape_text(*message) << "'"); // SQL插入命令字符串
        mysql->reset();
        if (mysql->do_(command) != 0)
        {
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
            MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_ANSWER_JOIN_GROUP);
        }
        json tmp_j = {{"mode", "done"}, {"group_id", group_id.c_str()}, {"target_id", id.c_str()}};
        std::string message_tmp = tmp_j.dump();
        point->send_message(message_tmp, ANSWER_JOIN_GROUP_SUCCESS, timestamp);
        return true;
    }
    mysql->reset();
    return true;
}

/**
 *  没写完 在rename后还要更新所有的表  chatroom.own_name chatroommember.member_name
 *
 * @brief 处理更改用户名请求
 * @param message json格式的请求消息字符串，需要处理的字段包括 "id"：用户ID，"new_name"：新用户名
 * @param point 指向发出请求的客户端
 * @param mysql 每个线程链接不同MYSQL客户端的字段
 * @return int 请求处理结果，成功返回true，否则返回false
 * {"id":"id", "new_name":"new_name}
 */
int request_rename_user(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    json j = json::parse(*message);
    std::string new_name = j.at("new_name"); // 获取新用户名
    std::string id = j.at("id");             // 获取用户ID
    std::string command;

    std::string new_name_safe(mysql->to_escape_text(new_name));

    mysql->reset();

    // 更新用户表中的用户名
    COMMAND(command, "update chatroom , chatroommember , user SET chatroom.own_name = '" << new_name_safe << "' , chatroommember.member_name = '" << new_name_safe << "' , user.name = '" << new_name_safe << "'  where chatroom.owner_id = " << id << " and chatroommember.member_id = " << id << " and user.id = " << id << "");
    // COMMAND(command, "UPDATE chatroom SET own_name = '" << new_name_safe << "' WHERE owner_id = " << id);
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (UPDATE) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_REQUEST_RENAME);
    }

    // 查询拥有好友关系的好友ID
    COMMAND(command, "select friend from friend_relationship where own = " << id << "and stat = 1");
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_REQUEST_RENAME);
    }
    char **row = NULL;
    user *use_in_each_row = NULL;

    // 如果好友在线，发送用户新名字的通知
    while ((row = mysql->get_rows_from_result()))
        if ((use_in_each_row = My_user_list.search(std::string(row[0]))))
            use_in_each_row->client->send_message(*message, NOTIFY_FRIEND_NEW_NAME, timestamp);

    mysql->reset();

    COMMAND(command, "insert into User_set_change (id,mode,data,time) values((select friend from friend_relationship where own = " << id << ") , " << ACTION_TO_REQUEST_RENAME_USER << " , '" << mysql->to_escape_text(*message) << "' ,'" << timestamp << "')");
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_REQUEST_RENAME);
    }
    mysql->reset();
    json tmp_j = {{"mode", "done"}, {"new_name", new_name.c_str()}, {"id", id.c_str()}};
    std::string message_tmp = tmp_j.dump();
    point->send_message(message_tmp, USER_RENAME_SUCCESS, timestamp);

    return true;
}
/**
 * REACT FUN
 *
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 *
 * {"group_id": "id", "new_name": "name" , "GROUP_owner_id" : id}
 *
 */
int request_rename_group(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{

    json j = json::parse(*message);
    std::string new_name = j.at("new_name"); // 获取新group名
    std::string group_id = j.at("group_id"); // 获取group ID
    std::string id = j.at("GROUP_owner_id"); // 群主id
    std::string command;

    // 更新组名
    COMMAND(command, "update chatroom set name = '" << mysql->to_escape_text(new_name) << "' where id = " << group_id);
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (UPDATE) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件

        std::string mode_val(std::to_string(RE_GROUP_NAME_SQL_FALSE));
        json tmp_json_to_requester = {{"mode", mode_val.c_str()}, {"group_id", group_id.c_str()}, {"group_new_name", new_name.c_str()}};
        std::string tmp_message_to_requester = tmp_json_to_requester.dump();
        point->send_message(tmp_message_to_requester, RE_GROUP_NAME_SQL_FALSE, timestamp);
        return false;
    }
    mysql->reset();

    // 查询拥有成员关系的ID
    COMMAND(command, "select member_id from chatroommember where id = " << group_id << "and member_id <> " << id);
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        std::string mode_val(std::to_string(NOTIFY_MEMBER_RE_GROUP_NAME_SQL_FALSE));
        json tmp_json_to_requester = {{"mode", mode_val.c_str()}, {"group_id", group_id.c_str()}, {"group_new_name", new_name.c_str()}};
        std::string tmp_message_to_requester = tmp_json_to_requester.dump();
        point->send_message(tmp_message_to_requester, NOTIFY_MEMBER_RE_GROUP_NAME_SQL_FALSE, timestamp);
        // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);
    }
    char **row = NULL;
    user *use_in_each_row = NULL;

    // 如果组员在线，发送组新名字的通知
    while ((row = mysql->get_rows_from_result()))
    {
        if ((use_in_each_row = My_user_list.search(std::string(row[0]))))
            use_in_each_row->client->send_message(*message, NOTIFY_GROUP_NEW_NAME, timestamp);
    }
    mysql->reset();

    COMMAND(command, "INSERT INTO User_set_change(id, mode, data, time) VALUES((SELECT member_id FROM chatroommember WHERE id = " << group_id << " AND member_id <> " << id << "), " << NOTIFY_GROUP_NEW_NAME << ", '" << mysql->to_escape_text(*message) << "','" << timestamp << "')");
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        // std::string mode_val(std::to_string(RE_GROUP_NAME_SUCCESS));
        // json tmp_json_to_requester = {{"mode", mode_val.c_str()}, {"group_id", group_id.c_str()}, {"group_new_name", new_name.c_str()}};
        // std::string tmp_message_to_requester = tmp_json_to_requester.dump();
        // point->send_message(tmp_message_to_requester, RE_GROUP_NAME_SUCCESS, timestamp);
        MYSQL_FAL_AND_RESET(message, RE_GROUP_NAME_SQL_FALSE_IN_NOTIFY);
    }

    std::string mode_val(std::to_string(RE_GROUP_NAME_SUCCESS));
    json tmp_json_to_requester = {{"mode", mode_val.c_str()}, {"group_id", group_id.c_str()}, {"group_new_name", new_name.c_str()}};
    std::string tmp_message_to_requester = tmp_json_to_requester.dump();
    point->send_message(tmp_message_to_requester, RE_GROUP_NAME_SUCCESS, timestamp);
    return true;
}
/**
 * TEST_DONE
 * download chathistory and set_change
 *
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * // json in -- >
 * {"time":"%Y-%m-%d %H:%M:%S"
 * ,"id":"2314"}
 *
 *  json out -- >
 *   {
 *    "user_history": {
 *        "id1": [
 *            {
 *                "data": "data",
 *                "time": "time"
 *            },
 *            {
 *                "data": "data",
 *                "time": "time"
 *            },
 *            {
 *                "data": "data",
 *                "time": "time"
 *            }
 *        ],
 *        "id2": "..."
 *    },
 * "group_history":{
 *         "id1": [
 *            {
 *                "data": "data",
 *                "time": "time"
 *            },
 *            {
 *                "data": "data",
 *                "time": "time"
 *            },
 *            {
 *                "data": "data",
 *                "time": "time"
 *            }
 *        ],
 *        "id2": "..."
 *   },
 *    "set_change": [
 *        {
 *            "mode": "1",
 *            "data": "text/plain"
 *            "time": "time"
 *        }
 *    ]
 *}
 */
int download_user_data(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{ // INIT at client begin
    using namespace std;
    json j = json::parse(*message);
    string time = j.at("time");
    string id = j.at("id");

    string command;

    json message_return;

    //////////////////////////// User_chathistroy ////////////////////////////////
    COMMAND(command, "select tmp.friend ,  h.message  , h.messagetime from (select friend , chatid from friend_relationship where own = " << id << ") as tmp , chathistory as h where tmp.chatid = h.chatid and h.messagetime > '" << time << "' order by tmp.friend ,  h.messagetime");
    // SELECT fr.friend, ch.message, ch.messagetime
    // FROM chathistory ch JOIN friend_relationship fr
    // ON ch.chatid = fr.chatid AND fr.own = 10019 ORDER BY ch.messagetime;
    // 使用JOIN语句查询chathistory和friend_relationship表中特定字段的数据，其中chathistory和friend_relationship通过chatid关联，并且messagetime大于给定时间戳time，同时own为指定的用户id，按照messagetime进行升序排序
    mysql->reset();
    if (mysql->do_(command)) //  查询失败
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_DOWNLOAD_USER_DATA);
    }
    else
    {

        if (mysql->get_rows_from_result() == NULL) // 执行第一次查询 // 没数据
        {
            message_return["friend_history"] = nullptr;
        }
        else
        {
            // string id;
            json data = json::object(); // 向这个data中加的是 id : [{message_kv + time_kv},{...}] 的kv 这将包含 所有id 的历史消息
            while (1)
            {
                char **row = mysql->row;     // 第一次查询的 row ptr
                string id(row[0]);           // friend_id 作为 key
                json arrays = json::array(); // 作为 id 的 value_array
                while (1)
                {
                    if (row == NULL || strcmp(id.c_str(), row[0]) != 0) // 上一个id的message读完 或者 整个读完
                        break;
                    json tmp;
                    tmp["data"] = row[1];
                    tmp["time"] = row[2];
                    arrays.push_back(tmp);

                    row = mysql->get_rows_from_result(); // 读下一行 读下一行 读下一行 读下一行 读下一行
                }
                data[id.c_str()] = arrays; // 所有的history_message和对应的time构成的数组 作为value

                if (row == NULL) // 整个读完
                    break;
            }
            message_return["friend_history"] = data;
            mysql->reset();
        }
    }
    mysql->reset();
    //////////////////////////// User_chathistroy ////////////////////////////////

    //////////////////////////// group_chathistroy ////////////////////////////////
    COMMAND(command, "SELECT chatroom.id, chathistory.message, chathistory.messagetime FROM chathistory JOIN chatroommember ON chatroommember.member_id = " << id << " JOIN chatroom ON chathistory.chatid = chatroom.chatid AND chatroom.id = chatroommember.id WHERE chathistory.messagetime > '" << time << "' AND chathistory.chatid = chatroom.chatid ORDER BY chatroom.id, chathistory.messagetime");
    if (mysql->do_(command)) // 查询失败
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_DOWNLOAD_GROUP_DATA);
    }
    else
    {
        char **row = mysql->get_rows_from_result();
        if (row == NULL) // 没结果
            message_return["group_history"] = nullptr;
        else
        {
            // string id;
            json data = json::object(); // 向这个data中加的是 id : [{message_kv + time_kv},{...}] 的kv 这将包含 所有id 的历史消息
            while (1)
            {
                char **row = mysql->row;     // 第一次查询的 row ptr
                string id(row[0]);           // group_id 作为 key
                json arrays = json::array(); // 作为 id 的 value_array
                while (1)
                {
                    if (row == NULL || strcmp(id.c_str(), row[0]) != 0) // 上一个id的message读完 或者 整个读完
                        break;
                    json tmp;
                    tmp["data"] = row[1];
                    tmp["time"] = row[2];
                    arrays.push_back(tmp);

                    row = mysql->get_rows_from_result(); // 读下一行 读下一行 读下一行 读下一行 读下一行
                }
                data[id.c_str()] = arrays; // 所有的history_message和对应的time构成的数组 作为value

                if (row == NULL) // 整个读完
                    break;
            }
            message_return["group_history"] = data;
            mysql->reset();
        }
    }

    mysql->reset();

    //////////////////////////// group_chathistroy ////////////////////////////////

    //////////////////////////// User_set_change ////////////////////////////////////
    COMMAND(command, "select mode , data, time from User_set_change where id = " << id << " order by time");
    if (mysql->do_(command)) //  查询失败
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_DOWNLOAD_USER_DATA);
    }
    else
    {

        if (mysql->get_rows_from_result() == NULL) // 执行第一次查询 // 没数据
        {
            message_return["set_change"] = nullptr;
            mysql->reset();
        }
        else
        {
            json array = json::array();
            char **row = mysql->row;
            while (row)
            {
                json ob = json::object();
                ob["mode"] = row[0];
                ob["data"] = row[1];
                ob["time"] = row[2];
                array.push_back(ob);
                row = mysql->get_rows_from_result();
            }
            message_return["set_change"] = array;
            mysql->reset();
        }
    }

    string message_out = message_return.dump();
    //////////////////////////// User_set_change ////////////////////////////////////

    point->send_message(message_out, ACTION_TO_DOWNLOAD_USER_DATA_SUCCESS, timestamp);
    return true;
}
/**
 * react群组的成员信息
 *
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * {
 *  "group_id" : "14234"
 * }
 *
 */
int download_group_data(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    using namespace std;
    json j = json::parse(*message);
    string group_id = j.at("group_id");

    string command;
    COMMAND(command, "select mode ,member_id , member_name from chatroommember where id  =  " << group_id << " and mode > 0 order by mode desc , member_name");
    mysql->reset();
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (SELECT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        // json tmp = {{"mode", "false"}, {"data", "SQL FALSE"}};
        // string tmp_message = tmp.dump();
        // point->send_message(tmp_message, DOWNLOAD_GROUP_DATA_FALSE, timestamp);

        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_DOWNLOAD_GROUP_DATA);
    }
    char **row;
    json return_json;
    json mode1_array = json::array();
    json mode2_array = json::array();
    json mode3_array = json::array();
    while ((row = mysql->get_rows_from_result()))
    {
        if (strcmp(row[0], "1") == 0)
        {
            json tmp = {{row[1], row[2]}};
            mode1_array.push_back(tmp);
        }
        else if (strcmp(row[0], "2") == 0)
        {
            json tmp = {{row[1], row[2]}};
            mode2_array.push_back(tmp);
        }
        else if (strcmp(row[0], "3") == 0)
        {
            json tmp = {{row[1], row[2]}};
            mode3_array.push_back(tmp);
        }
        else
            continue;
    }
    mysql->reset();

    return_json["owner"] = mode3_array;
    return_json["manager"] = mode2_array;
    return_json["member"] = mode1_array;

    string return_string = return_json.dump();
    point->send_message(return_string, DOWNLOAD_GROUP_DATA_SUCCESS, timestamp);

    return true;
}
/**
 * react fun
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * json -->
 * {
 *   "filename" = "name" ,
 *   "self_id" = "2143",
 *   "target_id" = "2341" ,
 *
 *    "ip" = "127.0.0.1" ,
 *    "port" = "2341" ,
 *
 *    "size" = "12342341" // bytes
 * }
 *
 *  std::string file_path("/tmp/chatroom/" + filename + "_" + self_id);
 *
 * 需要在客户端初始化一个listen_fd 并将listen_fd的ip和端口传过来
 * 这个函数将在服务器中 创建一个临时的socket 用来文件的接受
 *
 * 前端要记录传输的file_name 当传重复上传一个文件时 对文件名进行修饰
 *
 * 在文件上传结束后 给临时的链接发一个  json tmp_j = {{"file_name", filename.c_str()}, {"mode", "done"}};
 * 这样的 tmp_j(json) 然后关闭链接
 *
 * target在线 则
 *  进行通知有文件下载请求
 *  target 收到的message 和上述的一样
 *
 * 进行sql 记录
 *
 * 发送文件数据函数，将socket连接中接收到的文件数据写入临时文件中
 */
int send_file_data(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    // 解析json格式的消息体
    json j = json::parse(*message);
    std::string filename = j.at("filename");
    std::string self_id = j.at("self_id");
    std::string target_id = j.at("target_id");
    std::string file_size_str = j.at("size");
    std::string ip_str = j.at("ip");
    std::string port_str = j.at("port");

    // 将字符串类型的文件大小和端口号转换为数字类型
    size_t file_size = std::stol(file_size_str);
    in_port_t port = std::stoi(port_str);

    std::string file_path("/tmp/chatroom/" + filename + "_" + self_id);
    // 临时文件名(路径) "/tmp/chatroom/filename_10019"
    int temp_sock_fd;
    // 创建临时socket连接 // 进行文件的上传
    {
        temp_sock_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (temp_sock_fd < 0)
        {
            // 如果创建socket连接失败，则发送错误消息，并返回false
            GLOG_ERROR("no socket fd more", 1);
            *message = "{ \"ERROR\": \"server busy\"}";
            point->send_message(*message, NO_SOCK_MORE, timestamp);
            return false;
        }

        // 准备临时socket连接的地址信息
        struct sockaddr_in temp_addr;
        temp_addr.sin_family = AF_INET;
        temp_addr.sin_port = port; // 使用临时端口号
        temp_addr.sin_addr.s_addr = inet_addr(ip_str.c_str());

        // 设置超时时间为5秒
        struct timeval tv; // 超时时间为5秒
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        if (setsockopt(temp_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) == -1)
        {
            GLOG_WARNING("tmp sock SET ERROR", 1);
            *message = "{ \"ERROR\": \"tmp sock SET ERROR \"}";
            point->send_message(*message, SERVER_CONNECT_TO_RECV_FILE_ERROR, timestamp);
            close(temp_sock_fd);
            return false;
        }

        // 进行临时socket连接
        if (connect(temp_sock_fd, (sockaddr *)&temp_addr, sizeof(temp_addr)) != 0) // 链接 超时或者出错
        {
            // 如果连接超时或出错，则发送错误消息，并返回false
            GLOG_WARNING("tmp sock CONNCET TIME OUT", 1);
            *message = "{ \"ERROR\": \" didn't connect | time out \"  , \"filename\" : \"" + filename + "\"}}";
            point->send_message(*message, SERVER_CONNECT_TO_RECV_FILE_ERROR, timestamp);
            close(temp_sock_fd);
            return false;
        }

        // 打开临时文件
        int file_fd = open(file_path.c_str(), O_RDWR);
        // 定义缓冲区并进行文件数据的接收和写入
        char buffer[BUFFER_SIZE] = {0};
        int n = 0; // number of bytes recved
        while ((n = recv(temp_sock_fd, buffer, BUFFER_SIZE - 1, 0)) > 0)
            write(file_fd, buffer, n);

        if (n == -1)
        {
            GLOG_WARNING("tmp sock recv error", 1);
            *message = "{ \"ERROR\": \" file save_error \", \"filename\": \"" + filename + "\"}";
            point->send_message(*message, SERVER_CONNECT_TO_RECV_FILE_ERROR, timestamp);
            close(temp_sock_fd);
            // temp_sock_fd = -1;
            close(file_fd);
            return false;
        }
        // 关闭文件连接
        close(file_fd);

        // 向主链接传输 save done
        /////// react here
        json tmp_j = {{"file_name", filename.c_str()}, {"mode", "1"}};
        std::string tmp = tmp_j.dump();
        // send(temp_sock_fd, tmp.c_str(), tmp.length() + 1, 0);
        point->send_message(tmp, FILE_UPLOAD_SUCCESS, timestamp);

        // 关闭链接
        close(temp_sock_fd);
    }
    // 文件上传(save)成功则执行到这里

    user *tmp_user_ptr = My_user_list.search(target_id);
    if (tmp_user_ptr != nullptr)
        tmp_user_ptr->client->send_message(*message, ACTION_TO_RECV_FILE_DATA, timestamp);
    else
        ;

    std::string command;
    COMMAND(command, "insert into User_set_change(id,mode,data,time) values(" << target_id << "," << ACTION_TO_RECV_FILE_DATA << ",'" << mysql->to_escape_text(*message) << "','" << timestamp << "')");
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_SEND_FILE_DATA);
    }
    else
        mysql->reset();

    return true;
}

/**
 *
 *  react
 *  (client)recv_file_data
 *  向用户发送file
 *
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * ------json---------
 * {
 *   "file_owner_id": "12345",
 *    "file_name": "filename",
 *
 *     "ip" : "1.1.1.1",
 *     "port" : "1010"
 * }
 *
 * 需要分线程去做(进程?)
 */
int recv_file_data(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    // 解析json字符串
    json j = json::parse(*message);
    std::string file_owner_id = j.at("file_owner_id"); // 文件所属者ID
    std::string file_name = j.at("file_name");         // 文件名
    std::string ip_str = j.at("ip");                   // IP地址
    std::string port_str = j.at("port");               // 端口号

    // 将端口号从字符串类型转换为in_port_t类型
    in_port_t port = std::stoi(port_str);

    // 拼接文件路径和文件名
    std::string file_path("/tmp/chatroom/" + file_name + "_" + file_owner_id);

    // 打开文件，如果打开失败则发送错误消息并返回false
    int file_fd = open(file_path.c_str(), O_RDONLY);
    if (file_fd == -1) // 文件未找到
    {
        *message = "\"ERROR\":\"Error FILE NAME || CONNOT OPEN\" , \"filename\" : \"" + file_name + "\"}";
        GLOG_ERROR(*message << "file_path: " << file_path, 1);    // 记录日志
        point->send_message(*message, FILE_NOT_FOUND, timestamp); // 向客户端发送错误消息
        return false;                                             // 返回false
    }

    int temp_sock_fd;
    // 创建临时socket连接
    {
        temp_sock_fd = socket(AF_INET, SOCK_STREAM, 0); // 创建socket连接
        if (temp_sock_fd < 0)                           // 如果创建socket连接失败，则发送错误消息，并返回false
        {
            GLOG_ERROR("no socket fd more", 1); // 记录日志
            *message = "{ \"ERROR\": \"server busy\" , \"filename\" : \"" + file_name + "\"}}";
            point->send_message(*message, SERVER_CONNECT_TO_RECV_FILE_ERROR, timestamp); // 向客户端发送错误消息
            close(file_fd);                                                              // 关闭文件描述符
            return false;                                                                // 返回false
        }

        // 准备临时socket连接的地址信息
        struct sockaddr_in temp_addr;
        temp_addr.sin_family = AF_INET;
        temp_addr.sin_port = port; // 使用临时端口号
        temp_addr.sin_addr.s_addr = inet_addr(ip_str.c_str());

        // 设置超时时间为5秒
        struct timeval tv; // 超时时间为5秒
        tv.tv_sec = 5;
        tv.tv_usec = 0;
        if (setsockopt(temp_sock_fd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) == -1)
        {
            GLOG_WARNING("tmp sock SET ERROR", 1); // 记录日志
            *message = "{ \"ERROR\": \"tmp sock SET ERROR \" , \"filename\" : \"" + file_name + "\"}";
            point->send_message(*message, SERVER_CONNECT_TO_RECV_FILE_ERROR, timestamp); // 向客户端发送错误消息
            close(file_fd);                                                              // 关闭文件描述符
            close(temp_sock_fd);                                                         // 关闭socket连接
            return false;                                                                // 返回false
        }

        // 进行临时socket连接
        if (connect(temp_sock_fd, (sockaddr *)&temp_addr, sizeof(temp_addr)) != 0) // 链接 超时或者出错
        {
            GLOG_WARNING("tmp sock CONNCET TIME OUT", 1); // 记录日志
            *message = "{ \"ERROR\": \" didn't connect | time out \" , \"filename\" : \"" + file_name + "\"}}";
            point->send_message(*message, SERVER_CONNECT_TO_RECV_FILE_ERROR, timestamp); // 向客户端发送错误消息
            close(temp_sock_fd);                                                         // 关闭socket连接
            close(file_fd);                                                              // 关闭文件描述符
            return false;                                                                // 返回false
        }
    }

    // 进行文件的下载
    struct stat tmp_file_stat;
    stat(file_path.c_str(), &tmp_file_stat);
    off_t sent_byte = sendfile(temp_sock_fd, file_fd, NULL, tmp_file_stat.st_size); // 发送文件内容
    if (sent_byte == -1)                                                            // send file 失败
    {
        GLOG_WARNING("sendfile failed FILEPATH > " << file_path << " json:> " << *message, 1); // 记录日志
        json tmp = {{"ERROR", "sendfile() failed"}, {"filename", file_name.c_str()}};
        std::string tmp_message = tmp.dump();
        point->send_message(tmp_message, SENDFILE_ERROR, timestamp);
        // 向 主链接 发送send失败
        close(file_fd);      // 关闭文件描述符
        close(temp_sock_fd); // 关闭socket连接
        return false;        // 返回false
    }
    else // send file successfully
    {
        json tmp = {{"filename", file_name.c_str()}, {"mode", "done"}};
        std::string message = tmp.dump();
        point->send_message(message, SAVEFILE_SUCCESS, timestamp);
        // 发送成功消息

        close(file_fd);      // 关闭文件描述符
        close(temp_sock_fd); // 关闭socket连接
    }
    return true;
}
/**
 * react fun
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * {
 *  group_id : 53462
 *  mamager_id : 1234
 *  target_id : 12345
 *  target_name : "wang"
 *
 *  mode : 2/3/4 踢人 添加管理员 删管理
 *  data : "explain"
 * }
 */
int manage_group_member(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    using namespace std;
    json j = json::parse(*message);
    string mamager_id = j.at("mamager_id");
    string target_id = j.at("target_id");
    string target_name = j.at("target_name");
    string group_id = j.at("group_id");
    string mode = j.at("mode");
    string data = j.at("data");

    string command;
    // check
    COMMAND(command, "select mode from chatroommember where id = " << group_id << " and member_id = " << mamager_id);
    if (mysql->do_(command))
    {
        GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        MYSQL_FAL_AND_RESET(message, SQL_ERROR_IN_MANAGER_GROUP_MEMBERS);
    }
    char **row = mysql->get_rows_from_result();
    char grade = row[0][0]; // 发送请求的人的等级 // '2'mamager // '3'owner
    if (!(grade > '1'))     // 非管理 / 超级用户
    {
        // 向请求执行者发
        json tmp = {{"false", "not mamager"}, {"mode", mode.c_str()}, {"target_id", target_id.c_str()}};
        string tmp_message = tmp.dump();
        point->send_message(tmp_message, ACTION_TO_MANAGE_GROUP_MEMBER_FAILE, timestamp);
        return false;
    }
    mysql->reset();

    // manage
    if (mode == "~1") // 加人 // 重合了 加人 不要走这个fun 用 request/answer_join_group()
    {
        mysql->reset();

        // 构建 insert 语句
        COMMAND(command, "insert into chatroommembers (id,member_id,member_name) values( " << group_id << ", " << target_id << ", '" << mysql->to_escape_text(target_name) << "')");
        if (mysql->do_(command)) // 向数据库执行insert // 失败
        {
            // 写入日志文件
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true);
            mysql->reset(); // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);

            // 构建加人失败信息 返回给 管理或者群主
            string mode_value(std::to_string(SQL_ERROR_MANAGE_JOIN_GROUP_FALSE));
            json tmp_json = {{"mode", "false"}, {"target_id", target_id.c_str()}, {"group_id", group_id.c_str()}, {"mamager_mode", mode_value.c_str()}};
            string tmp_message = tmp_json.dump();
            point->send_message(tmp_message, SQL_ERROR_MANAGE_JOIN_GROUP_FALSE, timestamp);

            return false;
        }
        mysql->reset();

        // 向数据库insert成功后

        // 构建 加人成功消息 react 给 管理或群主
        string mode_value(std::to_string(MANAGE_JOIN_GROUP_SUCCESS));
        json tmp_json = {{"mode", "done"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
        string tmp_message = tmp_json.dump();
        point->send_message(tmp_message, MANAGE_JOIN_GROUP_SUCCESS, timestamp);

        // 向被加入用户发消息

        // 用户在线
        user *tmp = My_user_list.search(target_id);
        if (tmp != nullptr)
            tmp->client->send_message(*message, ACTION_TO_MANAGE_GROUP_MEMBER, timestamp);

        // 构建insert语句 加入User_set_change

        // COMMAND(command, "insert into User_set_change(id,mode,data) values(" << target_id << "," << ACTION_TO_MANAGE_GROUP_MEMBER << ",'" << mysql->to_escape_text(data) << "')");
        // mysql->reset();
        // if (mysql->do_(command))
        // {
        //     GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        //
        //     // string mode_value(std::to_string(MANAGE_MANAGER_IN_GROUP_FALSE));
        //     // json tmp_json = {{"mode", "false"}, {"target_id", target_id.c_str()}, {"group_id", group_id.c_str()}, {"mamager_mode", mode_value.c_str()}};
        //     // string tmp_message = tmp_json.dump();
        //     // point->send_message(tmp_message, MANAGE_MANAGER_IN_GROUP_FALSE);,timestamp
        // }

        // 通知target
        {
            user *target_user_cl = My_user_list.search(target_id);
            if (target_user_cl != nullptr)
            {
                target_user_cl->client->send_message(*message, ACTION_TO_MANAGE_GROUP_MEMBER, timestamp);
            }
            COMMAND(command, "insert into User_set_change(id,mode,data,time) values(" << target_id << ", " << ACTION_TO_MANAGE_GROUP_MEMBER << " , '" << mysql->to_escape_text(*message) << "','" << timestamp << "')");
            if (mysql->do_(command))
            {
                GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
            }
            mysql->reset();
        }

        // return true;
    }
    else if (mode == "2") // 踢人
    {
        mysql->reset();
        // 构建delete语句
        COMMAND(command, "delete from chatroommember where member_id = " << target_id);
        // 执行
        if (mysql->do_(command)) // 失败
        {
            // 写入日志文件
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true);
            mysql->reset(); // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);

            // 构建踢人失败信息 返回给 管理或者群主
            string mode_value(std::to_string(SQL_ERROR_MANAGE_DELETE_FROM_GROUP_FALSE));
            json tmp_json = {{"mode", "false"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
            string tmp_message = tmp_json.dump();
            point->send_message(tmp_message, SQL_ERROR_MANAGE_DELETE_FROM_GROUP_FALSE, timestamp);

            return false;
        }
        // 构建踢人成功信息 返回给 管理或者群主
        string mode_value(std::to_string(MANAGE_DELETE_FROM_GROUP_SUCCESS));
        json tmp_json = {{"mode", "done"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
        string tmp_message = tmp_json.dump();
        point->send_message(tmp_message, MANAGE_DELETE_FROM_GROUP_SUCCESS, timestamp);

        // 向 target 发送
        user *tmp = My_user_list.search(target_id);
        if (tmp != nullptr)
            tmp->client->send_message(*message, ACTION_TO_MANAGE_GROUP_MEMBER, timestamp);

        // 构建insert语句 加入User_set_change
        COMMAND(command, "insert into User_set_change(id,mode,data,time) value(" << target_id << "," << ACTION_TO_MANAGE_GROUP_MEMBER << ",'" << mysql->to_escape_text(data) << "','" << timestamp << "')");
        mysql->reset();
        if (mysql->do_(command))
        {
            GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        }
        mysql->reset();

    } // 踢人
    else if (mode == "3" && grade == '3')
    {
        COMMAND(command, "update chatroommember set mode = 2 where member_id = " << target_id << ")");
        if (mysql->do_(command)) // update 执行 // fail_do
        {
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true);
            mysql->reset(); // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);

            // 构建失败信息 返回给  群主
            string mode_value(std::to_string(SQL_ERROR_MANAGE_SETUP_MANAGER_FROM_GROUP_FALSE));
            json tmp_json = {{"mode", "false"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
            string tmp_message = tmp_json.dump();
            point->send_message(tmp_message, SQL_ERROR_MANAGE_SETUP_MANAGER_FROM_GROUP_FALSE, timestamp);
            return false;
        }
        else
        {
            mysql->reset(); // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);

            // 构建成功信息 返回给  群主
            string mode_value(std::to_string(MANAGE_SETUP_MANAGER_FROM_GROUP_SUCCESS));
            json tmp_json = {{"mode", "done"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
            string tmp_message = tmp_json.dump();
            point->send_message(tmp_message, MANAGE_SETUP_MANAGER_FROM_GROUP_SUCCESS, timestamp);
        }

        user *tmp_user = My_user_list.search(target_id);
        if (tmp_user != nullptr)
        {
            tmp_user->client->send_message(*message, ACTION_TO_MANAGE_GROUP_MEMBER, timestamp);
        }

        COMMAND(command, "insert into User_set_change(id,mode,data,time) value(" << target_id << "," << ACTION_TO_MANAGE_GROUP_MEMBER << ",'" << mysql->to_escape_text(data) << "','" << timestamp << "')");
        mysql->reset();
        if (mysql->do_(command))
        {
            GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        }
        mysql->reset();
    }
    else if (grade == '3' && mode == "4") // 删除管理
    {
        // 构建 update 语句 把管理降级
        COMMAND(command, "update chatroommember set mode = 1 where member_id = " << target_id << ")");
        if (mysql->do_(command)) // update 执行 // fail_do
        {
            GLOG_WARNING("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true);
            mysql->reset(); // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);

            // 构建失败信息 返回给  群主
            string mode_value(std::to_string(SQL_ERROR_MANAGE_DELETE_MANAGER_FROM_GROUP_FALSE));
            json tmp_json = {{"mode", "false"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
            string tmp_message = tmp_json.dump();
            point->send_message(tmp_message, SQL_ERROR_MANAGE_DELETE_MANAGER_FROM_GROUP_FALSE, timestamp);
            return false;
        }
        else // update 成功
        {
            mysql->reset(); // MYSQL_FAL_AND_RESET(message, ACTION_TO_ERROR);

            // 构建成功信息 返回给  群主
            string mode_value(std::to_string(MANAGE_DELETE_MANAGER_FROM_GROUP_SUCCESS));
            json tmp_json = {{"mode", "done"}, {"id", target_id.c_str()}, {"group_id", group_id.c_str()}};
            string tmp_message = tmp_json.dump();
            point->send_message(tmp_message, MANAGE_DELETE_MANAGER_FROM_GROUP_SUCCESS, timestamp);
        }

        // 对target执行
        user *tmp_user = My_user_list.search(target_id);
        if (tmp_user != nullptr) // 在线
        {
            tmp_user->client->send_message(*message, ACTION_TO_MANAGE_GROUP_MEMBER, timestamp);
        }

        // 在 user_set 表里塞更新消息
        COMMAND(command, "insert into User_set_change(id,mode,data,time) value(" << target_id << "," << ACTION_TO_MANAGE_GROUP_MEMBER << ",'" << mysql->to_escape_text(data) << "','" << timestamp << "')");
        mysql->reset();
        if (mysql->do_(command))
        {
            GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        }
        mysql->reset();
    }
    // react ed
    COMMAND(command, "select member_id from chatroommember where id = " << group_id << " and mode >= 1 order by mode , member_id");
    mysql->reset();
    if (mysql->do_(command))
    {
        GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
    }
    char **line = NULL;
    while ((line = mysql->get_rows_from_result()))
    {
        auto a = My_user_list.search(line[0]);
        if (a != nullptr)
        {
            a->client->send_message(*message, ACTION_TO_MANAGE_GROUP_MEMBER, timestamp);
        }
    }
    return true;
}
/**
 * // 用户拉黑
 *
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 * {
 *   user_id = 2134,
 *   target_id = 6542,
 * }
 */
int remind_user_member(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    using namespace std;
    json j = json::parse(*message);
    string user_id = j.at("user_id");
    string target_id = j.at("target_id");

    user *tmp_user_cl = My_user_list.search(target_id); // 在线
    if (tmp_user_cl != nullptr)
    {
        {
            std::unique_lock<std::mutex> lock(tmp_user_cl->unfriend_set_lock);
            tmp_user_cl->unfriend.insert(user_id); // 线程不安全····「done 加锁了」
        }
        tmp_user_cl->client->send_message(*message, ACTION_TO_REMIND_USER_MEMBER, timestamp); // 告诉target 自己被拉黑
    }

    string command;
    COMMAND(command, "update friend_relationship set stat = 0 where own = " << target_id << " and friend = " << user_id);
    mysql->reset();
    if (mysql->do_(command)) // update // false
    {
        GLOG_WARNING("chat_data (update) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        // json tmp = {{"mode", "false"}, {"data", "SQL FALSE"}};
        // string tmp_message = tmp.dump();
        // point->send_message(tmp_message, SQL_ERROR_REMIND_USER_FALSE, timestamp);

        MYSQL_FAL_AND_RESET(message, SQL_ERROR_REMIND_USER_FALSE); // 这里return
    }
    mysql->reset();
    COMMAND(command, "insert into User_set_change(id,mode,data,time) values(" << target_id << "," << ACTION_TO_REMIND_USER_MEMBER << ",'" << mysql->to_escape_text(*message) << "','" << timestamp << "')");
    if (mysql->do_(command)) // insert // false
    {
        GLOG_ERROR("chat_data (update) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
    }
    mysql->reset();
    // react
    string return_str(R"({"mode":"done","target_id":")" + target_id + "\"}");
    point->send_message(return_str, REMIND_USER_SUCCESS, timestamp);
    return true;
}
/**
 * // message 是 json --> 需要处理的字段
 * // point 指向 发来请求的 client
 * // mysql 是指 每个线程 不同的 和MYSQL客户端链接的字段
 */
// 不要求实现 群组拉黑
int remind_group_member(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    return true;
}

int dead_notification(std::string *dead_id, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    point = NULL;
    // mysql = NULL;
    using namespace std;
    string command;
    COMMAND(command, "select friend from friend_relationship where own = " << dead_id);
    if (mysql->do_(command) == 0)
    {
        char **row;
        user *tmp;
        while ((row = mysql->get_rows_from_result()))
        {
            if ((tmp = My_user_list.search(row[0])))
            {
                // json tmp = {{"id" , dead_id->c_str()}}
                string tmp_str = R"({"id": ")" + *dead_id + "\"})";
                tmp->client->send_message(tmp_str, FRIEND_SIGN_OUT, timestamp);
            }
        }
    }
    mysql->reset();
    return true;
}
/**
 * react
 *
 * json ------
 * {"owner_id" : id, "GROUP_name" : name }
 *
 *
 */
int make_group(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    using namespace std; // namespace std;
    json a = json::parse(*message);
    string chatid;
    getMillisecondTimestamp(chatid);
    string owner_id = a.at("owner_id");
    string group_name = a.at("group_name");
    string command;
    chatid += owner_id;

    COMMAND(command, "insert into chatroom(name,owner_id,own_name,chatid) values('" << mysql->to_escape_text(group_name) << "'," << owner_id << ",(select name from user where id = " << owner_id << "),'" << chatid << "')");
    mysql->reset();
    if (mysql->do_(command)) // 不成功
    {
        GLOG_ERROR("chat_data (INSERT) in SQL failed __ COMMAND:>" << command, true); // 写入日志文件
        string tmp_message(R"({"mode":"false","reason":"sql error"})");
        point->send_message(tmp_message, MAKE_GROUP_FALSE, timestamp);
        return false;
    }
    string group_id(mysql->get_last_insert_id());
    mysql->reset();

    json return_json = {{"id", group_id.c_str()}, {"group_name", group_name.c_str()}, {"chatid", chatid.c_str()}};
    string return_str = return_json.dump();

    point->send_message(return_str, MAKE_GROUP_SUCCESS, timestamp);
    return true;
}

/**
 *{id : "19999"}
 {
    "friend": [
        {
            "id": "10001",
            "name": "testuser",
            "chatid": "chatid",
            "stat": "1 / 0"
        },
        {
            "id": "10002",
            "name": "testuser",
            "chatid": "chatid2",
            "stat": "1 / 0"
        }
    ],
    "group": [
        {
            "id": "98679",
            "name": "testGroup",
            "own_name": "name",
            "owner_id": "56789",
            "chatid": "chatid"
        }
    ]
}
*/
int init_data(std::string *message, socket_connect_to_client *point, MySql *mysql, char *timestamp)
{
    using namespace std;
    json j = json::parse(*message);
    string id = j.at("id");
    char **row = NULL;
    json rt;
    json jsarr = json::array();
    string command;
    string rt_msg;

    COMMAND(command, "select b.friend , a.name , b.chatid, b.stat from friend_relationship as b ,(select name , id from user) as a where b.own = " << id << " and b.friend = a.id order by a.name");
    mysql->reset();
    if (mysql->do_(command))
        goto fail_do;

    while ((row = mysql->get_rows_from_result()) != NULL)
    {
        json tmp = {{"id", row[0]}, {"name", row[1]}, {"chatid", row[2]}, {"stat", row[3]}};
        jsarr.push_back(tmp);
    }
    if (jsarr.empty())
        rt["friends"] = nullptr;
    else
        rt["friends"] = jsarr;
    jsarr.clear();
    mysql->reset();
    COMMAND(command, "select id , name , owner_id, own_name , chatid from chatroom where id in (select id from chatroommember where member_id = " << id << ")");
    if (mysql->do_(command))
        goto fail_do;
    while ((row = mysql->get_rows_from_result()) != NULL)
    {
        json tmp = {{"id", row[0]}, {"name", row[1]}, {"owner_id", row[2]}, {"own_name", row[3]}, {"chatid", row[4]}};
        jsarr.push_back(tmp);
    }
    if (jsarr.empty())
        rt["groups"] = nullptr;
    else
        rt["groups"] = jsarr;

    rt_msg = rt.dump();
    point->send_message(rt_msg, ACTION_TO_INIT_DATA_SUCCESS, timestamp);
    return true;
fail_do:
    mysql->reset();
    rt_msg = R"({"mode":"SQL false"})";
    point->send_message(rt_msg, ACTION_TO_INIT_DATA_FALSE, timestamp);
    return false;
}

std::string find_user_name(std::string id, MYSQL *mysql)
{
    using namespace std;
    string name = "";
    string command = "SELECT name FROM user WHERE id = '" + id + "'";
    if (mysql_query(mysql, command.c_str()) != 0)
    {
        // 查询失败，输出错误信息
        // cerr << "Error: " << mysql_error(mysql) << endl;
    }
    else
    {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result != nullptr && mysql_num_rows(result) == 0)
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != nullptr && row[0] != nullptr)
            {
                name = row[0]; // 获取查询结果中的第一个字段
            }
        }
        mysql_free_result(result); // 释放结果集内存
    }
    return name;
}

std::string find_group_name(std::string id, MYSQL *mysql)
{
    using namespace std;
    string name = "";
    string command = "SELECT name FROM chatroom WHERE id = '" + id + "'";
    if (mysql_query(mysql, command.c_str()) == 0)
    {
        MYSQL_RES *result = mysql_store_result(mysql);
        if (result != nullptr && mysql_num_rows(result) == 0)
        {
            MYSQL_ROW row = mysql_fetch_row(result);
            if (row != nullptr && row[0] != nullptr)
            {
                name = row[0]; // 获取查询结果中的第一个字段
            }
        }
        mysql_free_result(result); // 释放结果集内存
    }
    return name;
}