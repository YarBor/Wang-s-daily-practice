#include"my_head.h"
int main(int argc,char** argv)
{
    int i = 1;
    while(i<argc)
    {
        FILE * a = fopen(argv[i],"r");
        if (!a) 
        {
            printf("%s can't open\n",argv[i]);
            break;
        }
        else 
        {
            printf("%s:\n",argv[i]);
            char aa ;
            while ((aa=fgetc(a))!=EOF)
            {
                putchar(aa);
            }
            printf("\n");
            fclose(a);
        }
        i++;
        printf("\n");
    }
}