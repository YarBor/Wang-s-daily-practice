the diffinent of the py.
1. for

```py
for iteration_var in sequence:
    statements(s)
```
- 在py中for循环主要针对`系列`(列表 元组 字典) 对系列的每个元素进行操作
```py
for letter in 'Python':     # 第一个实例
   print("当前字母: %s" % letter)
 
fruits = ['banana', 'apple',  'mango']
for fruit in fruits:        # 第二个实例
   print ('当前水果: %s'% fruit)
 
print ("Good bye!")
```
代码中会将`sequence`中的个体进行遍历

- 通过序列引所迭代
```py
fruits = ['banana', 'apple',  'mango']
for index in range(len(fruits)):
   print ('当前水果 : %s' % fruits[index])
 
print ("Good bye!")
```
- for else 语句
在for 循环结束是 可以进入 else模块
```py
names = ['banana', 'apple', 'mango']

for i in range(10)
    for j in names
        j += str(j)
    else print names
```

- break 
break打破最内层的循环
若有else则会跳到else语句
- continue 
和C一样 
- pass 
  p用没有 
  或者说 在定义一个函数后若未想好具体代码 可以用pass占位 
    - ```py
        def sample(n_samples):
        pass

- py 支持使用`del`进行删除单个或多个变量

```py
del val
del vall,valll
```


- Python 支持四种不同的数值类型：

  - 整型(Int) - 通常被称为是整型或整数，是正或负整数，不带小数点。
  - 长整型(long integers) - 无限大小的整数，整数最后是一个大写或小写的L。
  - 浮点型(floating point real values) - 浮点型由整数部分与小数部分组成，浮点型也可以使用科学计数法表示（2.5e2 = 2.5 x 102 = 250）
  - 复数(complex numbers) - 复数由实数部分和虚数部分构成，可以用a + bj,或者complex(a,b)表示， 复数的实部a和虚部b都是浮点型。


- py函数穿惨:python 中一切都是对象，严格意义我们不能说值传递还是引用传递，我们应该说传不可变对象和传可变对象。
- 不定长传参 

```py
   def functionname([formal_args,] *var_args_tuple ):
   "函数_文档字符串"
   function_suite
   return [expression]
```
加了星号的变量名会存放所用未命名的参数
```py
# 可写函数说明
def printinfo( arg1, *vartuple ):
   "打印任何传入的参数"
   print "输出: "
   print arg1
   for var in vartuple:
      print var
   return
 
# 调用printinfo 函数
printinfo( 10 )
```
printinfo( 70, 60, 50 )
```shell
输出:
10
输出:
70
60
50
```

- 匿名函数
- python 使用 lambda 来创建匿名函数。

    - lambda只是一个表达式，函数体比def简单很多。
    - lambda的主体是一个表达式，而不是一个代码块。仅仅能在lambda表达式中封装有限的逻辑进去。
    - lambda函数拥有自己的命名空间，且不能访问自有参数列表之外或全局命名空间里的参数。
    - 虽然lambda函数看起来只能写一行，却不等同于C或C++的内联函数，后者的目的是调用小函数时不占用栈内存从而增加运行效率。

`lambda [arg1 [,arg2,.....argn]]:expression`
```py
#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
# 可写函数说明
sum = lambda arg1, arg2: arg1 + arg2
 
# 调用sum函数
print "相加后的值为 : ", sum( 10, 20 ) # 30
print "相加后的值为 : ", sum( 20, 20 ) # 40 

```
