#include "stdafx.h"
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <glut.h>
#include <time.h>
#include <vector>
#include <Windows.h>
#include "CMatrix.h"
#include "CVector3.h"
#ifndef one_H
#define one_H
using namespace std;

float g_fSpeed = 33;
float g_fSeta = 33;
float Seta = 6;
GLfloat z[3][10010];
int visit[10001];
int flag = 0;
const int n = 10001;
vector<int> point;
int site = 0;

void scatter()
{
	int i;
	glClear(GL_COLOR_BUFFER_BIT);
	GLfloat pointSize = 5.4f;//���õ�Ĵ�СΪ5.4f
	glPointSize(pointSize);
	CMatrix t1;
	CVector3 p(0, 1, 0, 0);
	t1.SetRotate(Seta + 0.2, p).run();
	glBegin(GL_POINTS);
	for (i = 0; i < n; i++)
	{
		float temp = rand() / double(RAND_MAX);//�������
		float temp2 = rand() / double(RAND_MAX);//�����ɫ
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

void Scatter_Line()
{
	int i;
	GLfloat dis_min, dis3;//��̾��� 
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
		float r = 0.5f;
		float b = 0.5f;
		if (i == point.size() - 2)//���ֻ��һ�����ӹ��ܣ������µ�һ��ֱ���Ǻ�ɫ��
		{
			r = 1.0f;
			b = 0.0f;
		}
		glColor3f(r, b, b);
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3fv(p);//ֱ�����
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		//t1 = t1.SetRotate(60, p).run();
		glVertex3fv(p);//ֱ���յ�
	}
	glEnd();
	glFlush();
}

void Scatter_triangle()
{
	int i;
	GLfloat dis_min, dis3;//��̾��� 
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
			float c1 = (rand() % 100) / 100.0;//��������㣬ȷ���ӽǶ����Կ���
			float c2 = (rand() % 100) / 100.0;
			float c3 = (rand() % 100) / 100.0;
			glColor3f(c1, c2, c3);
		}
		else
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		p.Set(z[0][point[i - 1]], z[1][point[i - 1]], z[2][point[i - 1]], 0);
		glVertex3fv(p);//�����ε�һ����
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3fv(p);//�����εڶ�����
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		glVertex3fv(p);//�����ε�������
	}
	glEnd();
	glFlush();
}
#endif;