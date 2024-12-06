#include<iostream>
#define max_size 100
using namespace std;
bool check[max_size];
struct Stack
{
	int arr[max_size];
	int top;
};
void CreateStack(Stack &s)
{
 	s.top = -1;
}
bool EmptyStack(Stack s)
{
	if(s.top==-1)
 		return true;
 	return false;
}
bool IsFullStack(Stack s) 
{
 	if(s.top>=max_size)
 		return true;
 	return false;
 
}
void Push(Stack &s,int x)
{
	if(!IsFullStack(s))
 	{
 		s.top ++;
 		s.arr[s.top] = x;
 	}
}
void Pop(Stack &s)
{
	if(!EmptyStack(s))
 	{
 		s.top--;
 	}
}
int Top(Stack s)
{
	return s.arr[s.top];
}
void DFS(int a[][max_size],int n)
{
	Stack q;
	CreateStack(q);
	int rout;
	cout<<"nhap rout: ";
	cin>>rout;
	Push(q,rout);
	check[rout] = 0;
	while(!EmptyStack(q))
	{
		int flag = 0;
		int x = Top(q);
		for(int i=1; i <= n;i++)
			if(check[i] == 1 && a[x][i] == 1)
			{
				cout<<"("<<x<<","<<i<<")";
				check[i] = 0;
				Push(q,i);
				flag = 1;
				break;
			}
		if(flag == 0)
			Pop(q);
			
	}
}
int main()
{
	int n;
	cout<<"nhap so luong diem\n";
	cin>>n;
	for(int i = 1;i <= n;i++)
		check[i] = 1;
	int a[max_size][max_size];
	cout<<"Nhap do thi\n";
	for(int i = 1;i<=n;i++)
		for(int j=1;j<=n;j++)
			cin>>a[i][j];
	DFS(a,n);
	return 0;
}
/*
0 1 0 1 0 0 0 0
1 0 1 0 1 0 0 0  
0 1 0 0 0 1 1 0  
1 0 0 0 0 1 0 0  
0 1 0 0 0 0 1 1  
0 0 1 1 0 0 1 1  
0 0 1 0 1 1 0 1  
0 0 0 0 1 1 1 0 
*/
