#include<stdio.h> 
 
int main(){  
    int i = 0;  
    int j = 0;  
    int n;
    scanf("%d",&n);
    for(i=1;i<=n;i++){  
        for(j=1;j<=i;j++){  
            printf("%dx%d=%d\t",j,i,i*j);  
        }  
       
    }  
}
