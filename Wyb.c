#include"strbuf.h"
void strbuf_init(struct strbuf*sb,size_t alloc){
    sb->len=0;
    sb->alloc=alloc;
     if(alloc)
    sb->buf=(char*)malloc(sizeof(char)*(alloc+1));
}

void strbuf_attach(struct strbuf*sb,void*str,size_t len,size_t alloc){//str的len和alloc

    sb->alloc=alloc;
    sb->len=len;
    sb->buf=(char*)str;
    sb->buf[sb->len]='\0';

}
void strbuf_release(struct strbuf *sb)
{   
     if(sb->alloc==0||sb==NULL)return;
    free(sb->buf);//----------
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
void strbuf_swap(struct strbuf*a,struct strbuf*b)
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
    char*ptr=sb->buf;//...?
    *sz=sb->alloc;
    sb->len=0;
    sb->alloc=0;
    return ptr;

}
ssize_t strbuf_read(struct strbuf* sb, int fd, size_t hint)//hint,0x1000,直接给他扩容
{
    FILE*fp=fdopen(fd,"r");
    char c;
    if((c=fgetc(fp))==EOF){
        return sb->len;
    }else{
        sb->buf[sb->len++]=c;
        sb->alloc+=8192;
        sb->buf=(char*)realloc(sb->buf,sizeof(char)*(sb->alloc));
    
        while((c=fgetc(fp))!=-1){
            sb->buf[sb->len]=c;
            sb->len++;
        }
    }
    sb->buf[sb->len]='\0';
    return sb->len;
}
int strbuf_cmp(const struct strbuf *first, const struct strbuf *second)
{
    return first->len-second->len;
    }
void strbuf_reset(struct strbuf *sb){
    for(int a  = 0; a<sb->len;a++)
        sb->buf[a]='\0';
    sb->len=0;
}
void strbuf_grow(struct strbuf* sb,size_t extra)
{
    if(sb->alloc-sb->len-1 > extra)
    return ;
    else 
    {
        sb->buf=(char *)realloc(sb->buf,(sb->alloc+extra)*sizeof(char));
        sb->alloc=(sb->alloc+extra);
        if(sb -> alloc == 0)        sb->buf[0]='\0';

    }
    return ;
}
void strbuf_add(struct strbuf *sb, const void *data, size_t len)
{
    strbuf_grow(sb,len+1);
    memcpy(sb->buf+sb->len,data,len);////////////////
    sb->len=sb->len+len;
    sb->buf[sb->len]='\0';
    return ;

}
void strbuf_addch(struct strbuf*sb , int c)
{
    strbuf_grow(sb,2);
    sb->buf[sb->len]=c;
    sb->len++; 
    sb->buf[sb->len]='\0';
    return ;
}
void strbuf_addstr(struct strbuf* sb,const char* s)
{
    int s_len =strlen(s);
    strbuf_grow(sb,s_len+1);
    memcpy(sb->buf+sb->len,s,s_len);
    sb->len=sb->len+s_len;
    sb->buf[sb->len]='\0';

}
void strbuf_addbuf(struct strbuf *sb, const struct strbuf *sb2)
{
    strbuf_grow(sb,sb2->len+1);
    memcpy(sb->buf+sb->len,sb2->buf,sb2->len);
    sb->len+=sb2->len;
    sb->buf[sb->len]='\0';
    return ;// ?
}
void strbuf_setlen(struct strbuf *sb, size_t len)
{
    sb->len=len;
    sb->buf[len]='\0';
    return ;    
}
size_t strbuf_avail(const struct strbuf *sb )
{
    return (sb->alloc-sb->len-1);
}
void strbuf_insert(struct strbuf* sb, size_t pos,const void* data,size_t len)
{

    strbuf_grow(sb,len+1);
    memmove(sb->buf+pos+len,sb->buf+pos,sb->len-pos);
    memmove(sb->buf+pos,(char*)data,len);
    sb->len+=len;
    sb->buf[sb->len]='\0';
}
void strbuf_rtrim(struct strbuf*sb)
{
    int i=sb->len-1;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'){
        sb->len--;
        sb->buf[i]='\0';
        i--;
    }
}
void strbuf_ltrim(struct strbuf*sb)
{
    int i=0;
    while(sb->buf[i]==' '||sb->buf[i]=='\t'){
        sb->len--;
        i++;
    }
    memmove(sb->buf,sb->buf+i,sb->len);
}
void strbuf_remove(struct strbuf *sb, size_t pos, size_t len){
 memmove(sb->buf+pos,sb->buf+pos+len,sb->len-pos-len);
     sb->len-=len;
}

struct strbuf **strbuf_split_buf(const char *str, size_t len, int terminator, int max)
{///////////
      int i,count=0;
    
    char q[2]; 
    q[0]=(char)terminator;
    q[1]='\0';
    struct strbuf **ptr2=NULL;
    struct strbuf *ptr;
    char s[len+1];
    memcpy(s,str,len+1);
    
    for(i=0;i<len;i++){
        if(s[i]=='\0'){
            s[i]='#';
        }
    }
    char*r=strtok(s,q);
    
    
    while(r!=NULL&&count<max)
    {   
        int rlen=strlen(r);
        for(i=0;i<rlen;i++){
            if(r[i]=='#'){
                r[i]='\0';
            }
        }
        ptr=(struct strbuf*)malloc(sizeof(struct strbuf));
        {
            strbuf_init(ptr,rlen+1); 
            strbuf_add(ptr,r,rlen);
        }
        ptr2=(struct strbuf**)realloc(ptr2,sizeof(struct strbuf*)*(count+2));
        ptr2[count]=ptr;
        count++;
        
        r=strtok(NULL,q);
        
    }

    ptr2=(struct strbuf**)realloc(ptr2,sizeof(struct strbuf*)*(count+1));


    ptr2[count]= NULL;
    return ptr2;


}
bool strbuf_begin_judge(char* target_str, const char* str, int strlen)
{
    int i;
    for(i=0;i<strlen;i++){
        if(str[i]=='\0'){
            break;
        }
        if(target_str[i]!=str[i]){
            return false;
        }
    }
    return true;

}
char* strbuf_get_mid_buf(char* target_buf, int begin, int end, int len)
{ 
     if(begin>end||target_buf==NULL){
        return NULL;
    }
    char*ptr=(char*)malloc(sizeof(char)*(end-begin+1));
        memcpy(ptr,target_buf+begin,end-begin);
        ptr[end-begin]='\0';
    return ptr;
}

int strbuf_getline(struct strbuf* sb, FILE* fp)
{
    char ch;
    while ((ch=fgetc(fp)) != EOF)
    {
        if (ch == '\n' || feof(fp) != 0 || ch == '\r')    break;
        strbuf_addch(sb, ch);
    }
    return 1;
}
