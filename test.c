#include<stdio.h>
int main(int argc,char**argv,char ** envg)
{
    if(argc==1) printf("我嫩爹");
    printf("%d\n",argc);//
    for(int n=0;argv[n]!=NULL;n++)
    puts(argv[n]);
}