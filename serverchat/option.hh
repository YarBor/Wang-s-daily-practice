#define FUNCTION_NUMS 20

#define ACTION_TO_ERROR 999
#define ACTION_TO_FALSE 0
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_CHECK_USERNAME_PASSWORD 1
// false
#define SQL_ERROE_IN_CHECK 199
#define NOT_FIND_IN_CHECK 101 
#define WRONG_PASSWORD 102
// success
#define ANSWER_TO_CHECK_USERNAME_PASSWORD_SUCCESS 100

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_SET_USERNAME_PASSWORD 2
// false
#define SQL_ERROR_IN_SET 299
// success
#define SET_USERNAME_PASSWORD_SUCCESS 200

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_SEND_MESSAGE_TO_USER 3
// false
#define ACTION_TO_SEND_MSG_TO_USER_FALSE 301 // 被拉黑
#define SQL_ERROE_IN_SEND_MSG_TO_FRIEND 399
// success
#define ACTION_TO_SEND_MSG_TO_USER_SUCCESS 300

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_SEND_MESSAGE_TO_GROUP 4
// false
#define SQL_ERROE_IN_SEND_MSG_TO_GROUP 499
#define HAS_REMIND_IN_GRUOP 401
// success
#define ACTION_TO_SEND_MSG_TO_GROUP_SUCCESS 400

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_REQUEST_MAKE_FRIEND 5
// false
#define SQL_ERROR_IN_REQUEST_MAKE_FRIEND 599
// success
#define REQUEST_MAKE_FRIEND_SUCCESS 500
#define RECV_MAKE_FRIEND_REQUEST 501
#define RECV_ANSWER_JOIN_GROUP 502
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_ANSWER_MAKE_FRIEND 6
// false
#define SQL_ERROR_IN_ANSWER_MAKE_FRIEND 699
// success
#define ANSWER_MAKE_FRIEND_REQUEST_SUCCESS 600

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_REQUEST_JOIN_GROUP 7
// false
#define SQL_ERROR_IN_REQUEST_JOIN_GROUP 799
#define NOT_FIND_IN_JOIN_GROUP 701
// success
#define REQUEST_JOIN_GROUP_SUCCESS 700

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_ANSWER_JOIN_GROUP 8
// f
#define SQL_ERROR_IN_ANSWER_JOIN_GROUP 899
// s
#define ANSWER_JOIN_GROUP_SUCCESS 800
#define RECV_REQUEST_TO_ANSWER_JOIN_GROUP 801
#define NEW_MEMBER_JOIN_GROUP 802
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_REQUEST_RENAME_USER 9
// f
#define SQL_ERROR_IN_REQUEST_RENAME 999
// s
#define USER_RENAME_SUCCESS 900

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_REQUEST_RENAME_GROUP 10
// f
#define RE_GROUP_NAME_SQL_FALSE 1099
#define NOTIFY_MEMBER_RE_GROUP_NAME_SQL_FALSE 1099
#define RE_GROUP_NAME_SQL_FALSE_IN_NOTIFY 1099
// s
#define RE_GROUP_NAME_SUCCESS 1000

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_DOWNLOAD_USER_DATA 11
// f
#define SQL_ERROR_IN_DOWNLOAD_USER_DATA 1199
// s
#define ACTION_TO_DOWNLOAD_USER_DATA_SUCCESS 1100

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_DOWNLOAD_GROUP_DATA 12
// f
#define SQL_ERROR_IN_DOWNLOAD_GROUP_DATA 1299
// s
#define DOWNLOAD_GROUP_DATA_SUCCESS 1200

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_SEND_FILE_DATA 13
// f
#define SERVER_CONNECT_TO_RECV_FILE_ERROR 1314
#define SQL_ERROR_IN_SEND_FILE_DATA 1399 // 上传完毕 对面没收到
// s
#define FILE_UPLOAD_SUCCESS 1300

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_RECV_FILE_DATA 14
// f
#define SERVER_CONNECT_TO_RECV_FILE_ERROR 1314
#define FILE_NOT_FOUND 1401
#define SENDFILE_ERROR 1402
#define NO_SOCK_MORE 1402
// s
#define SAVEFILE_SUCCESS 1400

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_MANAGE_GROUP_MEMBER 15 // 重合了 加人 不要走这个fun 用 request/answer_join_group()
// f
#define SQL_ERROR_IN_MANAGER_GROUP_MEMBERS 1599
#define ACTION_TO_MANAGE_GROUP_MEMBER_FAILE 1581  // 非管理者请求
#define SQL_ERROR_MANAGE_JOIN_GROUP_FALSE 9999999 // 重合了 加人 不要走这个fun 用 request/answer_join_group()
#define SQL_ERROR_MANAGE_DELETE_FROM_GROUP_FALSE 1582
#define SQL_ERROR_MANAGE_MANAGER_IN_GROUP_FALSE 1583
#define SQL_ERROR_MANAGE_SETUP_MANAGER_FROM_GROUP_FALSE 1584
#define SQL_ERROR_MANAGE_DELETE_MANAGER_FROM_GROUP_FALSE 1585
// s
#define MANAGE_JOIN_GROUP_SUCCESS 9999999 // 重合了 加人 不要走这个fun 用 request/answer_join_group()
#define MANAGE_DELETE_MANAGER_FROM_GROUP_SUCCESS 1501
#define MANAGE_MANAGER_IN_GROUP_SUCCESS 1502
#define MANAGE_SETUP_MANAGER_FROM_GROUP_SUCCESS 1503
#define MANAGE_DELETE_FROM_GROUP_SUCCESS 1504

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_REMIND_USER_MEMBER 16

#define SQL_ERROR_REMIND_USER_FALSE 1699
#define REMIND_USER_SUCCESS 1600

///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_REMIND_GROUP_MEMBER 17 // undone
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_DEAD_NOTIFICATION 18
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_MAKE_GROUP 19
#define MAKE_GROUP_FALSE 1989
#define MAKE_GROUP_SUCCESS 1900
///////////////////////////////////////////////////////////////////////////////////////////////////////
#define ACTION_TO_INIT_DATA 20
#define ACTION_TO_INIT_DATA_SUCCESS 2000
#define ACTION_TO_INIT_DATA_FALSE 2089

// no server function

// #define ACTION_TO_JOIN_GROUP_FALSE 102

// FILE DO --

// #define SET_GROUP_NAME_ERROR 303
// #define SET_USER_NAME_ERROR 304
// #define ANSWER_TO_SET_USERNAME_PASSWORD 401
// #define ANSWER_TO_SEND_MESSAGE_TO_USER 402
// #define ANSWER_TO_SEND_MESSAGE_TO_GROUP 403
// #define ANSWER_TO_REQUEST_MAKE_FRIEND 404
// #define ANSWER_TO_REQUEST_JOIN_GROUP 406
// #define ANSWER_TO_REQUEST_RENAME_USER 408
// #define ANSWER_TO_REQUEST_RENAME_GROUP 409
// #define ANSWER_TO_DOWNLOAD_USER_DATA 410
// #define ANSWER_TO_DOWNLOAD_GROUP_DATA 411
// #define ANSWER_TO_SEND_FILE_DATA 412
// #define ANSWER_TO_RECV_FILE_DATA 413
// #define ANSWER_TO_MANAGE_GROUP_MEMBER 414
// #define ANSWER_TO_REMIND_USER_MEMBER 415
// #define ANSWER_TO_REMIND_GROUP_MEMBER 416

// client wrong do
// in menage member
// to manager

// to member
// #define JOIN_GROUP_SUCCESS 800
// #define KICKED_OUT_FROM_GROUP 801

// react
// #define RE_GROUP_NAME_SUCCESS 900
// #define RE_GROUP_NAME_FALSE 901

// download group data
// #define DOWNLOAD_GROUP_DATA_FALSE 1001

// remind user
// to target
// #define BE_IGNORED_FROM_USER 1111

// make group

// client fun
// return flag
#define ACTION_TO_RECV_USER_MESSAGE 99
#define ACTION_TO_RECV_GROUP_MESSAGE 98
#define NOTIFY_FRIEND_NEW_NAME 96
#define ANSWER_TO_CHECK_USERNAME_PASSWORD 95
#define ANSWER_TO_ANSWER_MAKE_FRIEND 94
#define ANSWER_TO_ANSWER_JOIN_GROUP 93
#define FRIEND_SIGN_OUT 92
#define ANSWER_TO_REQUEST_JOIN_GROUP 91
#define NOTIFY_GROUP_NEW_NAME 90
#define ANSWER_TO_REQUEST_MAKE_FRIEND 89
/// view to ctrl
/// 包的转发
#define SEND_USER_MESSAGE_TO_SERVER ACTION_TO_SEND_MESSAGE_TO_USER
#define SEND_GROUP_MESSAGE_TO_SERVER ACTION_TO_SEND_MESSAGE_TO_GROUP
#define CHECK_USERNAME_PASSWORD_TO_SERVER ACTION_TO_CHECK_USERNAME_PASSWORD
#define SET_USERNAME_PASSWORD_TO_SERVER ACTION_TO_SET_USERNAME_PASSWORD
/// ctrl to view
#define FLUSH_WINDOW 2
#define DATA_UPDATE 3