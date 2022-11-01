#include <stdio.h>
void swap(int *a, int *b)
{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}
void maopao(int *arr, int size)
{
    for (int i = 0; i < size; i++)
    {
        for (int a = 0; a < size - i - 1; a++)
        {
            if (arr[a] < arr[a + 1])
                swap(arr + a, arr + a + 1);
        }
    }
}
int main(){
    int arr[]={1,2,3,5,6,8,4,2,3,6,9,3};
    maopao(arr,12);
    for (size_t i = 0; i < 12; i++)
    {
        printf("%d",arr[i]);
        /* code */
    }
    
}