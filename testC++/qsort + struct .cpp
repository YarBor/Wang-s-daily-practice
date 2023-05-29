#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct stu
{
    char name[20];
    char num[50];
} xue_sheng;

xue_sheng wang = {"wangwu", "1231233131"};
xue_sheng zhang = {"zhangsan", "321321321321"};
//ÅÅÐò
int cmp(const void *a, const void *b)
{
    return strcmp((((xue_sheng *)a)->name), (((xue_sheng *)b)->name));
}
int cmp1(const void *a, const void *b)
{
    return strcmp((((xue_sheng *)a)->num), (((xue_sheng *)b)->num));
}
xue_sheng yiban[2] = {wang, zhang};
int main()
{
    qsort(yiban, 2, sizeof(yiban[0]), cmp);
    printf("%s \n%s\n ", yiban->name, yiban->num);
    //	qsort(yiban, 2, sizeof(yiban[0]), cmp1);
    printf("%s \n%s\n ", (yiban + 1)->name, (yiban + 1)->num);
    return 0;
}