#include"_head.h"
void _3_4()
{
    float number;

    printf("Enter a float-point value:");
    scanf("%f", &number);
    printf("fixed-point notation: %f\n", number);
    printf("exponential notation: %e\n", number);//指数类型
    printf("p notation: %a\n", number);//八进制类型
}