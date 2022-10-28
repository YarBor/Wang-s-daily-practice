#include"my_head.h"
int main (int argc ,char **argv)//mix cat// ./13.7.out mixcat name1 name2
{
    if(argc<4)
    {
        puts("input \"-1 / -2 \"+ source_name_1 + source_name_2 ");
    }
    FILE* a =fopen(argv[2],"r");
    FILE* b =fopen(argv[3],"r");
    if(!a&&!b){
        printf("%s and %s both can't be opened",argv[2],argv[3]);
        return 0;
    }
    else if (!a)
    {
        printf("%s can't be opened\n\n",argv[2]);
        {
            printf("%s:\n",argv[3]);
            char aa ;
            while ((aa=fgetc(b))!=EOF)
            {
                putchar(aa);
            }
            printf("\n");
            fclose(b);
        }
        return 0;
    }    
    else if (!b)
    {
        printf("%s can't be opened\n\n",argv[3]);
        {
            printf("%s:\n",argv[2]);
            char aa ;
            while ((aa=fgetc(a))!=EOF)
            {
                putchar(aa);
            }
            printf("\n");
            fclose(a);
        }
        return 0;
    }
    else 
    {
        if(0==strcmp(argv[1],"-1"))
        {
            char aa;
            while(!feof(a)&&!feof(b))
            {
                while ((aa=fgetc(a))!=EOF&&aa!='\n')
                {
                    putchar(aa);
                }
                printf("\t");
                while ((aa=fgetc(b))!=EOF&&aa!='\n')
                {
                    putchar(aa);
                }
                printf("\n");
                if(feof(a)&&!feof(b))
                {
                    while ((aa=fgetc(b))!=EOF)
                {
                    putchar(aa);
                }
                    fclose(a); fclose(b);
                    return 0;
                }
                if(!feof(a)&&feof(b))
                {
                    while ((aa=fgetc(a))!=EOF)
                {
                    putchar(aa);
                }
                    fclose(a); fclose(b);
                    return 0;
                }
            }
            fclose(a); fclose(b);
            return 0;
        }
        else if(0==strcmp(argv[1],"-2"))
        {
            char aa;
            while(!feof(a)&&!feof(b))
            {
                while ((aa=fgetc(a))!=EOF&&aa!='\n')
                {
                    putchar(aa);
                }
                printf("\n");
                while ((aa=fgetc(b))!=EOF&&aa!='\n')
                {
                    putchar(aa);
                }
                printf("\n");
                if(feof(a)&&!feof(b))
                {
                    while ((aa=fgetc(b))!=EOF)
                {
                    putchar(aa);
                }
                    fclose(a); fclose(b);
                    return 0;
                }
                if(!feof(a)&&feof(b))
                {
                    while ((aa=fgetc(a))!=EOF)
                {
                    putchar(aa);
                }
                    fclose(a); fclose(b);
                    return 0;
                }
            }
            fclose(a); fclose(b);
            return 0;
        } 
        else 
        {
            puts("input \"-1 / -2 \"+ source_name_1 + source_name_2 ");
        }
        return 0;
    }


}