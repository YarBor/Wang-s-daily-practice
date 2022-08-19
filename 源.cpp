//#define _CRT_SECURE_NO_WARNINGS 1
#include "ͷ.h"

int main()
{
	int a = 0;
	static int i = -1;
	while (1)
	{
		menu();
		scanf("%d", &a);
		if (a == out)
		{
			puts("done");
			break;
		}
		else if (a == add)
		{
			i++;
			point[i] = (struct person*)malloc(sizeof(struct person));
			add_one(point[i]);
			size_++;
		}
		else if (a == del)
		{
			puts("delete who?");
			char input[20];
			scanf("%s", &input);
			//int m = 0;
			//for (; m < size_; m++)
			//{
			//	if (strcmp(input, point[m]->name) == 0)
			//	{
			//		if (point[m + 1] == NULL)
			//		{
			//			free(point[m]);
			//			break;
			//		}
			//		memmove(point[m], point[m + 1], (size_ - m) * sizeof(struct person));
			//		free(point[size_ - 1]);
			//		puts("done.");
			//		break;
			//	}
			//}
			//if (m == size_) 
			//{
			//	puts("wrong name!");
			//	continue;
			//}	
			int x = find_(input, point, size_);
			if (x == -1)
			{
				puts("wrong name!");
				continue;
			}
			delete_one(x, &size_, point);
			i--; size_--;
		}
		else if (a == scan)
		{
			print(point, size_);
		}
		else if (a == revise)
		{
			int q = to_revise(point, size_);
			if (q == 0)
				continue;


		}
		else if (a == sort)
		{
			//qsort(point, size_, sizeof(struct person*), my_compear);
			mysort(point, size_);
			puts("done.");
		}


	}


	return 0;
}