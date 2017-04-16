#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <glut.h>
#include <time.h>
#include<vector>
#include <Windows.h>
#include"one.h"
using namespace std;

//float g_fSpeed = 33;
//float g_fSeta = 33;
//
//GLfloat z[3][10010];
//int visit[10001];
//int flag = 0;
//const int n = 10001;
//vector<int> point;
//
//int site = 0;

void Initial();
void SetRC()
{
	Initial();
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Display()
{
	glRotatef(g_fSeta / 1000, 0, 1, 0);//旋转
	if (flag == 0)//设置标记变量flag flag==0时 画点
	{
		scatter();
	}
	else if (flag == 1)//flag==1时画线
	{
		Scatter_Line();
	}
	else if (flag == 2)//flag==3时，画三角形
	{
		Scatter_triangle();
	}
	glFlush();
}


void OnTimer(int val)
{
	g_fSeta = g_fSpeed;//时间函数
	Display();
	glutTimerFunc(g_fSeta, OnTimer, 1);
}

void Initial()
{
	int i;
	srand((unsigned)(time(NULL)));//按时间生成随机种子，且数量不是定值
	for (i = 0; i < n; i++)
	{
		z[0][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的x轴坐标
		z[1][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的y轴坐标
		z[2][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的z轴坐标
	}
	
	memset(visit, 0, sizeof(visit));
	visit[0] = 1;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  //清屏颜色  
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(1.0, 1.0, 1.0, 1.0);   //投影到裁剪窗大小：世界  
	//gluOrtho2D(0.0, 1.0, 0.0, 1.0);
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)      
	{  
	case ' ': 
		flag ++;
		if (flag == 3)
		{
			flag = 0;
		}
		printf("\n");
		printf("%d\n", flag);
		break;
	case '1':
		g_fSpeed *= 2.0;
		break;
	case '4':
		g_fSpeed *= 0.5;
		break;
	}
	//std::cout << g_fSpeed;
}

void myReshape(int w, int h)
{
	GLfloat nRange = 100.0f;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, GLfloat(w) / h, 1, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scatter Plot");
	SetRC();
	glutDisplayFunc(&Display);
	glutTimerFunc(g_fSeta, OnTimer, 1);
	glutKeyboardFunc(&myKeyboardFunc);
	glutReshapeFunc(&myReshape);
	glutMainLoop();
	return 0;
}
