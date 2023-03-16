#define _CRT_SECURE_NO_WARNINGS 1
#include <stdio.h>
//??ŵ??
int han_nuo(int n) //?ⶨ??n??ת?Ƶ?c??????ϸ????
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
#define ab "??A??B"
#define ac "??A??C"
#define bc "??B??C"
#define ca "??C??A"
#define cb "??C??B"
#define ba "??B??A"
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
void bu_zhou_a(int k)//??ӡn??n-1??ʱ?Ĳ???
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
void bu_zhou0(int n)//N ????2ʱ ??һ?????ƶ?????
{
	printf("ab ");
	printf("ac ");
	printf("bc ");
}
int main()
{
	int n;//??????n??
	printf("????__??Բ???ں?ŵ???ϣ?\n");
	scanf("%d", &n);
	int m = han_nuo(n);
	printf("%d?㺺ŵ????Ҫ%d??\n", n, m);
	printf("????????Ϊa b c ??3?????ӣ???%d??????a???ϣ??ȴ??ƶ?\n", n);
	printf("????????ϸ????");
	bu_zhou_a(n);
	return 0;
}
