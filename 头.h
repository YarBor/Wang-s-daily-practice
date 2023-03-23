#define _CRT_SECURE_NO_WARNINGS 1		
void menu();
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>
static int size_ = 0;
void delete_one(int x, int* size_, struct person** point);
struct person {
	char name[20];
	char sex[10];
	char phone[20];
};
int to_revise(struct person** point, int size_);
static struct person* point[100];
void mysort(struct person**, int size_);
void print(struct person* point[], int size_);
enum xuanxiang {
	out = 0,
	add,
	del,
	scan,
	revise,
	sort
};
void add_one(struct person*);
int find_(char*, struct person** point, int size_);
