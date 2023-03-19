//#define _CRT_SECURE_NO_WARNINGS 1		
#include "ͷ.h"

void print(struct person* point[], int size_)
{
	for (int a = 0; a < size_; a++) {
		printf("the %d\n", a + 1);
		printf("name:%s\n", point[a]->name);
		printf("sex:%s\n", point[a]->sex);
		printf("number:%s\n", point[a]->phone);
		puts("\n");
	}

}
void menu()
{
	printf("**********ͨѶ¼*********\n");
	printf("*****1.add     2.delete**\n");
	printf("*****3.scan    4.revise**\n");
	printf("*****5.sort    0.out****\n");
	printf("*************************\n");
	puts("??????>:");
}
void add_one(struct person* point)
{
	puts("name:");
	scanf("%s", &point->name);
	puts("sex:");
	scanf("%s", &point->sex);
	puts("the phone-number:");
	scanf("%s", &point->phone);
	puts("alright!");
}
int find_(char* a, struct person** point, int size_)
{
	//char* b;
	//int i = 0;
	//b = point[i].name;
	//for (; 0 != strcmp(a, b); b = point[i++].name)
	//	if (point[i].name == NULL)
	//		return -1;
	//return i;
	int i = 0;
	int t;
	while (i < size_)
	{
		t = strcmp(a, point[i]->name);
		if (t == 0)
			return i;
		i++;

	}
	return -1;
}
void delete_one(int x, int* size_, struct person** point)
{
	if (x == (*size_ - 1))
	{
		free(point[x]);
		point[x] = NULL;
		//		*size_--;
		puts("done.");

	}
	else
	{
		//free(point[x]);
		memmove((void*)point[x], (void*)point[x + 1], (*size_ - 1 - x) * sizeof(struct person));
		free(point[*size_ - 1]);
		point[*size_ - 1] = NULL;
		puts("done.");
		//		*size_ --;
	}
}
void mysort(struct person** point, int size_)
{
	for (; size_ > 1; size_--)
	{
		int g = 0;
		for (int i = 0; i < size_ - 1; i++)
		{

			int v = strcmp(point[i]->name, point[i + 1]->name);
			if (v > 0)
			{
				struct person* arr;
				arr = point[i];
				point[i] = point[i + 1];
				point[i + 1] = arr;
				g++;
			}
		}
		if (g == 0)
			break;
	}
}
int to_revise(struct person** point, int size_)
{
	puts("who?");
	char* a = (char*)malloc(20 * sizeof(char));
	int i = 0;
	while (1)
	{
		scanf("%s", a);
		if (strcmp(a, "999") == 0)
			return 0;

		for (; strcmp(a, point[i]->name) != 0 && point[i] != NULL; i++)
			if (i == size_ - 1)
			{
				printf("%s", "wrong name! again please(number 999 to quit)");
			}
			else break;
	}
	while (1)
	{
		puts("change what? (number/sex)\n:>");
		*a = 0;
		scanf("%s", &a);
		if (strcmp(a, "number") == 0)
		{
			*a = 0;
			puts("to what?");
			scanf("%s", &a);
			*(point[i]->phone) = *a;
			break;
		}
		if (strcmp(a, "sex") == 0)
		{
			*a = 0;
			puts("to what?");
			scanf("%s", &a);
			*(point[i]->sex) = *a;
			break;
		}
		else continue;
	}
	puts("done!");
}




