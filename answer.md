### Xiyou_LinuxGroup2022 纳新答案
本篇会指出每到题目所包含的知识点 建议有所不了解时 查看链接内容

---
#### - 0  我的计算器坏了?
![image.png](https://s2.loli.net/2022/10/26/MGci2q9rEOCUn8X.png)
应该和数学有关 用 log 算...什么的
咳 不会

---
#### - 1  printf 还能这么玩?
![image.png](https://s2.loli.net/2022/10/26/6VQBlbW9ofuTPyO.png)
题目关于 printf()函数的返回值 
- **相关知识点链接**:> http://t.csdn.cn/kZvjN
答案: >输出 `Xiyou Linux Group - 2022`
---
#### - 2 你好你好你好呀
![image.png](https://s2.loli.net/2022/10/26/Pih8Lgx43p1ySFu.png)
老生常谈的sizeof()和strlen()
- **相关知识点链接**:> http://t.csdn.cn/zsjk7
```c
输出: >
p == p1: 0       p2 == p1: 1     p == p2: 0
sizeof(p): 15    sizeof(p1): 8  sizeof(*p2): 1
strlen(p): 11    strlen(p1) :11
```
---
#### - 3 换个变量名不行吗
![image.png](https://s2.loli.net/2022/10/26/6Dk5PzXIxV2ZnjM.png)
变量的生命周期, 未初始化的变量声明
- **相关知识点链接** 1.生命周期 :> http://t.csdn.cn/j3wA6
- **相关知识点链接** 2.声明与定义 : 在第一条printf()中的 a , 变量的内容是随机的 未定义的 ,
    ```c
    相当于
    int main(int arg1,char **argv)
    {
        int a ;
        printf("%d",a);
    }
    ```
- 输出: > 
```shell
a = 1234 
a = 2
a = 3 
```
---
#### - 4 内存对不齐
![image.png](https://s2.loli.net/2022/10/26/6Dk5PzXIxV2ZnjM.png)
结构体内存对齐: >http://t.csdn.cn/6Uqwg
联合体相关知识点: >http://t.csdn.cn/ewKwK
答案: > 
```c
输出
sizeof(UNION) = 24
sizeof(STRUCT) = 40
```
---
#### - 5 Bitwise
![image.png](https://s2.loli.net/2022/10/26/lBj7VmqIHdvkOh4.png)
位运算:>http://t.csdn.cn/NSSsB
整形提升和算数转换:> http://t.csdn.cn/LtMi2
数据储存(源码反码补码):> http://t.csdn.cn/BFqsp
答案: >
```c
输出
a: 56,b: 0,c: 255,d: 56
e: 0x4
```
---
#### - 6 英译汉
![image.png](https://s2.loli.net/2022/10/26/F26HxZPfLNqDoRX.png)
const:> 
> int x = 1,y = 2;
>  // a 与 b 都是整型常量，值都不能修改,只读变量
>  const int a = 0;
>  int const b = 0;

>  const int *px = &a;// px 不是常量 *px 是常量
>  int * const py = &y;// py 是常量 *py 不是常量
>  const int * const pp = &x;// pp 与 *pp 都是常量
>  /*
>  1、如果 const 在 * 前面，修饰的是数据类型，指向的内容不
>   能修改
>  2、如果 const 在 * 后面，修饰的是变量，变量是常量
>  3、如果 const 在 * 前后都有，都是常量
>  */
---
#### -7 汉译英
![image.png](https://s2.loli.net/2022/10/26/26ZS8azw3jxV1b4.png)
函数指针的声明及相关知识点;http://t.csdn.cn/gHQmn
答案: > 
```
int *p [10];
int (*p)[10];
int (*p[3])(int);
```
---
#### - 8 混乱中建立秩序
![image.png](https://s2.loli.net/2022/10/26/XM3IFCHDilQN6RK.png)
就.. 算法 (网上纸己搜)

---
#### - 9 手脑并用
![image.png](https://s2.loli.net/2022/10/26/fnp5JH1bcxQutKy.png)
动态规划内存:>http://t.csdn.cn/jFBNQ
```c
char* convertAndMerge(char word[][20]);
int main(void) {
  char words[2][20] = {"Welcome to Xiyou ", "Linux Group 2022"};

  printf("%s\n", words[0]);
  printf("%s\n", words[1]);

  char* str = convertAndMerge(words);
  printf("str = %s\n", str);
  free(str);
}
char* convertAndMerge(char word[][20])
{
  char *a = (char *)malloc(sizeof(char)*20*2);
  sprintf(a,"%s%s",word[0],word[1]);
  return a ;
}

```
---
#### - 10 给我你的指针 访问你的心声
![image.png](https://s2.loli.net/2022/10/26/fEL1Y8jRUrX5wZn.png)
这里是输出:>
```c
0       1       2       3       4
25      26      27      28      29
45      46      47      48      49
60      61      62      63      64
70      71      72      73      74
```
开始变得有意思了 为什么这么输出呢;
因为 二位数组地址的存放是连续的
- 未开始时    
arr[][]内部是
```c
[0]     0       0       0       0//temp位置
0       0       0       0       0
0       0       0       0       0
0       0       0       0       0
0       0       0       0       0
```

- 所以 当外层循环 i=0 跑完时 

```c
// i = 0 时

0         1       2       3       4
5         6       7       8       9
10        11      12      13      14
15        16      17      18      19
20        21      22      23      24
[*]//当我的指针走到[*]时 出循环
```
i++;
然后 temp = *(arr+i)
进入循环 继续++ 赋值
```c
0        1       2       3       4
[5]      6       7       8       9 //[temp]位置 即开始++赋值的位置
10      11      12      13      14
15      16      17      18      19
20      21      22      23      24

```

```c
0       1       2       3       4
25      26      27      28      29
30      31      32      33      34
35      36      37      38      39
40      41      42      43      44
[*]//结束
```
然后同理
```c
0       1       2       3       4
25      26      27      28      29
[30]    31      32      33      34  //启始位置下移
35      36      37      38      39
40      41      42      43      44
```
循环++赋值 直到`temp位置 和 arr[5]`重合
拿到答案:>
```c
0       1       2       3       4
25      26      27      28      29
45      46      47      48      49
60      61      62      63      64
70      71      72      73      74
```
---
#### - 11 奇怪的参数
![image.png](https://s2.loli.net/2022/10/26/KcpDnY4qOybHh6M.png)

argc和argv:> http://t.csdn.cn/A1rCU
变量溢出:> http://t.csdn.cn/GIXNA
这里 argc和argv的用处不大 这二者在系统编程中运用较多\

#### - 12 奇怪的字符
![image.png](https://s2.loli.net/2022/10/26/lUD92ji7zpaEcWA.png)

大小端的储存格式;> http://t.csdn.cn/7oqzN
ascll碼转换;
输出:>
`Welcome to Xiyou Linux Group 2022`

---
#### - 13 小试宏刀
![image.png](https://s2.loli.net/2022/10/26/bWIa9o2KQTVsFCN.png)

输出:>
```c
x = 2 y = 1 tmp = 1
x = 1 y = 2 tmp = 2
x = 2, y = 2
x = 5 y = 5 tmp = 2
```
考察的是宏的相关问题
简而言之 宏定义会在预编译阶段被编译器插入到源代码中;
下面是预编译后的内容
```c
int main()
{
  int tmp;
  int x = 1;
  int y = 2;
  int z = 3;
  int w = 3;
  tmp = x; x = y; y = tmp;
  printf("x = %d y = %d tmp = %d\n", x, y, tmp);
  if (x > y)
    tmp = x; x = y; y = tmp;
  printf("x = %d y = %d tmp = %d\n", x, y, tmp);
  if (1 + 2 + z++ + ++w *1 + 2 + z++ + ++w == 100) tmp = x; x = y; y = tmp;
  printf("x = %d, y = %d\n", x, y);
  printf("x = %d y = %d tmp = %d\n", z, w, tmp);
}

```
---
#### - 14 GNU/Linux命令(选做)
![image.png](https://s2.loli.net/2022/10/26/f2xzOZpckqVB8Fl.png)
关于Linux常用的指令:> http://t.csdn.cn/yY01k


---
done