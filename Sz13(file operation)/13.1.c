#include"my_head.h"
int main(int argc,char** argv)
{
    int ch;
    FILE *file;
    unsigned long count = 0;
    if(argc!=2)
    {   
        printf("Usage: %s filename\n",argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((file=fopen(argv[1],"r"))==NULL)
    {
        printf("Can't open %s\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    while((ch=getc(file))!=EOF)
    {
        putc(ch, stdout);
        count++;
    }
    fclose(file);
    printf("file %s has %lu charaters\n",argv[1],count);
    return 0;
}