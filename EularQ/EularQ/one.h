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
	GLfloat pointSize = 5.4f;//设置点的大小为5.4f
	glPointSize(pointSize);
	CMatrix t1;
	CVector3 p(0, 1, 0, 0);
	t1.SetRotate(Seta + 0.2, p).run();
	glBegin(GL_POINTS);
	for (i = 0; i < n; i++)
	{
		float temp = rand() / double(RAND_MAX);//随机亮度
		float temp2 = rand() / double(RAND_MAX);//随机颜色
		if (i < 3000)
		{
			glColor4f(0.0f, 1.0f, 0.0f, temp);//3000以内是绿色
		}
		else if (3000 < i < 6000)
		{
			glColor4f(0.0f, 0.0f, 1.0f, temp);//3000-6000是蓝色
		}
		else
		{
			glColor4f(temp2, temp2, temp2, temp);//涂色
		}
		glVertex3f(z[0][i], z[1][i], z[2][i]);//生成点
	}
	glEnd();
	//glFlush();
}

void Scatter_Line()
{
	int i;
	GLfloat dis_min, dis3;//最短距离 
	dis_min = 1e9;
	int m = -1;
	for (i = 1; i < 10001; i++){//设置10000个点
		if (visit[i] || site == i)//当前位置已经访问过或者当前位置和i一样
			continue;
		dis3 = sqrt((z[0][site] - z[0][i]) * (z[0][site] - z[0][i]) + (z[1][site] - z[1][i]) * (z[1][site] - z[1][i]) +
			(z[2][site] - z[2][i]) * (z[2][site] - z[2][i]));//求解最近距离
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
		visit[m] = 1;	//标记已访问过位置
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
		if (i == point.size() - 2)//这个只是一个附加功能：在最新的一条直线是红色的
		{
			r = 1.0f;
			b = 0.0f;
		}
		glColor3f(r, b, b);
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3fv(p);//直线起点
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		//t1 = t1.SetRotate(60, p).run();
		glVertex3fv(p);//直线终点
	}
	glEnd();
	glFlush();
}

void Scatter_triangle()
{
	int i;
	GLfloat dis_min, dis3;//最短距离 
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
			float c1 = (rand() % 100) / 100.0;//生成随机点，确保视角都可以看见
			float c2 = (rand() % 100) / 100.0;
			float c3 = (rand() % 100) / 100.0;
			glColor3f(c1, c2, c3);
		}
		else
		{
			glColor3f(0.5f, 0.5f, 0.5f);
		}
		p.Set(z[0][point[i - 1]], z[1][point[i - 1]], z[2][point[i - 1]], 0);
		glVertex3fv(p);//三角形第一个点
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3fv(p);//三角形第二个点
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		glVertex3fv(p);//三角形第三个点
	}
	glEnd();
	glFlush();
}
#endif;