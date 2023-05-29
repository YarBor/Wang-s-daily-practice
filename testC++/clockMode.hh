#include <arpa/inet.h>
#include <assert.h>
#include <time.h>

#include <algorithm>
#include <chrono>
#include <functional>
#include <queue>
#include <unordered_map>

using TimeoutCallBack = std::function<void()>;
using Clock = std::chrono::high_resolution_clock;  // ns级时钟
using MS = std::chrono::milliseconds;
using TimeStamp = Clock::time_point;  // 具体时间

// 定时器节点
struct TimerNode {
  int id;
  TimeStamp expires;
  TimeoutCallBack cb;
  bool operator<(const TimerNode &t) { return expires < t.expires; }
};

// 定时器管理类
class HeapTimer {
 public:
  HeapTimer() { heap_.reserve(64); }

  ~HeapTimer() { clear(); }

  void adjust(int id, int newExpires);

  void add(int id, int timeOut, const TimeoutCallBack &cb);

  void doWork(int id);

  void clear();

  void tick();

  void pop();

  int GetNextTick();

 private:
  void del_(size_t i);

  void siftup_(size_t i);

  bool siftdown_(size_t index, size_t n);

  void SwapNode_(size_t i, size_t j);

 private:
  std::vector<TimerNode> heap_;

  std::unordered_map<int, size_t> ref_;  // key：节点id, value：数组索引
};

#include <sys/fcntl.h>
#include <time.h>

#include <cstring>
#include <memory>
#include <string>

class clock_timer_node {
  clock_timer_node(time_t s, long ns) {
    operator_time.tv_sec = s;
    operator_time.tv_nsec = ns;
    clock_gettime(CLOCK_REALTIME, &Next_time);
    Next_time.tv_nsec += operator_time.tv_nsec;
    Next_time.tv_sec += operator_time.tv_sec;
    


  }
  std::string job_name = "";
  timespec Next_time;
  std::function<void()> time_callback;
  timespec operator_time;
  bool operator<(clock_timer_node const &other) {
    return this->Next_time.tv_sec < other.Next_time.tv_sec ||
           this->Next_time.tv_nsec < other.Next_time.tv_nsec;
  }
};

void clock_signal_handler(int signum) {}

class clock_timer {
  friend void clock_signal_handler(int signum);
  clock_timer() { timeHeap.reserve(64); }

  void Init();
  std::vector<std::shared_ptr<clock_timer_node>> timeHeap;

  int socket_write = -1;
  int socket_read = -1;
};

void clock_timer::Init() {
  this->socket_write = socket(AF_INET, SOCK_STREAM, 0);
  this->socket_read = socket(AF_INET, SOCK_STREAM, 0);

  sockaddr_in write_addr, read_addr;

  memset(&write_addr, 0, sizeof(write_addr));
  memset(&read_addr, 0, sizeof(read_addr));

  read_addr.sin_family = AF_INET;
  read_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 绑定到回环地址
  read_addr.sin_port = 0;

  write_addr.sin_family = AF_INET;
  write_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);  // 绑定到回环地址
  write_addr.sin_port = 0;

  bind(socket_read, (sockaddr *)&read_addr, sizeof(sockaddr_in));
  bind(socket_write, (sockaddr *)&write_addr, sizeof(sockaddr_in));

  listen(socket_read, 1);

  int flags_read = fcntl(socket_read, F_GETFL, 0);
  fcntl(socket_read, F_SETFL, flags_read | O_NONBLOCK);

  int flags_write = fcntl(socket_write, F_GETFL, 0);
  fcntl(socket_write, F_SETFL, flags_write | O_NONBLOCK);

  bool is_connected = false;
  bool is_listened = false;
  while (1) {
    if (is_connected && is_listened) break;
    if (!is_connected) {
      if (connect(socket_write, (struct sockaddr *)&read_addr,
                  sizeof(struct sockaddr_in)) != -1)
        is_connected = true;
    }
    if (!is_listened) {
      int a = -1;
      if ((a = accept(socket_read, 0, 0)) != -1) {
        close(socket_read);
        socket_read = a;
        is_listened = true;
      }
    }
  }
}