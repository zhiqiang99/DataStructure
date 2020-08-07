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
/*
初始条件：存在两个线性表LA,LB 
操作结果：扩大线性表LA，将存在于线性表LB中而不存在线性表LA中的数据元素插入到线性表LA中去
	只要从线性表LB中依次取得每个数据元素，并依值在线性表LA中进行查访，若不存在，则插入之
A = AUB

若GetElem和ListInsert这两个操作的执行时间和表长无关，LocalElem的执行时间和表长成正比
算法时间复杂度为：O(ListLength(LA) * ListLength(LB)) 
*/

//void Union(List &La, List Lb)
//{
//	//将所有在线性表Lb中但不在La中的数据元素插入到表La中（不改变Lb）
//	ElemType e;
//	int La_len, Lb_len;
//	int i;
//	La_len = ListLength(La);	//求线性表La的长度
//	Lb_len = ListLength(Lb);	//求线性表Lb的长度
//	for(i=1; i<=Lb_len; i++)	//从表Lb的第一个元素到最后一个元素 
//	{	
//		GetElem(Lb, i, e);		//取表Lb中第i个元素的值赋给e	
//		if(!LoacteElem(La, e, equal))	//表La中不存在和e相同的元素
//			ListInsert(La, ++La_len, e);//在表La的最后插入元素e 
//	}	
//}

 
/*
初始条件：线性表LA,LB存在，且数据元素都是按值非递减有序排列
操作结果：将LA和LB归并为一个新的顺序表LC，且LC中的数据任然按值非递减有序排列
例如：
LA=3,5,8,11
LB=2,6,8,9,11,15,20
LC=2,3,5,6,8,8,9,11,11,15,20 
*/ 
//方法一：
/*
先设立空表LC，然后将LA,LB中的数据元素逐个插入到LC中即可
时间复杂度：O(ListLength(LA) + ListLength(LB)) 
*/ 

//void MergeList(List La, List Lb, List &Lc)
//{
//	//已知线性表La和Lb中的数据元素按值非递减排列
//	//归并La和Lb得到新的线性表Lc，Lc的数据元素也按值非递减排列
//	int i = 1, j=1, k=0;
//	int La_len, La_len;
//	ElemType ai, bj;
//	InitList(Lc);	//创建空表Lc
//	La_len = ListLength(La);	//求线性表La的长度
//	Lb_len = ListLength(Lb);	//求线性表Lb的长度	
//	while(i<=La_len; && j<=Lb_len)	//i,j分别指示表La和Lb中的元素序号
//	{
//		GetElem(La, i, ai);	//取表La中第i个数据元素的值赋给ai
//		GetElem(Lb, i, bj);	//取表Lb中第j个数据元素的值赋给bj
//		if(ai<bi)			//表La的当前元素不大于表Lb的当前元素 
//		{
//			ListInsert(Lc, ++k, ai);	//在表Lc的最后插入元素ai
//			i++;	//i指示表La中的下一个元素		
//		}
//		else
//		{
//			ListInsert(Lc, ++k, bj);	//在表Lc的最后插入元素bj
//			j++;	//j指示表Lb中的下一个元素		
//		}	 
//	} 
//	//以下两个while循环只会有一个被执行
//	while(i<=La_len)	//表La中还有元素未插入到表Lc
//	{
//		GetElem(La, i++, ai);	//取表La中第i个数据元素的值赋给ai，i指示表La中的下一个元素
//		ListInsert(Lc, ++k, ai);	//在表Lc的最后插入元素ai 
//	}
//	while(j<=Lb_len)	//表Lb中还有元素未插入到表Lc
//	{
//		GetElem(Lb, j++, bj);	//取表Lb中第j个数据元素的值赋给bj，j指示表Lb中的下一个元素
//		ListInsert(Lc, ++k, bj);	//在表Lc的最后插入元素bj 
//	} 
//}


//方法二 
void MergeList(Sqlist La, Sqlist Lb, Sqlist &Lc)
{
	//已知顺序线性表La和Lb中的数据元素按值非递减排列
	//归并La和Lb得到新的顺序线性表Lc，Lc的数据元素也按值非递减排列
	ElemType * pa, * pa_last, * pb, * pb_last, *pc;
	pa = La.elem;	//pa指向表La的第一个元素
	pb = Lb.elem;	//pb指向表Lb的第一个元素 
	Lc.listsize = Lc.length = La.length+Lb.length;	//不用InitList()创建空表Lc
	
	pc = Lc.elem = (ElemType *)malloc(Lc.listsize*sizeof(ElemType));	//分配所需空间
	if(!Lc.elem)	//存储分配失败 
	{
		exit(-1);	
	} 
	pa_last = La.elem+La.length-1;	//pa_last指向表La的最后一个元素
	pb_last = Lb.elem+Lb.length-1;	//pb_last指向表Lb的最后一个元素
	
	while(pa<=pa_last && pb<=pb_last)	//表La和表Lb均有元素未归并 
	{
		//归并
		if(*pa<=*pb)	//表La的当前元素不大于表Lb的当前元素 
			*pc++ = *pa++; 	//将pa所指向单元的值赋给pc所指向单元后，pa和pc分别+1(指向下一个单元)
		else 
			*pc++ = *pb++; 	//将pb所指向单元的值赋给pc所指向单元后，pb和pc分别+1(指向下一个单元)
	} 
	//以下两个while循环只会有一个被执行
	while(pa<=pa_last)	//表Lb中的元素全部归并 
	{
		*pc++ = *pa++;	//插入La的剩余元素 
	}
	while(pb<=pb_last)	//表La中的元素全部归并
	{
		*pc++ = *pb++;	//插入Lb的剩余元素
	} 
}

int main(void)
{
	Sqlist La, Lb, Lc;
	int j;
	InitList(La);	//创建空表La
	for(j=1; j<=5; j++)	//在表La中插入5个元素，依次是1,2,3,4,5 
	{
		ListInsert(La, j, j);	
	} 
	printf("La = "); 	//输出表La的内容 
	ListTraverse(La, print1);
	
	InitList(Lb);	//创建空表Lb
	for(j=1; j<=5; j++)	//在表Lb中插入5个元素，依次是2,4,6,8,10 
	{
		ListInsert(Lb, j, j*2);	
	} 
	printf("Lb = "); 	//输出表Lb的内容 
	ListTraverse(Lb, print1);
	
	MergeList(La, Lb, Lc);	//按非递减排列的表La，Lb得到按非递减排列的表Lc
	printf("Lc = "); 	//输出表Lc的内容 
	ListTraverse(Lc, print1); 
	return 0; 
}
