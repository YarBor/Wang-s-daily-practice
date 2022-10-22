#include"my_ls.h"
ino_t get_ino_ofname(char * filename)
{
    struct stat file_stat;
    assert( 0 == stat(filename,&file_stat));
    return file_stat.st_ino;
}
char * find_name_from_INO(ino_t ino)
{
    DIR *dp = NULL;
    struct dirent * dptr = NULL;
    char *filename = NULL;
    assert(dp=opendir("."));
    while(dptr=readdir(dp)){
        if(dptr->d_ino==ino)    
        {
            filename=strdup(dptr->d_name);
            break;
        }
    }
    return filename ;
}
int main(int argc,char **argv)
{
    char* nameof_this[256];
    unsigned current_depth = 0;
    while(1){
        ino_t ino_now =get_ino_ofname(".");
        ino_t ino_father = get_ino_ofname("..");
        if(ino_father==ino_now) break;
        else{
            chdir("..");
            nameof_this[current_depth++]=find_name_from_INO(ino_now);
        //    chdir(nameof_this);
        }
        
    }
    for(int i = current_depth-1;i>=0;i--)
        {
            nameof_this[i];
            free(nameof_this[i]);
        }
    
}