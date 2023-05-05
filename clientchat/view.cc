#include "view.hh"
// extern std::mutex friend_list_lock;
// extern std::mutex gruop_list_lock;

// extern std::mutex open_file_lock;
// extern std::mutex Ctrl_flie_name_lock;
// extern std::string viewer_opening_chatid; // 全局的 唯一的
extern Ctrl_Data_On_View Data_On_View;
// extern Ctrl_Data_On_Ctrl Data_On_Ctrl;

void view_begin(const Window *page)
{
    clear_window;
    std::cout << "User ID: " << page->user_id_ref << std::endl;
    std::cout << "User Name: " << page->user_name_ref << std::endl;

    std::cout << "Friends:" << std::endl;
    int count = 1;
    for (const auto &friend_data : page->friends_set_list_ref)
    {
        std::cout << "  F" << count++ << " " << friend_data.friend_name() << " (" << friend_data.friend_id() << ")" << (friend_data.is_new_chat ? "(New Message)" : "") << std::endl;
    }
    count = 1;
    std::cout << "Groups:" << std::endl;
    for (const auto &group_data : page->groups_set_list_ref)
    {
        std::cout << "  G" << count++ << " " << group_data.group_name() << " (" << group_data.group_id() << ")" << (group_data.is_new_chat ? "(New Message)" : "") << std::endl;
    }
}
void view_chat_friend(const Window *page)
{
    // self:> name (id)
    // friend:> name (id)
    //  ----------
    clear_window;

    std::cout << "self:> " << page->user_name_ref << "(" << page->user_id_ref << ")" << std::endl;
    std::cout << "friend:> " << page->friend_chat_now.friend_name() << "(" << page->friend_chat_now.friend_id() << ")" << std::endl;
    std::cout << std::string('-', page->getTerminalWidth()) << std::flush;
}
void view_chat_group(const Window *page)
{
    clear_window;
    std::cout << "self:> " << page->user_name_ref << "(" << page->user_id_ref << ")" << std::endl;
    std::cout << "group:> " << page->group_chat_now.group_name() << "(" << page->group_chat_now.group_id() << ")" << std::endl;
    std::cout << "enter \": show member\" to view members;" << std::endl;
    std::cout << "enter \": manage member\" to manage members(if u are manager/owner);" << std::endl;
    std::cout << std::string('-', page->getTerminalWidth()) << std::flush;
}
void view_show_group_data(const Window *page)
{
    clear_window;
    std::cout << "group:> " << page->group_chat_now.group_name() << "(" << page->group_chat_now.group_id() << ")" << std::endl;

    // *(std::vector<user_data> **)((void *)&(page->group_chat_now.group_members)) = new std::vector<user_data>;
    // const_cast<std::vector<user_data> *>(page->group_chat_now.group_members) = new std::vector<user_data>;
    std::vector<user_data> tmp_vec_user_data;
    // if (page->group_chat_now.load_group_members(Data_On_View, page->group_chat_now.group_id(), tmp_vec_user_data) == false)
        // std::cout << "loading data false " << std::endl;
    int count = 0;
    for (auto & i : page->group_chat_now.group_members)
    {
        std::cout << i.name << "(" << i.id << ")" << (count++ % 2 ? "\n" : "\t");
    }
    std::cout << std::string('-', page->getTerminalWidth()) << std::flush;

}

void view_manage_friend(const Window *page)
{
    clear_window;
}
void view_manage_group(const Window *page)
{
    clear_window;
}
void view_manage_self(const Window *page)
{
    clear_window;
}
void view_notices(const Window *page)
{
    clear_window;
}

void Window::show() const
{
    switch (this->stat)
    {
    case Begin:
        view_begin(this);
        break;
    case Friend_Chat:
        view_chat_friend(this);
        break;
    case Group_Chat:
        view_chat_group(this);
        break;
    case Group_Data:
        view_show_group_data(this);
        break;
    case Manage_Friend:
        view_manage_friend(this);
        break;
    case Manage_Group:
        view_manage_group(this);
        break;
    case Manage_Self:
        view_manage_self(this);
        break;
    case Notices:
        view_notices(this);
        break;
    default:
        assert(false);
        break;
    }
}
