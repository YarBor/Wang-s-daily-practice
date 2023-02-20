#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000

// 交换函数
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// 分治函数
int partition(int arr[], int low, int high) {
    int pivot = arr[low];
    while (low < high) {
        while (low < high && arr[high] >= pivot) {
            high--;
        }
        arr[low] = arr[high];
        while (low < high && arr[low] <= pivot) {
            low++;
        }
        arr[high] = arr[low];
    }
    arr[low] = pivot;
    return low;
}

// 快排函数
void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

// 打印数组
void printArray(int arr[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

int main() {
    int arr[N];
    srand((unsigned)time(NULL)); // 生成随机种子
    for (int i = 0; i < N; i++) {
        arr[i] = rand() % 1000; // 生成1000个0-999的随机数
    }
    // printf("Original array: ");
    // printArray(arr, N);
    quickSort(arr, 0, N - 1);
    // printf("Sorted array: ");
    // printArray(arr, N);
    return 0;
}
