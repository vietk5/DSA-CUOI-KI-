#include<graphics.h>
#include<iostream>
#include <math.h>
#include <sstream>
#include<string>
#define Round(a) (int)(a+0.5)   
#define max(a,b) (a>b)?a:b
#define max_size 100
#define N 8
int rout = 4;
int graph[8][8] = { 
        {0, 1, 0, 1, 0, 0, 0, 0}, 
        {1, 0, 1, 0, 1, 0, 0, 0},  
        {0, 1, 0, 0, 0, 1, 1, 0},  
        {1, 0, 0, 0, 0, 1, 0, 0},  
        {0, 1, 0, 0, 0, 0, 1, 1},  
        {0, 0, 1, 1, 0, 0, 1, 1},  
        {0, 0, 1, 0, 1, 1, 0, 1},  
        {0, 0, 0, 0, 1, 1, 1, 0},  
    };
using namespace std;
bool check[max_size];
struct Point{
	int x,y;
};
Point rec[N+1];
char* int_to_char(int x) {
    static char buffer[20];  
    sprintf(buffer, "%d", x);  
    return buffer;
}
void DrawPoint(Point &x,int value,int color=WHITE){ 
	setcolor(BLACK);
	circle(x.x, x.y, 30);
	setfillstyle(SOLID_FILL, color);        
    fillellipse(x.x,x.y, 30, 30);  
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
	outtextxy(x.x-15,x.y-10, int_to_char(value));
}
void lineDDA(Point p1, Point p2, int thickness,int Delay,int color) {
    int dx = p2.x - p1.x, dy = p2.y - p1.y;
    float x_inc, y_inc;
    float dis = sqrt(pow(dx,2) + pow(dy,2));
    float tile = 30/dis;
    float Dx = dx - 2*dx*tile;
	float Dy = dy - 2*dy*tile;
	float step = max(abs(Dx), abs(Dy));
	x_inc = Dx / step; 
    y_inc = Dy / step; 
    if (step <= 0) step = 1;                                                     
    for (int i = -thickness / 2; i <= thickness / 2; i++) {
        int offset = i;
		float x = p1.x + dx*tile , y = p1.y + dy*tile;
        int k = 1;
        while (k <= step) {
            k++;
            delay(Delay);  
            x += x_inc;
            y += y_inc;

            if(dx == 0)
            	putpixel(round(x + offset), round(y), color);
            else
            	putpixel(round(x), round(y+ offset-2), color);
    

        }
    }
}
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
void DFS(Point point[N])
{
	Stack q;
	CreateStack(q);
//	cin>>rout;
	Push(q,rout);
	outtextxy(rec[0].x+80,rec[0].y+20, int_to_char(rout));
	check[rout] = 1;
	DrawPoint(point[rout],rout,RED);
	delay(100);
	while(!EmptyStack(q))
	{
		int flag = 0;
		int x = Top(q);
		DrawPoint(point[x],x,GREEN);
		outtextxy(rec[q.top].x+120,rec[q.top].y+20, "<-");
		delay(700);
		if(check[x])
			if(x == rout)
				DrawPoint(point[x],x,RED);
			else
				DrawPoint(point[x],x,YELLOW);
		for(int i=0; i<N;i++)
			if(check[i] == 0 && graph[x][i] == 1)
			{
				delay(500);
				DrawPoint(point[i],i,YELLOW);
				lineDDA(point[x],point[i],5,0.5,RED);
				check[i] = 1;
				Push(q,i);
				outtextxy(rec[q.top].x+80,rec[q.top].y+20, int_to_char(i));
				flag = 1;
				break;
			}
		if(flag == 0)
		{
				delay(200);
				outtextxy(rec[q.top].x+80,rec[q.top].y+20, "    ");
				Pop(q);
		}
		outtextxy(rec[q.top-1].x+120,rec[q.top-1].y+20, "  ");
			
	}
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 4);
		outtextxy(600,700, "END PROGRAM");
}
int main()
{
	Point point[N+1];
	int x[8]={100,200,250,200,400,400,450,600};
	int y[8]={300,150,300,450,150,450,300,300};
	int gd=0,gm=0, maloi; 
    initgraph(&gd,&gm,"");
    initwindow(1920, 1080, "Fullscreen Graphics");
    setbkcolor(WHITE);
   	cleardevice();
	for(int i=0;i<N;i++)
	{
		check[i]=0;
		point[i].x=x[i];
		point[i].y=y[i];
		DrawPoint(point[i],i);
		
	}
	for(int i=0;i<N;i++)
		for(int j=0;j<N;j++)
		{
			if(graph[i][j] == 1)
				lineDDA(point[i],point[j],1,0,BLACK);
		}
	settextstyle(TRIPLEX_FONT, HORIZ_DIR, 6);
	outtextxy(100,50, "DFS");
	settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
	outtextxy(820,70, "STACK");
	int step = 0;
	for(int i=0; i<N+1 ;i++)
	{
		rec[i].x = 800;
		rec[i].y = 100 + step;
		step +=50 ;
		rectangle(rec[i].x, rec[i].y, rec[i].x+200, rec[i].y+50);
	}
	DFS(point);
	getch();
	closegraph();
	return 0;
}
