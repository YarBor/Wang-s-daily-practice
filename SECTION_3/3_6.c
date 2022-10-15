#include "_head.h"
void _3_6()
{
    double quart, molecules;
    printf("Please enter a quart for water:");
    scanf("%lf", &quart);
    molecules = quart *  950 / 3.0e-23;
    printf("%g quart water includes %e water molecules.\n", quart, molecules);
}
