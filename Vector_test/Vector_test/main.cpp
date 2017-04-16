#define _CRT_SECURE_NO_WARNINGS
#include"CVector3.h"
#include<iostream>
#include<string>
#include<fstream>
#include<cstdlib>
#include <GL/glut.h>
#include "math.h"
using namespace std;
float z[3][100];
//string type;
int flag=0;
void int2(string type);
int g_fSeta = 33;
void myDisplay(void)
{
	//glRotatef(g_fSeta / 1000, 0, 1, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_LINE_STRIP);
	for (int i = 1; i < 4; i++)
	{
		glColor3f(1.0f, 1.0f, 1.0f);
		glVertex3f(0, 0, 0);
		//std::cout << z[0][i] << "," << z[1][i] << "," << z[2][i] << " ";
		if (i == 2)
		{
			glColor3f(1.0f, 0.0f, 0.0f);//����ֱ��
		}
		glVertex3f(z[0][i], z[1][i], z[2][i]);
	}
	glEnd();
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
	gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);//�����ӽ�
}
void int2(string type)
{
	CVector3 a;
	CVector3 b;
	memset(z, 0, sizeof(z));
	cout << type << endl;
	//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
	if (type == "�������")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 /12.0;
		//std::cout << z[0][1] << "," << z[1][1] << "," << z[2][1] << " ";
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1/12.0; z[1][2] = t2/12.0; z[2][2] = t3/12.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		a = a + b;//���
		printf("%.2f,%.2f,%.2f\n", a.m_dX, a.m_dY, a.m_dZ);
		z[0][3] = a.m_dX/12.0; z[1][3] = a.m_dY/12.0; z[2][3] = a.m_dZ/12.0;
	}
	else if (type == "�������")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1 / 12.0; z[1][2] = t2 / 12.0; z[2][2] = t3 / 12.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		a = a - b;//���
		printf("%.2f,%.2f,%.2f\n", a.m_dX, a.m_dY, a.m_dZ);
		z[0][3] = a.m_dX / 12.0; z[1][3] = a.m_dY / 12.0; z[2][3] = a.m_dZ / 12.0;
	}
	else if (type == "��������")
	{
		float t4,t1, t2, t3;
		char _c;
		scanf("%f%c", &t1, &_c);
		if (_c == ' '){
			scanf("%f,%f,%f", &t2, &t3,&t4);
			z[0][1] = t2 / 12.0; z[1][1] = t3 / 12.0; z[2][1] = t4 / 12.0;
			b.Set(t2, t3,t4);
			std::cout << t1 << " ";
			std::cout << t2 << "," << t3 << "," << t4 << " ";
			b = b*(float)t1;//����*����
			z[0][2] = b.m_dX / 12.0; z[1][2] = b.m_dY / 12.0; z[2][2] = b.m_dZ / 12.0;
		}
		else
		{
			scanf("%f,%f %f", &t2, &t3, &t4);
			b.Set(t1,t2, t3);
			z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
			std::cout << t1 << "," << t2 << "," << t3 << " ";
			std::cout << t4 << " ";
			b = b*(float)t4;//����*����
			z[0][2] = b.m_dX / 12.0; z[1][2] = b.m_dY / 12.0; z[2][2] = b.m_dZ / 12.0;
		}
		printf("%.2f,%.2f,%.2f\n", b.m_dX, b.m_dY, b.m_dZ);
	}
	else if (type == "�������")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1 / 12.0; z[1][2] = t2 / 12.0; z[2][2] = t3 / 12.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		if (a == b)
		{
			printf("true\n");//��ȷ���true
		}
		else
			printf("false\n");//���ȷ���false
	}
	else if (type == "��������")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1 / 12.0; z[1][2] = t2 / 12.0; z[2][2] = t3 / 12.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		if (a != b)
		{
			printf("true\n");//���ȷ���true
		}
		else
			printf("false\n");//��ȷ���false
	}
	else if (type == "������ֵ")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		b = a;//��a��ֵ��b
		printf("%.2f,%.2f,%.2f\n", b.m_dX, b.m_dY, b.m_dZ);
		z[0][2] = b.m_dX / 12.0; z[1][2] = b.m_dY / 12.0; z[2][2] = b.m_dZ / 12.0;
	}
	else if (type == "�������")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 16.0; z[1][1] = t2 / 16.0; z[2][1] = t3 / 16.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1 / 16.0; z[1][2] = t2 / 16.0; z[2][2] = t3 / 16.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		float ans = a.dotMul(b);//���
		printf("%.2f\n", ans);
	}
	else if (type == "�������")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 16.0; z[1][1] = t2 / 16.0; z[2][1] = t3 / 16.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1 / 16.0; z[1][2] = t2 / 16.0; z[2][2] = t3 / 16.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		a = a.crossMul(b);//���
		printf("%.2f,%.2f,%.2f\n", a.m_dX, a.m_dY, a.m_dZ);
		z[0][3] = a.m_dX / 12.0; z[1][3] = a.m_dY / 12.0; z[2][3] = a.m_dZ / 12.0;
	}
	else if (type == "������׼��")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		a.Normalize();//��������ģ
		printf("%.2f,%.2f,%.2f\n", a.m_dX, a.m_dY, a.m_dZ);
		z[0][3] = a.m_dX / 12.0; z[1][3] = a.m_dY / 12.0; z[2][3] = a.m_dZ / 12.0;
	}
	else if (type == "������ģ")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		float ans = a.len();//���㳤��
		printf("%.2f\n", ans);
	}
	else if (type == "����ͶӰ")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][1] = t1 / 12.0; z[1][1] = t2 / 12.0; z[2][1] = t3 / 12.0;
		a.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		//printf("%lf,%lf,%lf  ", (double)t1, (double)t2, (double)t3);
		scanf("%f,%f,%f", &t1, &t2, &t3);
		z[0][2] = t1 / 12.0; z[1][2] = t2 / 12.0; z[2][2] = t3 / 12.0;
		b.Set(t1, t2, t3);
		std::cout << t1 << "," << t2 << "," << t3 << " ";
		a = a.project(b);//aͶӰ��b��
		printf("%.2f,%.2f,%.2f\n", a.m_dX, a.m_dY, a.m_dZ);
		z[0][3] = a.m_dX / 12.0; z[1][3] = a.m_dY / 12.0; z[2][3] = a.m_dZ / 12.0;
	}
	else
	{
		printf("no!\n");
	}
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(400, 400);
	glutCreateWindow("OpenGL����");
	SetRC();
	freopen("test.txt", "r", stdin);//��ȡtest.txt
	freopen("out.txt", "w", stdout);//д�뵽out.txt
	glutKeyboardFunc(&myKeyboardFunc);
	glutDisplayFunc(&myDisplay);
	glutReshapeFunc(&myReshape);
	glutMainLoop();
	return 0;
}