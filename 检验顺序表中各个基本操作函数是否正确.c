/*
总结：
使用结构体构造一个顺序表结构
当需要改变结构体中的成员变量的值的时候需要使用引用类型，或者使用指针变量
如果不需要对结构体中成员变量值进行改变，而只是判断，返回类型为bool时，不需要使用引用类型
当你在进行判断的时候使用指针的话也是可以的，但是就是不安全，可能在子函数里面改变了原来的值，但是Java和C++可以避免 

函数指针
这个语句就定义了一个指向函数的指针变量 p。首先它是一个指针变量，所以要有一个“*”，即（*p）；
其次前面的 int 表示这个指针变量可以指向返回值类型为 int 型的函数；后面括号中的两个 int 表示这个指针变量可以指向有两个参数且都是 int 型的函数。
所以合起来这个语句的意思就是：定义了一个指针变量 p，该指针变量可以指向返回值类型为int类型，且有两个整型参数的函数。p的类型为 int( * )(int, int) 
 
后自增
*p++
后自增整体表达式的值是*p加1之前的值, 比较完之后在加一 

指针数组
*(a+3)等价于a[3]等价于pArr[3]等价于*(pArr+3)也就是说他们四个所代表的是同一个变量
p+i的值是p+i * (p所指向的变量所占字节数)	  也就表示指针变量p指向下一个元素 

realloc函数语法
指针名=（数据类型*）realloc（要改变内存大小的指针名，新的大小）。
新的大小可大可小(如果新的大小大于原内存大小，则新分配部分不会被初始化；如果新的大小小于原内存大小，可能会导致数据丢失)

在函数ListTraverse()中
将visit()函数作为形参，为了要在 ListTraverse()中根据情况调用不同的函数而不是一个固定的数，从作为形参的visit()函数得知，
满足以下条件的函数可以被ListTraverse()函数调用：
在函数声明中指定了visit()的函数返回值类型：void
形参个数：一个
形参类型：ElemType的引用类型（ElemType&）
满足以上条件都可以作为ListTraverse()函数的实参 

为了和ListTraverse()函数要求一致，其形参被定义为引用类型，原因另一个实参函数dbl()是给形参的值加倍，且要将形参值的改变带回主调函数，
故必须为引用类型 

注意：C语言中数组下标从0开始，顺序表中第i个元素是L.elem[i-1] 

*/

 

//头文件
#include <stdio.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

//函数结果状态代码
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
typedef int Status;	//status是函数的类型，其值是函数结果状态代码，如OK 
typedef int Boolean;//Boolean是布尔类型，其值是TRUE或FALSE

typedef int ElemType;		//定义ElemType为int类型 

#define LIST_INIT_SIZE 10	//线性表存储空间的初始分配了 
#define LISTINCREMENT 2 	//线性表存储空间的分配增量
typedef struct Sqlist	//线性表的顺序存储结构 
{
	ElemType *elem;		//存储空间基址	 
	int length;			//当前长度（数组有效元素个数） 
	int listsize;		//当前分配的存储容量 （以sizeof(ElemType)为单位） 
}Sqlist;  


//顺序存储的线性表的基本操作
void InitList(Sqlist &L)	//操作结果：构造一个空的顺序线性表 
{
	L.elem = (ElemType *)malloc(LIST_INIT_SIZE * sizeof(ElemType));
	if(!L.elem)	//存储分配失败返回NULL
		exit(-1);
	else
	{
		L.length = 0;	//空表长度为0
		L.listsize = LIST_INIT_SIZE;	//初始存储容量	
	}	
} 

void DestroyList(Sqlist &L)	//初始条件：顺序线性表L已经存在。 操作结果：销毁顺序线性表L 
{
	free(L.elem);	//释放L.elem所指向的存储空间
	L.elem = NULL;	//L.elem不再指向任何存储单元 
	L.length = 0;
	L.listsize = 0; 
} 

void ClearList(Sqlist &L)	//初始条件：顺序线性表L已经存在。操作结果：将L重置为空表 
{
	L.length = 0;	//把有效元素个数置为0	
} 

Status ListEmpty(Sqlist L)	//因为不需要对顺序表里面的数据进行操作，所以不需要采用引用数据类型 
							//初始条件：顺序线性表L已存在。操作结果：若L为空表，则返回true，否则返回false 
{
	if(L.length == 0)
		return true;
	else
		return false;	
} 

int ListLength(Sqlist L)	//初始条件：顺序线性表L已存在。操作结果：返回L中数据元素的个数 
{
	return L.length; 
}

Status GetElem(Sqlist L, int i, ElemType &e)
{
	//初始条件：顺序线性表已经存在，1<=i<=ListLength(L)
	//操作结果：用e返回L中第i个数据元素的值
	if(i<1 || i>L.length)	//i不在表L的范围
		return false;
	e = *(L.elem+i-1);	//将表L的第i个元素的值赋值给e  或者写成e = L.elem[i-1];
	return OK;	
} 

int LocateElem(Sqlist L, ElemType e, Status(*compare)(ElemType, ElemType))	//此处使用的是函数指针 
{
	//初始条件：顺序线性表L已存在，compare()是数据元素判定函数(满足为1，否则为0)
	//操作结果：返回L中第一个与e满足关系compare()的数据元素的位序
		//若这样的数据元素不存在返回0
	int i=1;	//i的初始值为第一个元素
	ElemType * p = L.elem;	//p的初值为第一个元素的存储位置
	while(i<=L.length && !compare(*p++, e))	//i未超出表的范围且未找到满足关系的数据元素 
	{
		i++;	//继续向后找	
	} 
	if(i<=L.length)	//找到满足关系的数据元素
		return i;
	else //未找到满足关系的数据元素
		return 0;  
}

Status PriorElem(Sqlist L, ElemType cur_e, ElemType &pre_e)
{
	//初始条件：顺序线性表L已经存在
	//操作结果：若cur_e是L的数据元素，且不是第一个，则用pre_e返回它的前驱；
		//若操作失败 pre_e无定义
	int i = 2;	//	从第二个元素开始
	ElemType * p = L.elem+1;	//p指向第二个元素
	while(i<=L.length && *p!=cur_e)	//i未超过表的范围且未找到值为cur_e的元素 
	{
		p++;	//p指向下一个元素 
		i++;	//计数加一 
	} 
	if(i>L.length) 	//找到表结束都没找到值为cur_e的元素
		return false; //操作失败
	else	//找到值为cur_e的元素，并由p指向其
	{
		pre_e = *--p;	//p指向前一个元素cur_e的前驱，将所指元素的值赋值给pre_e
		return OK; 
	} 
}

Status NextElem(Sqlist L, ElemType cur_e, ElemType &next_e)
{
	//初始条件：顺序线性表L已经存在
	//操作结果：若cur_e是L的数据元素，且不是最后一个，则用next_e返回它的后继；
		//若操作失败 next_e无定义
	int i = 1;	//	从第一个元素开始
	ElemType * p = L.elem;	//p指向第一个元素
	while(i<L.length && *p!=cur_e)	//i未找到表尾且未找到值为cur_e的元素 
	{
		p++;	//p指向下一个元素 
		i++;	//计数加一 
	} 
	if(i==L.length) 	//到表尾的前一个元素都没找到值为cur_e的元素
		return false; //操作失败
	else	//找到值为cur_e的元素，并由p指向其
	{
		next_e = *++p;	//p指向下一个元素(cur_e的后继)，将所指元素的值赋值给next_e
		return OK; 
	} 	
} 

Status ListInsert(Sqlist &L, int i, ElemType e) 
{
	//初始条件：顺序线性表L已经存在，1<=i<=ListLength(L)+1
	//操作结果：在L中第i个位置之前插入新的数据元素e， L的长度加一
	ElemType * newbase, * q, * p;
	if(i<1 || i>L.length+1)	//i值不合法
		return false;
	if(L.length == L.listsize)	//表示当前存储空间已满，增加分配，修改 
	{
		newbase = (ElemType *)realloc(L.elem, (L.listsize+LISTINCREMENT)*sizeof(ElemType));
		if(!newbase)	//存储空间分配失败 
			exit(-1);
		L.elem = newbase;	//新基址赋值给L.elem
		L.listsize += LISTINCREMENT; 	//存储容量的数值增加 
	} 
	q = L.elem-1+i;	//q为插入位置
	for(p=L.elem+L.length-1; p>=q; --p)	//插入位置及之后的元素右移(由表尾元素开始移动)
	{
		*(p+1) = *p; 
	} 
	*q = e;	//插入e
	L.length++; //表长加一
	return OK; 
}

Status ListDelete(Sqlist &L, int i, ElemType &e)
{
	//初始条件：顺序线性表L已存在，1<=i<=ListLength(L)
	//操作结果：删除L的第i个数据元素，并用e返回其值，L的长度减一
	ElemType * p, * q;
	if(i<1 || i>L.length)
		return false;
	p = L.elem-1+i;	//p为被删除的元素的位置
	e = *p;	//要被删除的元素的值赋值给e
	q = L.elem + L.length-1; 	//q为表尾的元素位置
	for(p++; p<=q; p++)	//被删除元素之后的元素左移
		*(p-1) = *p;
	L.length--;	//表长减一 
	return OK; 
} 

void ListTraverse(Sqlist L, void(*visit)(ElemType&))
{
	//初始条件：顺序线性表L已存在 
	// 操作结果：依次对L的每一个数据元素调用函数visit()
		//visit()的形参加&，表明可通过调用visit()改变元素的值
	ElemType * p = L.elem;	//p指向第一个元素 
	int i;
	for(i=1; i<=L.length; i++)	//从表L的第一个元素到最后一个元素 
	{
		visit(*p++);	//对每个数据元素调用visit() 
	} 
	printf("\n");
} 

//几个常用的函数
Status equal(ElemType c1, ElemType c2)	//判断是否相等的函数
{
	if(c1 == c2)
	{
		return true;	
	} 
	else
	{
		return false;
	}
}

int comp(ElemType a, ElemType b)	//根据a<、=、>b，分别返回-1,0,1 
{
	if(a == b)
		return 0;
	else
		return (a-b)/abs(a-b);
} 

void print(ElemType c)	//以十进制整型的格式输出元素的值 
{
	printf("%d ", c);
}

void print1(ElemType &c)//以十进制整型的格式输出元素的值 (设c为引用类型)
{
	printf("%d ", c);	
} 

void print2(ElemType c)	//以字符型的格式输出元素的值 
{
	printf("%c ", c);	
} 

Status sq(ElemType c1, ElemType c2)	//数据元素判定函数平方关系，LocateElem()调用的函数 
{
	if(c1 == c2*c2)
		return TRUE;
	else 
		return FALSE; 
} 

void dbl(ElemType &c)	//元素值加倍，ListTraverse()调用的另一个函数 
{	
	c *= 2;
}


//主函数 
int main(void)
{
	Sqlist L;
	ElemType e, e0;
	Status i;
	int j, k;
	InitList(L);	//初始化线性表L
	printf("初始化L后，L.length = %d, L.listsize = %d, L.elem = %u\n", L.length, L.listsize, L.elem);

//初始化	
	for(j=1; j<=5; j++)	//在第i个位置之前插入j元素，循环进行下一轮是，插入的第一个元素就会下移，第一个位置始终是新的元素 
	{
		i = ListInsert(L, 1, j);	//在L的表头插入j	
	} 
	printf("在L的表头依次插入1-5后，*L.elem = ");
	for(j=1; j<=5; ++j)
	{
		printf("%d ", *(L.elem-1+j));	//依次输出表L中的元素 
	} 

//遍历输出	
	printf("\n调用ListTraverse()函数，依次输出表L中的元素：");
	ListTraverse(L, print1);	//依次对表L中的元素调用print1()函数(输出元素的值) 

//判断表是否为空	
	i = ListEmpty(L);	//检查表L是否为空 
	printf("L.length = %d, L.listsize = %d(不变), ", L.length, L.listsize);
	printf("L.elem = %u(不变), L是否为空？i = %d\n", L.elem, i);

//清空表L	 
	ClearList(L);	//清空表L
	i = ListEmpty(L); //再次检测表是否为空 
	printf("清空L之后 L.length = %d, L.listsize = %d(不变), ", L.length, L.listsize);
	printf("L.elem = %u(不变), L是否为空？i = %d\n", L.elem, i);
	
//插入元素
	for(j=1; j<=10; j++)
	{
		ListInsert(L, j, j);	//在L的表尾插入j	
	} 
	printf("在L的表尾依次插入1-10后，L=");
	ListTraverse(L, print1);	//依次对表L中的元素调用print1()函数(输出元素的值)  
	printf("L.length = %d, L.listsize = %d, L.elem = %u\n", L.length, L.listsize, L.elem);
	
//增加存储空间
	ListInsert(L, 1, 0); 	//在表头插入0，增加存储空间
	printf("在L的表头插入0后，L.length = %d, L.listsize = %d(改变), L.elem = %u(可能改变)\n", L.length, L.listsize, L.elem);
	
//得到表中某个位置的元素
	GetElem(L, 5, e);	//将表L中的第5个元素的值赋给e
	printf("第五个元素的值为%d\n", e);
	
//查找表中某个元素
	for(j=10; j<=11; j++)
	{
		k = LocateElem(L, j, equal);	//查找表L中与j相等的元素，并将其位序赋给k
		if(k)	//k不为0，表明有符合条件的元素
			printf("第%d个元素的值为%d，", k, j);
		else	//k为0，没有符合条件的元素
			printf("没有值为%d的元素\n", j); 			
	} 
	for(j=3; j<=4; j++)	//测试两个数据 
	{
		k = LocateElem(L, j, sq);	//查找表L中与j的平方相等的元素，并将其位序赋给k
		if(k)	//k不为0，表明有符合条件的元素
			printf("第%d个元素的值为%d的平方，", k, j);
		else	//k为0，没有符合条件的元素
			printf("没有值为%d的平方元素\n", j); 			
	}
	
//求顺序表的前驱元素
	for(j=1; j<=2; j++) //测试前两个数据
	{
		GetElem(L, j, e0); 	//将表L中的第j个元素的值赋给e0
		i = PriorElem(L, e0, e);	//求e0的前驱，如果成功将值赋给e 
		if(i == ERROR)	//操作失败 
			printf("元素%d无前驱，", e0);
		else	
			printf("元素%d的前驱为%d\n", e0, e); 			
	} 
	
//求顺序表的后继元素 
	for(j=ListLength(L)-1; j<=ListLength(L); j++) //测试最后两个数据
	{
		GetElem(L, j, e0); 	//将表L中的第j个元素的值赋给e0
		i = NextElem(L, e0, e);	//求e0的后继，如果成功将值赋给e 
		if(i == ERROR)	//操作失败 
			printf("元素%d无后继\n", e0);
		else	
			printf("元素%d的后继为%d, ", e0, e); 			
	} 

//删除表中元素
	k = ListLength(L);	//k为表长 
	for(j=k+1; j>=k; j--) //测试最后两个数据
	{
		i = ListDelete(L, j, e);	//删除第j个数据元素，并将第j个元素的值赋值给e 
		if(i == ERROR)	//表中没有第j个元素 
			printf("删除第%d个元素失败。", j);
		else			//表中存在第j个数据元素，删除成功，其值赋给e 
			printf("删除第%d个元素成功，其值为%d", j, e); 			
	} 
	
//对L数据元素进行加倍
	ListTraverse(L, dbl);	//依次对元素调用dbl，元素值乘以2
	printf("L的元素值加倍后，L= ");
	ListTraverse(L, print1);	//依次输出表L中的元素
	
//销毁表L
	DestroyList(L);
	printf("销毁L后，L.length = %d, L.listsize = %d, L.elem = %u\n", L.length, L.listsize, L.elem); 

	return 0;	
} 

 
