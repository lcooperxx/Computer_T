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
#include "CMatrix.h"
#include "CVector3.h"
using namespace std;
vector<CVector3> path;
float seta = 0;
float axis = 0;
//视点位置和方向
float mx = 0, my = 0, mz = 0.5, rx = 0, ry = 0, rz = 0;//平移和旋转
float sx = 1, sy = 1, sz = 1;//缩放
float mspeed = 0.05, rspeed = 1;
float g_IEyeMatt[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 }, g_EyeMatt[16] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
CMatrix g_IEyeMat, g_EyeMat;
int mode = 1;
void CM_print(CMatrix temp)
{
	for (int i = 0; i < 4; i++){
		for(int j = 0; j < 4;j++)
		{
			if (!((i*4+j) % 4) && i != 0)
				std::cout << endl;
			std::cout << temp[i+4*j] << " ";
		}

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
			t2 = GetInverse(t1, N);
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
	else if (type == "矩阵单位化")
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
		//CM_print(t1);
		float t3;
		scanf("%f\n", &t3);
		t1 = t1 * t3;
		CM_print(t1);
	}
	else if (type == "矩阵乘向量")
	{
		float temp[16], p1, p2, p3,p4;
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
		scanf("%f,%f,%f,%f\n", &p1, &p2, &p3,&p4);
		p.Set(p1, p2, p3, p4);
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
		scanf("%f,%f,%f,%f\n", &p.m_dX, &p.m_dY, &p.m_dZ,&p.m_dT);
		t1.Set(temp);
		p.Set(p.m_dX, p.m_dY, p.m_dZ, p.m_dT);
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
		float seta, p1, p2, p3;
		scanf("%f", &seta);
		scanf("%f,%f,%f\n", &p1, &p2, &p3);
		p.Set(p1, p2, p3, 1);
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
void Initial();//初始化随机
void Scat();//画线，点，面
void Scat_Path();//画轨迹
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
		t1.SetRotate(-rz*3.14/180, p).run();
		//glRotatef(-rz, 0, 0, 1);
		p.Set(1, 0, 0,0);
		t1.SetRotate(-rx*3.14 / 180, p).run();
		//glRotatef(-rx, 1, 0, 0);
		p.Set(0, 1, 0, 0);
		t1.SetRotate(-ry*3.14 / 180, p).run();
		//glRotatef(-ry, 0, 1, 0);

		p.Set(-mx, -my, -mz, 0);
		//p.Set(0, 0, 0, 0);
		t1.SetTrans(p).run();
		//glTranslatef(-mx, -my, -mz);
	}
}
void RenderWorld()//坐标轴
{
	//绘制坐标轴
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	CVector3 p(0, 0, 0,0);
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
	t1.SetRotate(Seta + 0.2, p).run();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
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
void changeR(float fi,CVector3 p)
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

	default:
		break;
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
		//my+=mspeed;
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
			path.push_back(CVector3(mx, my, mz,0));
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
			glPushMatrix();
			glLoadIdentity();
			CMatrix t1;
			t1.SetRotate(-rz,CVector3(0,0,1,0));
			t1.SetRotate(-rx,CVector3(1,0,0,0));
			//glRotatef(-rz, 0, 0, 1);
			//lRotatef(-rx, 1, 0, 0);
			glRotatef(-ry, 0, 1, 0);
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
		g_IEyeMat = GetInverse(g_EyeMat,N);
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
	float len = 0 ;
	glBegin(GL_LINE_STRIP);
	//glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for (int i = 1; i < path.size() - 1;i++)
	{
		/*len += sqrt((path[i].m_dX - path[i - 1].m_dX) * (path[i].m_dX - path[i - 1].m_dX)
			+ (path[i].m_dY - path[i - 1].m_dY) * (path[i].m_dY - path[i - 1].m_dY)
			+ (path[i].m_dZ - path[i - 1].m_dZ) * (path[i].m_dZ - path[i - 1].m_dZ));
		*/
		len += (path[path.size() -1] - path[path.size()-2]).len();
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
	float Quadratic[] = { 0, 0, 1};
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
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Scatter Plot");
	freopen("test2.txt", "r", stdin);//读取test.txt
	//freopen("out.txt", "w", stdout);//写入到out.txt
	while (cin >> type)
	{
		int2(type);
	}
	glutDisplayFunc(&Display);
	glutTimerFunc(g_fSeta, OnTimer, 1);
	path.push_back(CVector3(0, 0, 1, 0));
	glutKeyboardFunc(&myKeyboardFunc1);
	Scat_Path();
	glutSpecialFunc(&processSpecialKeys);
	glutReshapeFunc(&myReshape);
	SetRC();
	glutMainLoop();
	return 0;
}
