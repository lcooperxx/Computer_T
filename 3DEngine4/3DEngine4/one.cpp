#include"stdafx.h"
#include"total.h"
#include"gl/glut.h"
#include"gl/gl.h"
#include"math.h"
#include"time.h"
#include"stdlib.h"
void One::Scat_Initial()
{
	int i;
	srand((unsigned)(time(NULL)));//��ʱ������������ӣ����������Ƕ�ֵ
	for (i = 0; i < 10001; i++)
	{
		z[0][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//����10 000�����x������
		z[1][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//����10 000�����y������
		z[2][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//����10 000�����z������
	}
	memset(visit, 0, sizeof(visit));
	visit[0] = 1;
	gluOrtho2D(1.0, 1.0, 1.0, 1.0);   //ͶӰ���ü�����С������
}
void One::scatter()
{
	int i;
	//glClear(GL_COLOR_BUFFER_BIT);
	GLfloat pointSize = 5.4f;//���õ�Ĵ�СΪ5.4f
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	for (i = 0; i < 10001; i++)
	{
		double temp = rand() / double(RAND_MAX);//�������
		double temp2 = rand() / double(RAND_MAX);//�����ɫ
		if (i < 3000)
		{
			glColor4f(0.0f, 1.0f, 0.0f, temp);//3000��������ɫ
		}
		else if (3000 < i < 6000)
		{
			glColor4f(0.0f, 0.0f, 1.0f, temp);//3000-6000����ɫ
		}
		else
		{
			glColor4f(temp2, temp2, temp2, temp);//Ϳɫ
		}
		glVertex3f(z[0][i], z[1][i], z[2][i]);//���ɵ�
	}
	glEnd();
	//glFlush();
}

void One::Scatter_Line()
{
	int i;
	GLdouble dis_min, dis3;//��̾��� 
	dis_min = 1e9;
	int m = -1;
	for (i = 1; i < 10001; i++){//����10000����
		if (visit[i] || site == i)//��ǰλ���Ѿ����ʹ����ߵ�ǰλ�ú�iһ��
			continue;
		dis3 = sqrt((z[0][site] - z[0][i]) * (z[0][site] - z[0][i]) + (z[1][site] - z[1][i]) * (z[1][site] - z[1][i]) +
			(z[2][site] - z[2][i]) * (z[2][site] - z[2][i]));//����������
		if (dis3 < dis_min)
		{
			dis_min = dis3;
			m = i;
		}
	}
	site = m;
	if (m != -1)
	{
		point.push_back(m);
		visit[m] = 1;	//����ѷ��ʹ�λ��
	}
	/*cout << m<<"======="<<visit[m] << endl;*/
	glClear(GL_COLOR_BUFFER_BIT);
	glLineStipple(1, 0x0F0F);
	glLineWidth(2.0f);
	CVector3 p;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < point.size() - 1; i++){
		double r = 0.5f;
		double b = 0.5f;
		if (i == point.size() - 2)//���ֻ��һ�����ӹ��ܣ������µ�һ��ֱ���Ǻ�ɫ��
		{
			r = 1.0f;
			b = 0.0f;
		}
		glColor3f(r, b, b);
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3dv(p);//ֱ�����
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		//t1 = t1.SetRotate(60, p).run();
		glVertex3dv(p);//ֱ���յ�
	}
	glEnd();
	glFlush();
}

void One::Scatter_triangle()
{
	int i;
	GLdouble dis_min, dis3;//��̾��� 
	dis_min = 1e9;
	int m = -1;
	for (i = 1; i < 10001; i++){
		if (visit[i] || site == i)
			continue;
		dis3 = sqrt((z[0][site] - z[0][i]) * (z[0][site] - z[0][i]) + (z[1][site] - z[1][i]) * (z[1][site] - z[1][i]) +
			(z[2][site] - z[2][i]) * (z[2][site] - z[2][i]));
		if (dis3 < dis_min)
		{
			dis_min = dis3;
			m = i;
		}
	}
	site = m;
	if (m != -1)
	{
		point.push_back(m);
		visit[m] = 1;
	}
	//cout << m << "=======" << visit[m] << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	glLineStipple(1, 0x0F0F);
	glLineWidth(2.0f);
	CVector3 p;
	glBegin(GL_LINE_STRIP);
	for (int i = 1; i < point.size() - 1; i++){
		//
		//glEnable(GL_LINE_STIPPLE);
		glLineStipple(1, 0x0F0F);
		glLineWidth(1.0f);
		glBegin(GL_TRIANGLE_STRIP);
		if (200 < i < 300)
		{
			double c1 = (rand() % 100) / 100.0;//��������㣬ȷ���ӽǶ����Կ���
			double c2 = (rand() % 100) / 100.0;
			double c3 = (rand() % 100) / 100.0;
			glColor3f(c1, c2, c3);
		}
		else
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		p.Set(z[0][point[i - 1]], z[1][point[i - 1]], z[2][point[i - 1]], 0);
		glVertex3dv(p);//�����ε�һ����
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3dv(p);//�����εڶ�����
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		glVertex3dv(p);//�����ε�������
	}
	glEnd();
	glFlush();
}