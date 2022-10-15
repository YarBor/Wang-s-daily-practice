#include"head.h"
// typedef struct strbuf {
//   int len;     //当前缓冲区（字符串）长度
//   int alloc;   //当前缓冲区（字符串）容量
//   char *buf;   //缓冲区（字符串）
// }strbuf;
void strbuf_init(struct strbuf *sb, size_t alloc){
    if (alloc==0)
    {
    sb->buf=NULL;
    sb->alloc=alloc;
    sb->len=0;
    }
    
    sb->buf=(char *)malloc(sizeof(char )*alloc);
    sb->alloc=alloc;
    sb->len=0;
    return ;
}
void strbuf_attach(strbuf *sb ,void *str ,size_t len ,size_t alloc)//????
{
    //alloc=len;
    if(len==0) return;
    strbuf_init(sb,alloc);
    sb->alloc=alloc;
    sb->len=len;
    for (int i = 0; i < (sb->len+1); i++)
    {
        sb->buf[i]=((char*)str)[i];
    }
}
void strbuf_release(struct strbuf *sb)
{
    free(sb->buf);
    free(sb);
    return;
}
int swap(int *a,int*b)
{
    if(*a==*b)
    return 0;
    else{
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
    }
    return 0;
}
void strbuf_swap(strbuf*a,strbuf*b)
{
    char * c =a->buf;
    a->buf=b->buf;
    b->buf=c;
    swap(&a->alloc,&b->alloc);
    swap(&b->len,&a->len);
    return ;
}
char *strbuf_detach(struct strbuf * const sb, size_t *sz)//返回的字符串是malloc的//?会改变sb的地址?
{
    *sz= sb->len;
    char*a = NULL;
    a = (char* )malloc(sizeof(char)*(sb->len+1));
    for (size_t i = 0; i < (sb->len+1); i++)
    {
        a[i]=sb->buf[i];
    }
    
    return a;
}
ssize_t strbuf_read(struct strbuf* sb, int fd, size_t hint)//hint,0x1000,直接给他扩容
{
   
    sb->alloc += 0x1000;
    sb->buf = (char*)realloc(sb->buf, sizeof(char) * (sb->alloc ));
    FILE* pf = fdopen(fd, "r");
    int flag = 0;
    char ch;
    ssize_t cnt = 0;
    while ((ch=fgetc(pf)) != EOF)
    {
        flag = 1;
        sb->buf[sb->len++] = ch;
        cnt++;
    }
    if(flag==1)
    sb->buf[sb->len] = '\0';
    return cnt;

}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    int a = strcmp(first->buf,second->buf);
    if(a==0 && first->alloc==second->alloc) return 1;//true 
    else return 0;//false
}
void strbuf_reset(struct strbuf *sb){
    free(sb->buf);
    sb->alloc=0;
    sb->len=0;
}
int strbuf_grow(strbuf* sb,size_t extra)
{
    if(sb->alloc-sb->len-1 > extra)
    return sb->alloc;
    else 
    {
        sb->buf=realloc(sb->buf,(sb->alloc+extra)*sizeof(char));
        sb->alloc=(sb->alloc+extra);
    }
    return sb->alloc;
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    sb->alloc=strbuf_grow(sb,len);
    memcpy(sb->buf+sb->len,data,len+1);
    sb->len=sb->len+len;
    sb->buf[sb->len]='\0';
    return ;

}
void strbuf_addch(strbuf *sb , char c)
{
    sb->alloc=strbuf_grow(sb,1);
    memcpy(sb->buf+sb->len,&c,1);
    sb->len=sb->len+1;
    sb->buf[sb->len]='\0';
    return ;
}
void strbuf_addstr(strbuf * sb,const char* s)
{
    int s_len =strlen(s);
    sb->alloc=strbuf_grow(sb,s_len);
    memcpy(sb->buf+sb->len,s,s_len);
    sb->len=sb->len+s_len;
    sb->buf[sb->len]='\0';

}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_addstr(sb,sb2->buf);
    return ;
}
void strbuf_setlen(strbuf *sb,size_t len)
{
    if(sb->alloc<len+1)
        sb->alloc = strbuf_grow(sb,len);
    if(sb->len>len){
        memset(sb->buf+len,0,sb->alloc-len);
        sb->len=len;
        return;
    }
    else{
        memset(sb->buf+sb->len,0,len-sb->len+1);
        sb->len=len;
    }
    return;
}
size_t strbuf_avail(const strbuf  *sb )
{
    return (sb->alloc-sb->len-1);
}
void strbuf_insert(strbuf * sb, size_t pos,const void* data,size_t len)
{
    // assert(sb->len<pos);
    // if((sb->buf+sb->alloc)-(sb->buf+pos) < len+1)
    // {
    //     sb->buf=realloc(sb->buf,(sb->alloc+len+1)*sizeof(char));
    //     sb->alloc=(sb->alloc+len+1);
    // }
    // memcmp(sb->buf+pos,data,len);
    // if((sb->buf+len)-(sb->buf+pos)<len) 
    // {
    //     sb->len=pos+len;
    //     sb->buf[sb->len]='\0';
    // }
    // return;
    if(pos>sb->len)
    {
        sb->alloc = strbuf_grow(sb,pos-sb->len+len+1);
        memcpy(sb->buf+pos,data,len);
        sb->len=pos+len;
        sb->buf[sb->len]='\0';

    }
    else{
        sb->alloc = strbuf_grow(sb,sb->len+len+1);
        memmove(sb->buf+pos+len,sb->buf+pos,len);
        memcpy(sb->buf+pos,data,len);
        sb->len=sb->len+len;
        sb->buf[sb->len]='\0';
    }
}
void strbuf_ltrim(strbuf *sb)	                       
{
    int i =0;
    for(; sb->buf[i]==' '||sb->buf[i]=='\t'||sb->buf[i]=='\n';i++);
    memmove(sb->buf,sb->buf+i+1,sb->len-i-1);
    sb->len=sb->len-i-1;
    memset(sb->buf+sb->len,0,sb->alloc-sb->len);
}
void strbuf_rtrim(strbuf *sb)	
{
    int i = sb->len-1;
    for(;sb->buf[i]==' '||sb->buf[i]=='\t'||sb->buf[i]=='\n';i--);
    sb->len=i+1;
    memset(sb->buf+sb->len,0,sb->alloc-sb->len);
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){
    if(len>0)
    {    
        assert(pos+len>sb->alloc);
        if(pos+len>sb->len)
        sb->len=pos;
        memset(sb->buf+pos,0,len);
    }
    else
    {
        assert(pos+len<0);
        memset(sb->buf+pos+len,0,len);
    }
}
void print_buf(strbuf* sb)
{
    printf("buf > \n");
    for(size_t i = 0;sb->len!=0 && i < sb->len;i++)
        if(!sb->buf) {
            printf(" ");
            break;
        }
        else
        putchar(sb->buf[i]);
    putchar('\n');
    printf("长度 = %d\n",sb->len);
    printf("容量 = %d\n\n",sb->alloc);
    return ;
}
void print_bufarray(strbuf** arr){
    int a =0;
    while(arr[a]){
        print_buf(arr[a]);
        a++;
    }
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{
    int  n =0 ;
    int  count =1 ;
    while(str[n]!='\0')
    {
        if(str[n]==terminator)
        {
            count++;
        }
        n++;
    }
    if(count<max+1)
    {
        strbuf ** arr= (strbuf**)malloc(sizeof(strbuf*)*(count+1));//加一因为最后一项是NULL
        for(int  i =0;i<count;i++)
        arr[i]=(strbuf*)malloc(sizeof(strbuf));
        arr[count]=NULL;
        max = count;
        n=0;count=0;
    char *a=str;
    char *b=str;
    while(str[n]!='\0')///"_asdf_asdf_asdf_sasdf_s"     max指代块的个数
    {
        if(str[n]==terminator)
        {
            b=str+n;
            //str[n]='\0';
            strbuf_attach(arr[count],a,b-a,2*(b-a));
            b+=1;
            count++;
            a=b;
        }
        n++;
        if(count==max-1) 
        {
            while (str[n]!='\0')
            {
                n++;
            }
            break;
        }
    }
    b=str+n;//切割后的最后一段
    strbuf_attach(arr[count],a,b-a,2*(b-a));
    return arr;
}
    
    else//max小
    {
        struct strbuf ** arr=(strbuf**)malloc(sizeof(strbuf*)*(max+1+1));
        for(int i =0;i<=max;i++)
        arr[i]=(strbuf*)malloc(sizeof(strbuf));
        arr[max+1]=NULL;//数组最后一个元素为NULL
        n=0;count=0;
        char *a=str;
        char *b=str;
                                                            max+=1;
    while(str[n]!='\0')///"_asdf_asdf_asdf_sasdf_s"     //下面的max指代块的个数
    {
        if(str[n]==terminator)
        {
            b=str+n;
            strbuf_attach(arr[count],a,b-a,2*(b-a));
            b+=1;
            count++;
            a=b;
        }
            n++;
        if(count==max-1) 
        {
            while (str[n]!='\0')
            {
                n++;
            }
            break;
        }
    }
    b=str+n;//切割后的最后一段
    strbuf_attach(arr[count],a,b-a,2*(b-a));
    return arr;
}
}
bool strbuf_begin_judge(char* target_str, const char* str, int strlen)
{
    int i = 0;
    while(str[i]!='\0')
    {
        if(target_str[i]!=str[i])
        {
            return false;        
        }
        i++;
    }
    return 1;
}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{ 
    if(begin<end&&end<len) {
        char *a =(char*)malloc(sizeof(char)*(end-begin));
        memcpy(a,target_buf+begin,end-begin);
        return a;
    }
    return NULL;
}

int strbuf_getline(struct strbuf* sb, FILE* fp)
{
    char ch;
    while ((ch=fgetc(fp)) != EOF)
    {
        
        if (ch == '\n' || feof(fp) != 0 || ch == '\r')
        {
            break;
        }
        strbuf_addch(sb, ch);
    
    }
    return 1;
}

//测试用

int main()
{
    strbuf* a =NULL;
    a = (strbuf*)malloc(sizeof(strbuf));
    strbuf_attach(a,"wang_yibo ",10,20);

    // strbuf* b =NULL;
    // b = (strbuf*)malloc(sizeof(strbuf));
    // strbuf_attach(b,"wang_yibo ",10,20);

    // strbuf_swap(a,b);
   // size_t len_string = 0;
//     //char * string = strbuf_detach( a,&len_string);
//     //printf("%s\nlen_string = %lu\n\n",string,len_string );
//     strbuf_add(a,b->buf,b->len);
//     strbuf_addch(b,'\n');
//     strbuf_addstr(b,"hahahahahahaha");
//     print_buf(a);
//     print_buf(b);
//     strbuf_setlen(b,15);
//     print_buf(b);
//     //puts("---------------------------------------------");
     strbuf_insert(a,2,"saskdjfhsk2342342wertwertewrtgadfasdfa5 2sasfarwer____ery_pwwqewrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrreeeeeeeeeeeeeeeeeeeeeeeeewwwwwwwwwwwwwwwwwwwwwww",60);
//    // strbuf_insert(a,2,"___________________",19);
//     print_buf(a);
   // puts("---------------------------------------------");

 //   puts("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
    size_t split_arr_max = 20;
    strbuf**arr=strbuf_split_buf(a->buf,a->len,'_',split_arr_max);
    puts("---------------------------------------------");

    // if(strbuf_cmp(a,b)) puts("true");
    // else puts("false");
    print_bufarray(arr);
    print_buf(a);
    return 0;
}










