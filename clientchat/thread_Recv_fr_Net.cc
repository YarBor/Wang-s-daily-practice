#include "client.hh"

extern user_sk *u_sk_ptr; // 进行 user_sock 的读操作
Work_Queue *Ctrl_Work_Queue = nullptr;
// Work_Queue *queue_btw_Ctrl_View_ptr = nullptr;

void Thread_Recv_fr_Net_begin_fun()
{
    while (1)
    {

        std::string *message = new std::string();
        size_t mode = -1;
        job_in_Work_Queue *j = new job_in_Work_Queue(mode, message);
        // j.timesptamp = (char *)malloc(sizeof(char) * 28);
        j->timestamp = new char[strlen("0000-00-00 00:00:00.000000") + 1];

        j->timestamp[27] = '\0';

        u_sk_ptr->recv_message(*message, mode, j->timestamp);
        j->self = j;

        Ctrl_Work_Queue->submit(j);
    }
}