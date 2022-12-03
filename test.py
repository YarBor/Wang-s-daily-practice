#!/bin/python3


#list 
print ("int a d d d d")
wang = ["wangY", "yao", "wangH","123"]
print(wang[1:2])
wang.append("hhh")
print(wang)
yi = wang.copy()#return the hole list 
print(yi)
i = wang.count("you")#return the count of the `string`s times in the list
print(i)
if(wang.index("123")):
    print(wang)
    print(wang.pop())# returen the pop item
wang.sort()#it didn't return Or return None
print(wang)

# to use [] to create a list 

# tuple {} -> 元组
# tpule can't be changed  !!!!!
wang = {1,2}
# wang[1]=3 !!! error !!!
# pirnt() did not return 
print(wang)
def function():
    print("function say hello world")

function()

def function1(name ,age,int = 0):
    print(name +" is "+str(age)+" years old")
    #print (int)
    return  name +" is "+str(age)+" years old"
wang = "wang"
print(function1(wang, 18))