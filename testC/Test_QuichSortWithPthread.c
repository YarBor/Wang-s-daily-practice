#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_NUM_THREADS 4
#define THRESHOLD 100

int arr[1000];

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int left, int right) {
    int pivot = arr[right];
    int i = left - 1;
    for (int j = left; j < right; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[right]);
    return i + 1;
}

typedef struct {
    int left;
    int right;
} thread_args;

void* quicksort_thread(void* args) {
    thread_args* targs = (thread_args*)args;
    int left = targs->left;
    int right = targs->right;
    if (right - left + 1 <= THRESHOLD) {
        // 当数组长度小于等于阈值时，使用单线程进行排序
        for (int i = left; i <= right; i++) {
            for (int j = i + 1; j <= right; j++) {
                if (arr[i] > arr[j]) {
                    swap(&arr[i], &arr[j]);
                }
            }
        }
    } else {
        // 当数组长度大于阈值时，递归地进行排序
        int pi = partition(left, right);
        pthread_t left_thread, right_thread;
        thread_args left_args = {left, pi - 1};
        thread_args right_args = {pi + 1, right};
        pthread_create(&left_thread, NULL, quicksort_thread, (void*)&left_args);
        pthread_create(&right_thread, NULL, quicksort_thread, (void*)&right_args);
        pthread_join(left_thread, NULL);
        pthread_join(right_thread, NULL);
    }
    pthread_exit(NULL);
}

int main() {
    // 生成一个包含1000个随机元素的数组
    for (int i = 0; i < 1000; i++) {
        arr[i] = rand() % 1000;
    }

    // 创建并执行排序线程
    pthread_t root_thread;
    thread_args root_args = {0, 999};
    pthread_create(&root_thread, NULL, quicksort_thread, (void*)&root_args);
    pthread_join(root_thread, NULL);

    // 打印排序后的数组
    // for (int i = 0; i < 1000; i++) {
        // printf("%d ", arr[i]);
    // }
    // printf("\n");

    return 0;
}
