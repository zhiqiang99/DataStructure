#include <stdio.h>
#include <malloc.h>
#include <stdlib.h> //包含了exit函数

struct Arr //定义了一个数据类型，该数据类型名字叫struct Arr，该数据类型有三个成员
{
	int *pBase; //存储的是数组第一个元素的地址
	int len;	//数组所能容纳的最大元素的个数
	int cnt;	//当前数组有效元素的个数
};

void init_arr(struct Arr *, int);					   //初始化数组
bool append_arr(struct Arr *, int val);				   //在末尾追加元素
bool insert_arr(struct Arr *pArr, int pos, int val);   // pos的值从1开始，表示插入的位置
bool delete_arr(struct Arr *pArr, int pos, int *pVal); //函数内部可以通过pVal修改val的值，把要删除的值赋给了val这个变量
bool is_empty(struct Arr *);
bool is_full(struct Arr *);
void sort_arr(struct Arr *); //排序输出
void show_arr(struct Arr *);
void inversion_arr(struct Arr *); //倒置输出

int main(void)
{
	struct Arr arr; //定义一个变量arr
	int val;

	init_arr(&arr, 6);
	show_arr(&arr);

	append_arr(&arr, 1);
	append_arr(&arr, 2);
	append_arr(&arr, 3);
	append_arr(&arr, 4);
	append_arr(&arr, 5);
	insert_arr(&arr, 6, 99);

	show_arr(&arr);

	printf("\n");
	if (delete_arr(&arr, 1, &val)) //把val的地址发送给pVal
	{
		printf("删除成功\n");
		printf("您删除的元素是：%d\n", val);
	}
	else
	{
		printf("删除失败\n");
	}

	printf("之后的元素是：\n");
	show_arr(&arr);

	printf("\n");
	inversion_arr(&arr);
	printf("逆序输出之后的结果\n");
	show_arr(&arr);

	printf("\n");
	sort_arr(&arr);
	printf("输出排序后的元素：\n");
	show_arr(&arr);
	return 0;
}

//初始化一个数组
void init_arr(struct Arr *pArr, int length)
{
	pArr->pBase = (int *)malloc(sizeof(int) * length); //指针变量pArr所指向结构体变量中的pBase这个成员
	if (NULL == pArr->pBase)						   //分配失败把NULL分配给pBase
	{
		printf("动态内存分配失败！");
		exit(-1); //终止整个程序
	}
	else //分配成功
	{
		pArr->len = length;
		pArr->cnt = 0; //此时用户还没有给数组赋值，所有有效长度为0个
	}
	return; //指示函数已经终止
}

bool is_empty(struct Arr *pArr)
{
	if (0 == pArr->cnt)
		return true;
	else
		return false;
}

bool is_full(struct Arr *pArr)
{
	if (pArr->len == pArr->cnt) //判断有效个数是否为空
		return true;
	else
		return false;
}

void show_arr(struct Arr *pArr)
{
	if (is_empty(pArr))
	{
		printf("数组为空！\n");
	}
	else
	{
		for (int i = 0; i < pArr->cnt; ++i)
		{
			printf("%d ", pArr->pBase[i]);
			printf("\n");
		}
	}
}

bool append_arr(struct Arr *pArr, int val)
{
	if (is_full(pArr))
		return false; //满时返回false
	else
		pArr->pBase[pArr->cnt] = val;
	pArr->cnt++;
	return true;
}

bool insert_arr(struct Arr *pArr, int pos, int val)
{
	int i;

	if (is_full(pArr))
		return false;

	if (pos < 1 || pos > pArr->cnt + 1) //当前有效存储元素个数cnt=3时，可插入位置pos可以为4，但是不能放在第5个位置
		return false;

	for (i = pArr->cnt - 1; i >= pos - 1; --i)
	{
		pArr->pBase[i + 1] = pArr->pBase[i];
	}
	pArr->pBase[pos - 1] = val; //把值放在要插入的位置
	(pArr->cnt)++;

	return true;
}

bool delete_arr(struct Arr *pArr, int pos, int *pVal) //需要返回删除的元素值
{
	int i;

	if (is_empty(pArr))
		return false;
	if (pos < 1 || pos > pArr->cnt)
		return false;

	*pVal = pArr->pBase[pos - 1]; //先要把要删除的元素的值赋值给主函数的val保存下来，之后再移动后面的元素

	for (i = pos; i < pArr->cnt; ++i) //把要删除元素之后元素前移
	{
		pArr->pBase[i - 1] = pArr->pBase[i];
	}
	(pArr->cnt)--; //删完元素，cnt元素个数减一

	return true;
}

void inversion_arr(struct Arr *pArr)
{
	int i = 0;
	int j = pArr->cnt - 1;
	int t;

	while (i < j) //当为奇数个时，中间一个数i和j相等时就不需要交换了，偶数个数i<j也符合
	{
		t = pArr->pBase[i]; //交换a和b：t=a,a=b,b=t
		pArr->pBase[i] = pArr->pBase[j];
		pArr->pBase[j] = t;
		++i;
		--j;
	}
	return;
}

void sort_arr(struct Arr *pArr) //进行冒泡排序
{
	int i, j, t;

	for (i = 0; i < pArr->cnt - 1; ++i)
	{
		for (j = 0; j < pArr->cnt - 1 - i; ++j)
		{

			if (pArr->pBase[j] > pArr->pBase[j + 1])
			{
				t = pArr->pBase[j];
				pArr->pBase[j] = pArr->pBase[j + 1];
				pArr->pBase[j + 1] = t;
			}
		}
	}
}