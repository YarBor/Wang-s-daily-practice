#include"my_head.h"
int main(int argc,char**argv)
{
    if (argc<4){
        puts("input \"cp 'source name' 'target name' ");
    return 0;
    }

    if (0==strcmp("--help",argv[1]))
    {
        puts("input \"cp 'source name' 'target name' \"\n !: source name must already existed  ");
        return 0;
    }
    FILE * source = fopen(argv[2],"rb");
    if(!source)
    {
        puts("wrong name of source ;\nintput \" --help\" to firgue out");
        return 0;
    }
    FILE * target = fopen(argv[3],"wb+");
    while(!feof(source))
    {
        char arr[10];
        fread(arr,sizeof(arr),1,source);
        fwrite(arr,sizeof(arr),1,target);
        fflush(target);
    }
    fclose(target);
    fclose(source);
    return 0;
}