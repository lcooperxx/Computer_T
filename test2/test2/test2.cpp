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
	glRotatef(g_fSeta / 1000, 0, 1, 0);//��ת
	if (flag == 0)//���ñ�Ǳ���flag flag==0ʱ ����
	{
		scatter();
	}
	else if (flag == 1)//flag==1ʱ����
	{
		Scatter_Line();
	}
	else if (flag == 2)//flag==3ʱ����������
	{
		Scatter_triangle();
	}
	glFlush();
}


void OnTimer(int val)
{
	g_fSeta = g_fSpeed;//ʱ�亯��
	Display();
	glutTimerFunc(g_fSeta, OnTimer, 1);
}

void Initial()
{
	int i;
	srand((unsigned)(time(NULL)));//��ʱ������������ӣ����������Ƕ�ֵ
	for (i = 0; i < n; i++)
	{
		z[0][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//����10 000�����x������
		z[1][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//����10 000�����y������
		z[2][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//����10 000�����z������
	}
	
	memset(visit, 0, sizeof(visit));
	visit[0] = 1;
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  //������ɫ  
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(1.0, 1.0, 1.0, 1.0);   //ͶӰ���ü�����С������  
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
