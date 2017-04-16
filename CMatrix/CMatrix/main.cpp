#include"stdafx.h"
#include"CMatrix.h"
#include"CVector3.h"
#include<iostream>
#include<string>
#include<cstdlib>
#include <GL/glew.h>
#include <GL/glut.h>
#include "math.h"
#pragma comment(lib,"glew32.lib ")

using namespace std;
int flag1 = 0;
void int2(string type);
void myDisplay(void)
{
	//glRotatef(g_fSeta / 1000, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glFlush();
}

void myKeyboardFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		
	case ' ':
		string type;
		cin >> type;
		if (type.empty())
			break;
		int2(type);
		myDisplay();
		break;
	}
}

void SetRC()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glLineWidth(1.0f);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0xFFFE);
	glewInit();
	float Quadratic[] = { 0, 0, 0.0001 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);
	glPointParameterf(GL_POINT_SIZE_MAX, 100);
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
	gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);//调整视角
}
void CM_print(CMatrix temp)
{
	for (int i = 0; i < 16; i++){
		if (!(i % 4) && i!=0)
			std::cout << endl;
		std::cout << temp[i] << " ";
	}
	printf("\n");
}

void int2(string type)
{
	cout << type << endl;
	CMatrix t1;
	CMatrix t2;
	if (type == "矩阵求逆")
	{
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f\n", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
			//temp[i] = 1;
		}
		t1.Set(temp);
		CM_print(t1);
		float h = Inverse(t1, N);
		if (0 == h)
		{
			printf("行列式=%.2f\n", h);
		}
		else
		{
			t2 = GetMatrixInverse(t1, N);
			std::cout << "逆矩阵：" << endl;
			CM_print(t2);
		}
	}
	else if (type == "矩阵相乘")
	{
		CMatrix t1;
		CMatrix t2;
		float temp[16], temp2[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		t1.Set(temp);
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f\n", &temp2[i]);
				break;
			}
			scanf("%f,", &temp2[i]);
		}
		t2.Set(temp2);
		t1 = t2*t1;
		CM_print(t1);
	}
	else if (type == "矩阵标准化")
	{
		CMatrix t1;
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		t1.Set(temp);
		t1.Identity();
		CM_print(t1);
	}
	else if (type == "矩阵标乘")
	{
		CMatrix t1;
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		t1.Set(temp);
		float t3;
		scanf("%f\n", &t3);
		t1 = t1 * t3;
		CM_print(t1);
	}
	else if (type == "矩阵乘向量")
	{
		float temp[16], p1, p2, p3;
		CMatrix t1;
		CVector3 p;
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		t1.Set(temp);
		scanf("%f,%f,%f\n", &p1, &p2, &p3);
		p.Set(p1, p2, p3, 0);
		p = t1.vecMul(p);
		printf("%.2f,%.2f,%.2f,%.2f\n", p.m_dX, p.m_dY, p.m_dZ, p.m_dT);
	}
	else if (type == "矩阵乘位置")
	{
		CMatrix t1;
		CVector3 p;
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		scanf("%f,%f,%f\n", &p.m_dX, &p.m_dY, &p.m_dZ);
		t1.Set(temp);
		p.Set(p.m_dX, p.m_dY, p.m_dZ, 1);
		p = t1.posMul(p);
		printf("%.2f,%.2f,%.2f,%.2f\n", p.m_dX, p.m_dY, p.m_dZ, p.m_dT);
	}
	else if (type == "矩阵转置")
	{
		float temp[16];
		CMatrix t1;
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		t1.Set(temp);
		t1.Transpose();
		CM_print(t1);
	}
	else if (type == "矩阵设置旋转")
	{
		CVector3 p;
		float seta,p1,p2,p3;
		scanf("%f", &seta);
		scanf("%f,%f,%f\n", &p1, &p2, &p3);
		p.Set(p1, p2, p3,1);
		CMatrix t1 = t1.SetRotate(60, p);
		CM_print(t1);
	}
	else if (type == "矩阵设置缩放1")
	{
		CVector3 p;
		float k, p1, p2, p3;
		scanf("%f", &k);
		scanf("%f,%f,%f\n", &p1, &p2, &p3);
		p.Set(p1, p2, p3, 1);
		CMatrix t1 = t1.SetScale1(k, p);
		CM_print(t1);
	}
	else if (type == "矩阵设置缩放")
	{
		CVector3 p;
		float p1, p2, p3;
		scanf("%f,%f,%f\n", &p1, &p2, &p3);
		p.Set(p1, p2, p3, 1);
		CMatrix t1;
		t1 = t1.SetScale(p);
		CM_print(t1);
	}
	else if (type == "矩阵设置平移")
	{
		CVector3 p;
		float p1, p2, p3;
		scanf("%f,%f,%f\n", &p1, &p2, &p3);
		p.Set(p1, p2, p3, 1);
		CMatrix t1;
		t1 = t1.SetTrans(p);
		CM_print(t1);
	}
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OpenGL程序");
	SetRC();
	freopen("test.txt", "r", stdin);//读取test.txt
	//freopen("out.txt", "w", stdout);//写入到out.txt
	glutKeyboardFunc(&myKeyboardFunc);
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(&myReshape);
	glutMainLoop();
	getchar();
	return 0;
}