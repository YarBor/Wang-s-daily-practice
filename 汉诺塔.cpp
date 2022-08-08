#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
//汉诺塔
int han_nuo(int n) //测定第n层转移到c柱的详细步数
{
	int k;
	if (n <= 2)
	{
		return 3;
	}
	else
	{
		n = han_nuo(n - 1) * 2 + 1;
	}
	return n;
}
#define ab "从A到B"
#define ac "从A到C"
#define bc "从B到C"
#define ca "从C到A"
#define cb "从C到B"
#define ba "从B到A"
void bu_zhou_ab(int k);
void bu_zhou_ac(int k);
void bu_zhou_bc(int k);
void bu_zhou_ba(int k);
void bu_zhou_ca(int k);
void bu_zhou_cb(int k);
void bu_zhou_a(int k);

void bu_zhou0(int k);
void bu_zhou_ca(int k)
{
	if (k != 2) {
		bu_zhou_cb(k - 1);
		printf("ca ");
		bu_zhou_ba(k - 1);
	}
	else
	{
		printf("ac ");
		printf("ab ");
		printf("cb ");
	}

}
void bu_zhou_cb(int k)
{
	if (k != 2) {
		bu_zhou_ca(k - 1);
		printf("cb ");
		bu_zhou_ab(k - 1);
	}
	else
	{
		printf("ca ");
		printf("cb ");
		printf("ab ");
	}

}
void bu_zhou_ba(int k)
{
	if (k != 2) {
		bu_zhou_bc(k - 1);
		printf("ba ");
		bu_zhou_ca(k - 1);
	}
	else
	{
		printf("bc ");
		printf("ba ");
		printf("ca ");
	}

}
void bu_zhou_ac(int k)
{
	if (k != 2) {
		bu_zhou_ab(k - 1);
		printf("ac ");
		bu_zhou_bc(k - 1);
	}
	else
	{
		printf("ab ");
		printf("ac ");
		printf("bc ");
	}

}
void bu_zhou_bc(int k)
{
	if (k != 2) {
		bu_zhou_ba(k - 1);
		printf("bc ");
		bu_zhou_ac(k - 1);
	}
	else
	{
		printf("ba ");
		printf("bc ");
		printf("ac ");
	}

}
void bu_zhou_ab(int k)
{
	if (k != 2) {
		bu_zhou_ac(k - 1);
		printf("ab ");
		bu_zhou_cb(k - 1);
	}
	else
	{
		printf("ac ");
		printf("ab ");
		printf("cb ");
	}

}
void bu_zhou_a(int k)//打印n到n-1层时的步骤
{
	if (k == 2)
	{
		bu_zhou0(k);
	}
	else
	{
		bu_zhou_ab(k - 1);
		printf("ac ");
		bu_zhou_bc(k - 1);
	}
}
void bu_zhou0(int n)//N 等于2时 第一二层移动步骤
{
	printf("ab ");
	printf("ac ");
	printf("bc ");
}
int main()
{
	int n;//假设有n层
	printf("共有__个圆盘在汉诺塔上？\n");
	scanf("%d", &n);
	int m = han_nuo(n);
	printf("%d层汉诺塔需要%d步\n", n, m);
	printf("假设有名为a b c 的3个柱子，有%d个盘在a柱上，等待移动\n", n);
	printf("下面是详细步骤");
	bu_zhou_a(n);
	return 0;
}