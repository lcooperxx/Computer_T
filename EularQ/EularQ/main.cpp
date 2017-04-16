#define _CRT_SECURE_NO_WARNINGS
#include "stdafx.h"
#include <stdio.h>
#include <GL/glew.h>
#include <stdlib.h>
#include <GL/glut.h>
#include "math.h"
#include <string>
#include <iostream>
#include <time.h>
#include <vector>
#include <Windows.h>
#pragma comment(lib,"glew32.lib ")
#include "one.h"
#include "CQuaternion.h"

using namespace std;
vector<CVector3> path;
vector<CVector3> pos;

CMatrix s_pos1,s_pos2;
CEuler s_hpr1,s_hpr2,s_hpr3;
CQuaternion s_cq1, s_cq2, s_result;
CVector3 s_pf, s_pn, s_presult;
float s_t = 2.0;
float seta = 0;
float axis = 0;
//视点位置和方向
float mx = 0, my = 0, mz = 0.5, rx = 0, ry = 0, rz = 0;//平移和旋转
float sx = 1, sy = 1, sz = 1;//缩放
float mspeed = 0.05, rspeed = 0.5;
float g_IEyeMatt[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, g_EyeMatt[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
CMatrix g_IEyeMat, g_EyeMat;
int mode = 0;
float Max(float a, float b)
{
	return a > b ? a : b;
}
CEuler ToEuler(CVector3 ptest){
	float h, p, b;
	CVector3 p1, p2, p3;
	CEuler e1;
	p1.Set(ptest.m_dX, 0, ptest.m_dZ, 0);
	p2.Set(ptest.m_dX, ptest.m_dY, 0, 0);
	p3.Set(0, ptest.m_dY, ptest.m_dZ, 0);

	h = acosf(p1.dotMul(CVector3(0, 0, -1, 0)) / (p1.len())) * 180 / 3.14;
	p = acosf(ptest.dotMul(p1) / (ptest.len() * p1.len())) * 180 / 3.14;
	b = 0;//acosf(p3.dotMul(CVector3(0, 0, -1, 0)) / (p3.len())) * 180 / 3.14;
	if (p1.m_dX > 0)
	{
		h = -h;
	}
	if (p1.m_dY < 0)
	{
		p = -p;
	}
	e1.Set(h, p, b);
	return e1;
}
CQuaternion ToQuaternion(CMatrix t1)//矩阵转换为四元数
{
	CQuaternion cq1;
	cq1.w = sqrt(t1.m00 + t1.m11 + t1.m22 + 1) / 2.0f;
	cq1.x = sqrt(t1.m00 - t1.m11 - t1.m22 + 1) / 2.0f;
	cq1.y = sqrt(-t1.m00 + t1.m11 - t1.m22 + 1) / 2.0f;
	cq1.z = sqrt(-t1.m00 - t1.m11 + t1.m22 + 1) / 2.0f;
	if (cq1.w > Max(cq1.x, Max(cq1.y, cq1.z)))
	{
		float w = (4 * cq1.w);
		cq1.x = (t1.m12 - t1.m21) / w;
		cq1.y = (t1.m20 - t1.m02) / w;
		cq1.z = (t1.m01 - t1.m10) / w;//error1
	}
	else if (cq1.x > Max(cq1.w, Max(cq1.y, cq1.z)))
	{
		cq1.w = (t1.m21 - t1.m12) / (4 * cq1.x);
		cq1.y = (t1.m01 + t1.m10) / (4 * cq1.x);
		cq1.z = (t1.m20 + t1.m02) / (4 * cq1.x);
	}
	else if (cq1.y > Max(cq1.w, Max(cq1.x, cq1.z)))
	{
		cq1.w = (t1.m02 - t1.m20) / (4 * cq1.y);
		cq1.x = (t1.m01 + t1.m10) / (4 * cq1.y);
		cq1.z = (t1.m12 + t1.m21) / (4 * cq1.y);
	}
	else if (cq1.z > Max(cq1.w, Max(cq1.y, cq1.x)))
	{
		cq1.w = (t1.m10 - t1.m01) / (4 * cq1.z);
		cq1.x = (t1.m20 + t1.m02) / (4 * cq1.z);
		cq1.y = (t1.m12 + t1.m21) / (4 * cq1.z);
	}
	cq1.Normalize();
	return cq1;
}
CQuaternion ToQuaternion(CEuler e1)//欧拉角转换为四元数
{
	CQuaternion cq1;
	cq1.w = cosf((e1.h / 2.0) * 3.14 / 180) * cosf((e1.p / 2.0) * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180)
		+ sinf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180);
	cq1.x = cosf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180)
		+ sinf(e1.h / 2.0 * 3.14 / 180) * cosf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180);
	cq1.y = sinf(e1.h / 2.0 * 3.14 / 180) * cosf(e1.p / 2.0 * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180)
		- cosf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180);
	cq1.z = cosf(e1.h / 2.0 * 3.14 / 180) * cosf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180)
		- sinf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180);
	cq1.Normalize();
	return cq1;
}

CEuler ToEuler(CMatrix t1)
{
	CEuler e1;
	e1.p = (asinf(-t1.m21)) * 360 / (2 * 3.14);
	if (cosf(e1.p) == 0)
	{
		e1.h = (acosf(t1.m00)) * 180 / 3.14;
		e1.b = 0;
	}
	else
	{
		e1.h = (atan2f(t1.m20, t1.m22)) * 180 / 3.14;
		e1.b = (atan2f(t1.m01, t1.m11)) * 180 / 3.14;
	}
	return e1;
}
void CM_print(CMatrix temp)
{
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
		{
			if (!((i * 4 + j) % 4) && i != 0)
				std::cout << endl;
			std::cout << temp[i + 4 * j] << " ";
		}

	}
	printf("\n");
}
void int2(string type)
{
	cout << type << endl;
	if (type == "向量转换欧拉角")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CVector3 p(t1, t2, t3, 0);
		CEuler e1;
		e1 = ToEuler(p);
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "欧拉角转换向量")/*error1*/
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		//e1.h*3.14 / 180; e1.p*3.14 / 180; e1.b*3.14 / 180;
		CVector3 p;
		CVector3 updir;
		p = e1.ToVector3(&updir);
		printf("%.2f,%.2f,%.2f\n", p.m_dX, p.m_dY, p.m_dZ);
	}
	else if (type == "欧拉角转换四元数")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		CQuaternion cq1;
		cq1 = ToQuaternion(e1);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数转换欧拉角")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		CEuler e1;
		e1 = cq1.ToEuler();
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "欧拉角转换矩阵")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		CMatrix cm1;
		cm1 = e1.ToMatrix();
		CM_print(cm1);
	}
	else if (type == "矩阵转换欧拉角")
	{
		CMatrix cm1;
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
		cm1.Set(temp);
		CEuler e1;
		e1 = ToEuler(cm1);
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "矩阵转换四元数")
	{
		CMatrix cm1;
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f\n", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		cm1.Set(temp);
		CQuaternion cq1;
		cq1 = ToQuaternion(cm1);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.x, cq1.y, cq1.z, cq1.w);
	}
	else if (type == "四元数转换矩阵")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		CMatrix cm1;
		cm1 = cq1.ToMatrix();
		//cm1.Transpose();
		CM_print(cm1);
	}
	else if (type == "欧拉角标准化")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		e1.Normal();
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "四元数单位化")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		cq1.Normalize();
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.x, cq1.y, cq1.z,cq1.w);
	}
	else if (type == "四元数相乘")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		cq1 = cq1 * cq2;
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.x, cq1.y, cq1.z, cq1.w);
	}
	else if (type == "四元数求差")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		cq1.Div(cq2);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.x, cq1.y, cq1.z, cq1.w);
	}
	else if (type == "四元数点乘")
	{
		float t1, t2, t3, t4, t5;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		t5 = cq1.dotMul(cq2);
		printf("%.2f\n", t5);
	}
	else if (type == "四元数求逆")
	{
		float t1, t2, t3, t4, t5;
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		cq1 = cq1.Inverse();
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.x, cq1.y, cq1.z, cq1.w);
	}
	else if (type == "四元数求角度和旋转轴")
	{
		float t1, t2, t3, t4, angle;
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f", &angle);
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CVector3 p(t1, t2, t3, 0);
		cq1.GetAngle(angle, p);
		printf("angle=%.2f,p:%.2f,%.2f,%.2f\n", angle, p.m_dX, p.m_dY, p.m_dZ);
	}
	else if (type == "四元数插值")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		float t;
		scanf("%f", &t);
		CQuaternion cq3;
		cq3 = cq1.Slerp(cq2, t);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq3.x, cq3.y, cq3.z, cq3.w);
	}
}
void Initial();//初始化随机
void Scat();//画线，点，面
void Scat_Path();//画轨迹
void Scat_Jump();
void Reverse_Pos();
void Save_Pos();
void SetView()//设置视野
{
	if (mode == 0)
	{
		glLoadMatrixf(g_EyeMat);
	}
	else
	{
		CMatrix t1;
		CVector3 p(0, 0, 1, 0);
		t1.SetRotate(-rz*3.14 / 180, p).run();
		//glRotatef(-rz, 0, 0, 1);
		p.Set(1, 0, 0, 0);
		t1.SetRotate(-rx*3.14 / 180, p).run();
		//glRotatef(-rx, 1, 0, 0);
		p.Set(0, 1, 0, 0);
		t1.SetRotate(-ry*3.14 / 180, p).run();
		//glRotatef(-ry, 0, 1, 0);
		p.Set(-mx, -my, -mz, 0);
		//p.Set(0, 0, 0, 0);
		t1.SetTrans(p).run();
		//glTranslatef(-mx, -my, -mz);
		glGetFloatv(GL_MODELVIEW, g_EyeMat);
	}
}
void RenderWorld()//坐标轴
{
	//绘制坐标轴
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	CVector3 p(0, 0, 0, 0);
	glVertex3fv(p);
	p.Set(1, 0, 0, 0);
	glVertex3fv(p);
	glColor3f(0.0f, 1.0f, 0.0f);
	p.Set(0, 0, 0, 0);
	glVertex3fv(p);
	p.Set(0, 1, 0, 0);
	glVertex3fv(p);
	glColor3f(0.0f, 0.0f, 1.0f);
	p.Set(0, 0, 0, 0);
	glVertex3fv(p);
	p.Set(0, 0, 1, 0);
	glVertex3fv(p);
	glEnd();
	//glFlush();
}
void Display()
{
	//glRotatef(g_fSeta/100, 0, 1, 0);
	CMatrix t1;
	CVector3 p(0, 1, 0, 0);
	//t1.SetRotate(Seta + 0.2, p).run();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	if (mode == 0)
	{
		if (s_t < 1)
		{
			s_t += 0.01;
			s_presult = s_pf.lerp(s_pn, s_t);

			s_result = s_cq1.Slerp(s_cq2, s_t);
			g_IEyeMat = s_result.ToMatrix();
			g_IEyeMat[12] = s_presult.m_dX;g_IEyeMat[13] = s_presult.m_dY;g_IEyeMat[14] = s_presult[14];
			g_EyeMat = GetInverse(g_IEyeMat, 4);

			CEuler s_e1;
			s_e1 = s_result.ToEuler();
			printf("f5转欧拉角:%.2f,%.2f,%.2f\n", s_e1.b, s_e1.p, s_e1.b);
		}
	}
	else
	{
		if (s_t < 1)
		{
			s_t += 0.01;
			s_presult = s_pf.lerp(s_pn, s_t);
			mx = s_presult.m_dX; my = s_presult.m_dY; mz = s_presult.m_dZ;

			s_result = s_cq1.Slerp(s_cq2, s_t);
			s_hpr3 = s_result.ToEuler();
			rx = s_hpr3.b;ry = s_hpr3.h; rz = s_hpr3.p;
		}
	}
	SetView();
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
	RenderWorld();
	Scat_Path();
	glPopMatrix();
	glutSwapBuffers();
	//glFlush();
}
void OnTimer(int val)
{
	g_fSeta = g_fSpeed;//时间函数
	Display();
	glutTimerFunc(g_fSeta, OnTimer, 1);
}
void Initial()
{
	g_IEyeMat.Set(g_IEyeMatt);
	g_EyeMat.Set(g_EyeMatt);
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
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);  //清屏颜色
	//glMatrixMode(GL_PROJECTION);
	gluOrtho2D(1.0, 1.0, 1.0, 1.0);   //投影到裁剪窗大小：世界
	//gluOrtho2D(0.0, 1.0, 0.0, 1.0);
}
void changeT(CVector3 p)
{
	CMatrix t1;
	g_EyeMat = g_EyeMat * t1.SetTrans(p);
}
void changeR(float fi, CVector3 p)
{
	CMatrix t1;
	g_EyeMat = g_EyeMat * t1.SetRotate(fi, p);
}
void processSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F2:
		std::cout << "change direction mode" << mode << std::endl;
		mode = mode == 0 ? 1 : 0;
		break;
	case GLUT_KEY_F3:
		std::cout << "F3" << std::endl;
		Save_Pos();
		break;
	case GLUT_KEY_F4:
		std::cout << "F4" << std::endl;
		Reverse_Pos();
		break;
	case GLUT_KEY_F5:
		std::cout << "F5" << std::endl;
		Scat_Jump();
		break;
	default:
		break;
	}

}
void Save_Pos()
{
		s_pos1 = g_EyeMat;
		s_pf.Set(mx, my, mz,0);
		s_hpr1.Set(rx, ry, rz);
		printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", mx, my, mz, rx, ry, rz);
		//CM_print(posi);
}
void Reverse_Pos()
{
	CMatrix r_posi;
	r_posi = s_pos1;
	//CM_print(r_posi);
	g_EyeMat = r_posi;
	mx = s_pf.m_dX;
	my = s_pf.m_dY;
	mz = s_pf.m_dZ;
	rx = s_hpr1.h;
	ry = s_hpr1.p;
	rz = s_hpr1.b;
	printf("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n", mx, my, mz, rx, ry, rz);
}
void Scat_Jump()
{
	
	if (mode == 0)
	{
		s_pos2 = GetInverse(g_EyeMat, 4);
		s_pn.m_dX = s_pos2[12]; s_pn.m_dY = s_pos2[13]; s_pn.m_dZ = s_pos2[14];
		s_cq2 = ToQuaternion(GetInverse(g_EyeMat, 4));//当前四元数
		Reverse_Pos();
		s_pos1 = GetInverse(s_pos1, 4);
		s_pf.m_dX = s_pos1[12]; s_pf.m_dY = s_pos1[13]; s_pf.m_dZ = s_pos1[14];
		s_cq1 = ToQuaternion(s_pos1);//之前四元数
		s_t = 0;
	}
	else
	{
		s_hpr2.h = ry; s_hpr2.p = rz; s_hpr2.b = rx;//当前欧拉角
		s_pn.m_dX = mx; s_pn.m_dY = my; s_pn.m_dZ = mz;//当前位置
		s_cq2 = ToQuaternion(s_hpr2);
		s_cq1 = ToQuaternion(s_hpr1);
		s_t = 0;
	}
}
void myKeyboardFunc1(unsigned char key, int x, int y)
{
	bool bChange = false;
	switch (key)
	{
	case ' ':
		flag++;
		if (flag == 3)
		{
			flag = 0;
		}
		break;
	case 'w':
		if (mode == 0)
		{
			CVector3 p;
			p.Set(0, -mspeed, 0, 0);
			changeT(p);
		}
		else
		{
			mx -= g_IEyeMat[4] * mspeed;
			my -= g_IEyeMat[5] * mspeed;
			mz -= g_IEyeMat[6] * mspeed;
			path.push_back(CVector3(mx, my, mz, 0));
		}
		break;
	case 's':
		//my-=mspeed;
		if (mode == 0)
		{
			CVector3 p;
			p.Set(0, mspeed, 0, 0);
			changeT(p);
		}
		else
		{
			mx += g_IEyeMat[4] * mspeed;
			my += g_IEyeMat[5] * mspeed;
			mz += g_IEyeMat[6] * mspeed;
			path.push_back(CVector3(mx, my, mz, 0));
		}

		break;
	case 'a':
		//mx-=mspeed;
		if (mode == 0)
		{
			CVector3 p;
			p.Set(mspeed, 0, 0, 0);
			changeT(p);
		}
		else
		{
			mx -= g_IEyeMat[0] * mspeed;
			my -= g_IEyeMat[1] * mspeed;
			mz -= g_IEyeMat[2] * mspeed;
			path.push_back(CVector3(mx, my, mz, 0));
		}

		break;
	case 'd':
		//mx+=mspeed;
		if (mode == 0)
		{
			CVector3 p;
			p.Set(-mspeed, 0, 0, 0);
			changeT(p);
		}
		else
		{
			mx += g_IEyeMat[0] * mspeed;
			my += g_IEyeMat[1] * mspeed;
			mz += g_IEyeMat[2] * mspeed;
			path.push_back(CVector3(mx, my, mz, 0));
		}

		break;
	case 'q':
		if (mode == 0)
		{
			CVector3 p;
			p.Set(0, 0, mspeed, 0);
			changeT(p);
		}
		else
		{
			mx += g_IEyeMat[8] * mspeed;
			my += g_IEyeMat[9] * mspeed;
			mz += g_IEyeMat[10] * mspeed;
			path.push_back(CVector3(mx, my, mz, 0));
		}
		//mz-=mspeed;

		break;
	case 'e':
		if (mode == 0)
		{
			CVector3 p;
			p.Set(0, 0, -mspeed, 0);
			changeT(p);
		}
		else
		{
			mx -= g_IEyeMat[8] * mspeed;
			my -= g_IEyeMat[9] * mspeed;
			mz -= g_IEyeMat[10] * mspeed;
			path.push_back(CVector3(mx, my, mz, 0));
		}
		break;
	case 'i':
		if (mode == 0)
		{
			CVector3 p;
			p.Set(1, 0, 0, 0);
			float fi = -rspeed;
			changeR(fi, p);
		}
		else
		{
			rx += rspeed;
			bChange = true;
		}
		break;
	case 'k':
		if (mode == 0)
		{

			CVector3 p(1, 0, 0, 0);
			float fi = rspeed;
			changeR(fi, p);
		}
		else
		{
			rx -= rspeed;
			bChange = true;
		}

		break;
	case 'j':
		if (mode == 0)
		{
			CVector3 p(0, 1, 0, 0);
			float fi = -rspeed;
			changeR(fi, p);
		}
		else
		{
			ry += rspeed;
			bChange = true;
		}

		break;
	case 'l':
		if (mode == 0)
		{
			CVector3 p(0, 1, 0, 0);
			float fi = rspeed;
			changeR(fi, p);
		}
		else
		{
			ry -= rspeed;
			bChange = true;
		}
		break;
	case 'u':
		if (mode == 0)
		{
			CVector3 p(0, 0, 1, 0);
			float fi = rspeed;
			changeR(fi, p);
		}
		else
		{
			rz += rspeed;
			bChange = true;
		}

		break;
	case 'o':
		if (mode == 0)
		{
			CVector3 p(0, 0, 1, 0);
			float fi = -rspeed;
			changeR(fi, p);
		}
		else
		{
			rz -= rspeed;
			bChange = true;
		}

		break;
	case '1':
		sx += 0.1;
		break;
	case '2':
		sx -= 0.1;
		break;
	case '3':
		sy += 0.1;
		break;
	case '4':
		sy -= 0.1;
		break;
	case '5':
		sz += 0.1;
		break;
	case '6':
		sz -= 0.1;
		break;
	case '0':
		mode = 1 - mode;
		if (mode == 0)
		{
			//g_EyeMat.Set(g_EyeMatt);
			glPushMatrix();
			glLoadIdentity();
			CMatrix t1;
			t1.SetRotate(-rz, CVector3(0, 0, 1, 0));
			t1.SetRotate(-rx, CVector3(1, 0, 0, 0));
			//glRotatef(-rz, 0, 0, 1);
			//lRotatef(-rx, 1, 0, 0);
			//glRotatef(-ry, 0, 1, 0);
			t1.SetRotate(-ry, CVector3(0, 1, 0,0));
			glTranslatef(-mx, -my, -mz);
			glGetFloatv(GL_MODELVIEW_MATRIX, g_EyeMat);
			glPopMatrix();
		}
		printf("mode:%d\n", mode);
		break;
	case '=':
		mspeed *= 1.1;
		printf("mspeed:%.1f\n", mspeed);
		break;
	case '-':
		mspeed *= 0.9;
		printf("mspeed:%.1f\n", mspeed);
		break;
	}
	if (mode == 0)
	{
		g_IEyeMat = GetInverse(g_EyeMat, N);
		mx = g_IEyeMat.m03;
		my = g_IEyeMat.m13;
		mz = g_IEyeMat.m23;
		path.push_back(CVector3(mx, my, mz, 0));
	}
	if (bChange)//计算视点矩阵的逆矩阵
	{
		glPushMatrix();
		glLoadIdentity();
		glRotatef(ry, 0, 1, 0);
		glRotatef(rx, 1, 0, 0);
		glRotatef(rz, 0, 0, 1);
		glGetFloatv(GL_MODELVIEW_MATRIX, g_IEyeMat);
		glPopMatrix();
	}
	//std::cout << g_fSpeed;
}
void Scat_Path()
{
	float len = 0;
	glBegin(GL_LINE_STRIP);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 1; i < path.size() - 1; i++)
	{
		len += (path[path.size() - 1] - path[path.size() - 2]).len();
		if (len < 10)
		{
			glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		}
		else if (10 <len && len < 20)
		{
			float a = 0.8f;
			glColor4f(1.0f, 1.0f, 1.0f, a);
			a -= 0.1f;
		}
		else
		{
			glColor4f(0.0f, 0.0f, 0.0f, 0.0f);
		}
		glVertex3fv(path[i]);
		//glVertex3fv(path[i]);
	}
	glEnd();
}
void SetRC()
{
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	Initial();
	glLineWidth(1.0f);
	glShadeModel(GL_FLAT);
	glFrontFace(GL_CW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glPolygonMode(GL_BACK, GL_LINE);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_LINE_STIPPLE);
	glLineStipple(2, 0xFFFE);
	glewInit();
	float Quadratic[] = { 0, 0, 1 };
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
	gluPerspective(60, GLfloat(w) / h, 0.01, 1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0, 0, 2, 0, 0, 0, 0, 1, 0);//调整视角
}
int main(int argc, char *argv[])
{
	string type;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scatter Plot");
	freopen("testt.txt", "r", stdin);//读取test.txt
	//freopen("out.txt", "w", stdout);//写入到out.txt
	while (cin >> type)
	{
		int2(type);
	}
	glutDisplayFunc(&Display);
	glutTimerFunc(g_fSeta, OnTimer, 1);
	path.push_back(CVector3(0, 0, 1, 0));
	glutSpecialFunc(&processSpecialKeys);
	glutKeyboardFunc(&myKeyboardFunc1);
	glutReshapeFunc(&myReshape);
	SetRC();
	glutMainLoop();
	return 0;
}
