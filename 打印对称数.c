#define _CRT_SECURE_NO_WARNINGS 1		
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int panduan_weishu(int i);
void dayin(int i, int n);
void dayin1(int i,int n);
//void dayin2(int i,int n);
//打印1-10000的对称数/11211.121.151.656
int main()
{
	int last;
	printf("打印 1 -- ？的对称数 （121，11211，14341）\n");
	scanf("%d", &last);
	int i = 1;
	for (; i <= last; i++)
	{
		int n = panduan_weishu(i);
		//printf("%d", n);
		dayin(i, n);
	}
	
	return 0;
}

int panduan_weishu(int last)
{
	if (last / 10 == 0)
		return 1;
	else
		return 1 + panduan_weishu(last / 10);
}
void dayin(int i, int n)
{
	if (n == 1)
		printf("%d\t", i);
	else
		dayin1(i,n);
}
void dayin1(int i,int n)
{
	char* arr;
	arr = malloc((n)*sizeof (char));
	n -= 1;
	sprintf(arr, "%d", i);
	int x = 0;
	for (;x<n;x++,n--)
	{
		if (arr[x] == arr[n]);
		else
			break;
	}
	if (n < x)
	{
		printf("%d\t", i);
	}
}