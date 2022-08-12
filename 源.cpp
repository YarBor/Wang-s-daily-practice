#define _CRT_SECURE_NO_WARNINGS 1		
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void px_compare(char*arr[]);
char* fget(char* input, int n);
void cd_compare(char* arr[]);
int main()
{
	char pe[5][11];
	char* arr[5];
	for (int i = 0; i <= 4; i++)
	{
		arr[i] = fget(pe[i], 10);
		if (arr[i][0] == EOF || arr[i] == "EOF" || arr[i] == "eof")
			break;
	}
	int b;
	scanf("%d", &b);
	if (b == 1)
		for (int i = 0; i <= 4; i++)
		{
			fputs(arr[i], stdout);
			printf("\n");
		}
	else if (b == 2)
		px_compare(arr);
	else if (b == 3)
		cd_compare(arr);
	return 0;
} 
void px_compare(char*arr[])
{
	int n = 3;
	while (n >= 0)
	{
		for (int i = 0; i <= n; i++)
		{
			int e = strcmp(*(arr + i),*(arr + i + 1));//±È½Ï×Ö·û´®
			if (e == 1)
			{
				char (*tep);
				tep = *(arr + i);
				*(arr + i) = *(arr + i + 1);
				*(arr + i + 1) = tep;
			}
		}
		n--;
	}
	for (int i = 0; i <= 4; i++)
	{
		fputs(arr[i], stdout);
		printf("\n");
	}
}
char* fget(char* input, int n)
{
	fgets(input, n+1, stdin);
	int a = 0;
	for (; a <= 9; a++)
	{
		if (input[a] == '\n')
		{
			input[a] = '\0';
			return input;
		}
	}
	while (getchar() != '\n')
	{
		continue;
	}
	return input;
}
void cd_compare(char* arr[])
{
	

}


//#include <stdio.h>
//int main()
//{
//	for (int a = 0; a < 10; a++)
//	{
//		for (int b = 10; b < 9; b++)
//			printf("%d%d\n", a, b);
//		printf("\n");
//	}
//	return 0;
//}