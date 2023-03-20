#include "test.h"

void fun(bool * a){
    *a = true;
}

int main()
{
    // bool aa = true;
    bool *a = NULL;
    fun(a);
    if (*a)
        puts("a");
    return 0;
}