#pragma once
#include "client.hh"

#define clear_window std::cout << "\033[2J\033[1;1H" << std::flush
enum WIN_STATE
{
    // init stat
    // Un_log = 0,
    // //
    // To_Log_In,
    // To_Set_Up,
    //
    Begin,
    Friend_Chat,
    Group_Chat,
    Group_Data,
    Manage_Friend, // 加好友
    Manage_Group,
    Manage_Self,
    Notices,
    End
};
enum group_level
{
    Group_Member = 10,
    Group_Manager,
    Group_Owner
};
#define set_window_size 11
using win_stat = uint;
// using ROOT = 4294967295U;
#define ROOT 4294967295U

bool windows_map[set_window_size][set_window_size];

// char Un_log_map[set_window_size + 1] = {};
// char To_Log_In_map[set_window_size + 1] = {};
// char To_Set_Up_map[set_window_size + 1] = {};
// char Begin_map[set_window_size + 1] = {};
// char Friend_Chat_map[set_window_size + 1] = {};
// char Group_Chat_map[set_window_size + 1] = {};
// char Group_Data_map[set_window_size + 1] = {};
// char Manege_Friend_map[set_window_size + 1] = {};
// char Manage_Group_map[set_window_size + 1] = {};
// char Manage_Self_map[set_window_size + 1] = {};
void Set_windows_map()
{
    memset(windows_map, 0, set_window_size * set_window_size);

    // windows_map[Un_log][To_Log_In] = 1;
    // windows_map[Un_log][To_Set_Up] = 1;
    // windows_map[Un_log][End] = 1;

    // windows_map[To_Log_In][Begin] = 1;
    // windows_map[To_Log_In][End] = 1;
    // windows_map[To_Log_In][Un_log] = 1;
    // windows_map[To_Log_In][To_Set_Up] = 1;

    // windows_map[To_Set_Up][Begin] = 1;
    // windows_map[To_Set_Up][End] = 1;
    // windows_map[To_Set_Up][Un_log] = 1;

    windows_map[Begin][Manage_Group] = 1;
    windows_map[Begin][Manage_Friend] = 1;
    windows_map[Begin][Manage_Self] = 1;
    windows_map[Begin][Friend_Chat] = 1;
    windows_map[Begin][Group_Chat] = 1;
    windows_map[Begin][Notices] = 1;
    windows_map[Begin][End] = 1;

    windows_map[Friend_Chat][Begin] = 1;

    windows_map[Group_Chat][Begin] = 1;
    windows_map[Group_Chat][Manage_Group] = 1;
    windows_map[Group_Chat][Group_Data] = 1;

    windows_map[Group_Data][Group_Chat] = 1;
    windows_map[Group_Data][Manage_Group] = 1;

    windows_map[Manage_Friend][Begin] = 1;

    windows_map[Manage_Group][Group_Chat] = 1;
    windows_map[Manage_Group][Begin] = 1;
    windows_map[Manage_Group][Group_Data] = 1;

    windows_map[Notices][Begin] = 1;

    windows_map[Manage_Self][Begin] = 1;

    // windows_map[End][ALL] = 0;
}

// struct user_data aa;
// struct user_data a;
// a.operator==(aa);
// a == aa;

// std::string group
// std::st

struct user_data
{
    virtual bool operator==(user_data const &other) const { return this->id == other.id; }
    virtual bool operator!=(user_data const &other) const { return !operator==(other); }
    virtual bool operator>(user_data const &other) const { return this->id > other.id; }
    virtual bool operator<(user_data const &other) const { return this->id < other.id; }
    virtual bool operator<=(user_data const &other) const { return !operator>(other); }
    virtual bool operator>=(user_data const &other) const { return !operator<(other); }
    user_data() = default;
    user_data(std::string &id, std::string &name, std::string &stat) : id(std::move(id)), name(std::move(name)), stat(std::move(stat)) {}
    // user_data(std::string &id, std::string &name) : id(std::move(id)), name(std::move(name)) {}
    user_data(std::string &&id, std::string &&name) : id(std::move(id)), name(std::move(name)) {
        this->stat = "1";
    }
    std::string id = "";
    std::string stat = ""; // 在好友列表中 指代好友的关系 在群组中指代manager/owner
    std::string name = "";
};

struct friend_data : private user_data
{
    using user_data::operator==;
    using user_data::operator!=;
    using user_data::operator>;
    using user_data::operator<;
    using user_data::operator<=;
    using user_data::operator>=;

    // using user_data::chatid;
    // using user_data::id;
    // using user_data::name;
    using user_data::user_data;
    // friend_data():is_new_chat(false) {};
    friend_data(std::string id, std::string name, std::string chatid, std::string stat)
        : user_data(id, name, stat)
    {
        // this->stat = std::move(stat);
        friendchatid = std::move(chatid);
    }
    friend_data &operator=(const friend_data &other)
    {
        if (this != &other) // check for self-assignment
        {
            user_data::operator=(other);          // call the base class assignment operator
            friendchatid = other.friend_chatid(); // assign the friend data member a value from the other object
        }
        return *this;
    }
    void set_friend_name(const std::string &name)
    {
        user_data::name = std::move(name);
    }

    void clear()
    {
        id.clear();
        name.clear();
        friendchatid.clear();
        is_new_chat = false;
    }

    bool is_new_chat = false; // 有新消息来的时候该这个字段要被更新 在查看给该用户时 更新

    std::string const &friend_id() const { return id; };
    std::string const &friend_name() const { return name; };
    std::string const &friend_chatid() const { return friendchatid; };
    std::string const &friend_stat() const { return stat; }

private:
    std::string friendchatid;
    // std::string stat;
};

struct group_data : private user_data
{

    using user_data::operator==;
    using user_data::operator!=;
    using user_data::operator>;
    using user_data::operator<;
    using user_data::operator<=;
    using user_data::operator>=;

    // using user_data::chatid;
    using user_data::id;
    using user_data::name;
    // using user_data::user_data;
    group_data() = default;
    group_data(std::string id_in, std::string name_in, std::string chatid_in, std::vector<user_data> group_members)
        : user_data(std::move(id_in), std::move(name_in)), group_members(std::move(group_members))
    {
        group_chat_id = std::move(chatid_in);
    }

    const std::string &group_id() const { return id; };
    const std::string &group_name() const { return name; };
    const std::string &group_chatid() const { return group_chat_id; };

    bool is_new_chat = false;
    void clear()
    {
        id.clear();
        name.clear();
        group_chat_id.clear();
        is_new_chat = false;
    }

    int user_level = -1;
    // bool load_group_members(Ctrl_Data_On_View &a, std::string const &id, std::vector<user_data> &target) const
    // {
    //     bool flag;
    //     a.lock();
    //     for (auto it : a.get_groups_set())
    //     {
    //         if (it.group_id() == id)
    //         {
    //             target = *it.group_members;
    //             flag = true;
    //             break;
    //         }
    //     }
    //     a.unlock();
    //     if (flag)
    //         return true;
    //     return false;
    // }; ////////////////////////////////////
    // bool load_group_members(Ctrl_Data_On_View &a) { return true; }; ////////////////////////////////////
    std::vector<user_data> group_members;

private:
    std::string group_chat_id;
};

class Window // stack 元素
{
public:
    void show() const; // show basic data
    void re_init(nlohmann::json user_cache,
                 std::string user_id,
                 std::string user_name,
                 std::vector<friend_data> friends_set,
                 std::vector<group_data> groups_set, bool is_changed)
    {
        this->user_cache = std::move(user_cache);
        this->user_id = std::move(user_id);
        this->user_name = std::move(user_name);
        this->friends_set = std::move(friends_set);
        this->groups_set = std::move(groups_set);
    };
    std::string *get_input()
    {
        // if (message != nullptr)
        // delete message;
        message = new std::string;
        // cin >> *message;
        getline(std::cin, *message);
        return message;
    };
    void flush_out(std::stringstream &stream, Ctrl_Data_On_View *Data_On_View_ptr)
    {
        this->show();
        stream.seekg(0);
        std::string line;
        while (std::getline(stream, line))
        {
            // std::string line("[2020-02-02 00:00:00] yarbor: 你是?");
            if (line.find("------------") == 0)
            {
                std::cout << line << std::endl;
                continue;
            }
            auto it = line.find(": ");
            if (it == line.npos)
            {
                std::cout << line << std::endl;
            }
            else
            {
                std::cout << line.substr(0, it + 2) << "\n\t" << line.substr(it + 2) << std::endl;
            }
        }
        std::cout << std::string('-', getTerminalWidth()) << std::flush;
        if ([&]() -> bool
            {
            bool flag = false;
            Data_On_View_ptr->lock();
            int count = 0;
            for (auto &a : Data_On_View_ptr->get_friends_set())
            {
                if (a.is_new_chat)
                {
                    std::cout << a.friend_name() << "(new chat) " << (count++ % 2 ? "\n" : "\t");
                    flag = true;
                }
            }
            for (auto &a : Data_On_View_ptr->get_groups_set())
            {
                if (a.is_new_chat)
                {
                    std::cout << a.group_name() << "(new chat) " << (count++ % 2 ? "\n" : "\t");
                    flag = true;
                }
            }
            Data_On_View_ptr->unlock();
                return flag; }())
            std::cout << std::string('-', getTerminalWidth()) << std::flush;
        if (is_wrong_lasttime)
        {
            std::cout << "|Wrong Input Last-Time! input \": help\" to get help|" << std::endl;
            is_wrong_lasttime = true;
        }
        else
            ;
        stream.clear();
    }
    /**写 ": help" 的提示清单的*/
    void mune();
    void reset()
    {
        friend_chat_now.clear();
        group_chat_now.clear();
        want_to_chat_group.clear();
        want_to_chat_friend.clear();
    };
    int getTerminalWidth() const
    {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
        return size.ws_col;
    }
    Window() = default;
    Window(Window &source, win_stat target_stat) : stat_ref(this->stat), user_id_ref(user_id),
                                                   user_name_ref(user_name),
                                                   friends_set_list_ref(this->friends_set),
                                                   groups_set_list_ref(this->groups_set)
    {
        switch (target_stat)
        {
        case ROOT:
            reset();
            break;
        case Begin:
            switch (source.stat)
            {
            case Friend_Chat:
            case Group_Chat:
            case Manage_Friend:
            case Manage_Group:
            case Manage_Self:
            case Notices:
                reset();
                break;
            default:
                assert(false);
                break;
            }
            break;
        case Friend_Chat:
            switch (source.stat)
            {
            case Begin:
            {
                this->reset();
                this->friend_chat_now = std::move(source.want_to_chat_friend);
                source.want_to_chat_friend.clear();
                break;
            }
            default:
                assert(false);
                break;
            }
            break;
        case Group_Chat:
        {
            switch (source.stat)
            {
            case Group_Chat:
            case Manage_Group:
                this->group_chat_now = source.group_chat_now;
                break;
            case Begin:
                this->group_chat_now = std::move(source.want_to_chat_group);
                source.want_to_chat_group.clear();
                break;
            default:
                assert(false);
                break;
            }
            break;
        }
        break;
        case Group_Data:
            switch (source.stat)
            {
            case Group_Chat:
            case Manage_Group:
                this->group_chat_now = source.group_chat_now;
                break;
            default:
                assert(false);
                break;
            }
            break;
        case Manage_Friend:
            switch (source.stat)
            {
            case Begin:
                this->friend_chat_now = std::move(source.want_to_chat_friend);
                source.want_to_chat_friend.clear();
                break;
            default:
                assert(false);
                break;
            }
            break;
        case Manage_Group:
            switch (source.stat)
            {
            case Begin:
                this->friend_chat_now = std::move(source.want_to_chat_friend);
                source.want_to_chat_friend.clear();

                break;
            default:
                assert(false);
                break;
            }
            break;
        case Notices:
            switch (source.stat)
            {
            case Begin:
                reset();
                break;
            default:
                assert(false);
                break;
            }
            break;
        case Manage_Self:
            switch (source.stat)
            {
            case Begin:
                reset();
                break;
            default:
                assert(false);
            }
            break;
        default:
            assert(false);
            break;
        }
    };
    Window(nlohmann::json user_cache,
           std::string user_id,
           std::string user_name,
           std::vector<friend_data> friends_set,
           std::vector<group_data> groups_set, bool is_changed)
        : stat_ref(this->stat), user_id_ref(this->user_id),
          user_name_ref(this->user_name),
          friends_set_list_ref(this->friends_set),
          groups_set_list_ref(this->groups_set)
    {
        assert(!is_init);
        is_init = true;
        this->user_cache = std::move(user_cache);
        this->user_id = std::move(user_id);
        this->user_name = std::move(user_name);
        this->friends_set = std::move(friends_set);
        this->groups_set = std::move(groups_set);
        this->is_changed = false;
    };
    bool Is_changed()
    {
        return is_changed;
    }
    // Window * next_window = nullptr;
    // Window * prev_window = nullptr;
    std::string const &user_id_ref = user_id;
    std::string const &user_name_ref = user_name;
    std::vector<friend_data> const &friends_set_list_ref = friends_set;
    std::vector<group_data> const &groups_set_list_ref = groups_set;
    win_stat const &stat_ref = stat;

    bool is_wrong_lasttime = false;
    friend_data friend_chat_now;
    group_data group_chat_now;

private:
    bool is_changed = false;
    bool is_init = false;
    win_stat stat = 4294967295U;
    static nlohmann::json user_cache;
    static std::string user_id;
    static std::string user_name;
    static std::vector<friend_data> friends_set;
    static std::vector<group_data> groups_set;
    // std::string now_group_name = "";
    // std::string now_group_id = "";

    group_data want_to_chat_group;   // 页面传递后会置空
    friend_data want_to_chat_friend; // 页面传递后会置空

    std::string *message = nullptr;
};
