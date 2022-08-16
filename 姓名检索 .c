#define _CRT_SECURE_NO_WARNINGS 1		
#include<stdio.h>
#include <string.h>
//#define renshu 4;
int main()
{
	char arr[][64] = { {"张三"} ,{"李四"}, {"王五"}, {"谁谁谁"} };
	char shuru[] = "张三李四 HHH";//输入的名字 因为不会输入汉字 假定这是输入
	for (int i = 0; i<=(sizeof(arr)/sizeof(arr[0])-1); i++)
	{
		if (strstr(shuru, arr[i]) == NULL)
			puts(arr[i]);
	}
	return 0;
}