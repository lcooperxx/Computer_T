// myOpenGL.cpp: implementation of the CmyOpenGL class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "project5.4.h"
#include "CmyOpenGL.h"
#include <GL/glew.h>
#include "gl/glut.h"
#include "gl/gl.h"
#pragma comment(lib,"glew32.lib ")

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CmyOpenGL::CmyOpenGL()
{
	m_bWired = false;
}

CmyOpenGL::~CmyOpenGL()
{
}

void CmyOpenGL::SetRC()
{
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
	float Quadratic[] = { 0, 0, 0.1 };
	glPointParameterfv(GL_POINT_DISTANCE_ATTENUATION, Quadratic);
	glPointParameterf(GL_POINT_SIZE_MIN, 1);
	glPointParameterf(GL_POINT_SIZE_MAX, 100);
}
void CmyOpenGL::Scat_Initial()
{
	path.push_back(CVector3(0, 1, 0, 0));
	for (int i = 0; i < 16; i++)
	{
		if (i == 0 || i == 4 || i == 10 || i == 15)
		{
			g_IEyeMatt[i] = g_EyeMat[i] = 1;
		}
		else
		{
			g_IEyeMatt[i] = g_EyeMatt[i] = 0;
		}
	}
	g_IEyeMat.Set(g_IEyeMatt);
	g_EyeMat.Set(g_EyeMatt);
	int i;
	srand((unsigned)(time(NULL)));//按时间生成随机种子，且数量不是定值
	for (i = 0; i < 10001; i++)
	{
		z[0][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的x轴坐标
		z[1][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的y轴坐标
		z[2][i] = 2.0*(0.5 - rand() / double(RAND_MAX));//生成10 000个点的z轴坐标
	}
	memset(visit, 0, sizeof(visit));
	visit[0] = 1;
	gluOrtho2D(1.0, 1.0, 1.0, 1.0);   //投影到裁剪窗大小：世界
}
void CmyOpenGL::scatter()
{
	int i;
	GLfloat pointSize = 5.4f;//设置点的大小为5.4f
	glPointSize(pointSize);
	glBegin(GL_POINTS);
	for (i = 0; i < 10001; i++)
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
		glVertex3d(z[0][i], z[1][i], z[2][i]);//生成点
	}
	glEnd();
}

void CmyOpenGL::Scatter_Line()
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
	//glClear(GL_COLOR_BUFFER_BIT);
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
		glVertex3dv(p);//直线起点
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		//t1 = t1.SetRotate(60, p).run();
		glVertex3dv(p);//直线终点
	}
	glEnd();
}

void CmyOpenGL::Scatter_triangle()
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
	CVector3 p;
	glBegin(GL_TRIANGLE_STRIP);
	for (int i = 1; i < point.size() - 1; i++){
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
		glVertex3dv(p);//三角形第一个点
		p.Set(z[0][point[i]], z[1][point[i]], z[2][point[i]], 0);
		glVertex3dv(p);//三角形第二个点
		p.Set(z[0][point[i + 1]], z[1][point[i + 1]], z[2][point[i + 1]], 0);
		glVertex3dv(p);//三角形第三个点
	}
	glEnd();
}
void CmyOpenGL::Display()
{
	SetRC();
	CMatrix t1;
	CVector3 p(0, 1, 0, 0);
	//t1.SetRotate(Seta + 0.2, p).run();
	glPushMatrix();	
	if (s_t){
		if (s_t == 1){
			m_pCamere->m_hpr.Set(s_hpr2[0], s_hpr2[1], s_hpr2[2], 0);
		}
		Scat_Jump();
		s_t++;
		if (s_t == 100) s_t = 0;
	}
	if (Cmy_flag == 0)//设置标记变量flag flag==0时 画点
	{
		scatter();
	}
	else if (Cmy_flag == 1)//flag==1时画线
	{
		Scatter_Line();
	}
	else if (Cmy_flag == 2)//flag==3时，画三角形
	{
		Scatter_triangle();
	}
	Scat_Path();
	glPopMatrix();
}
void CmyOpenGL::Scat_Path()
{
	float len = 0;
	glBegin(GL_LINE_STRIP);
	for (int i = 1; i < path.size() - 1; i++)
	{
		CVector3 t;
		t.m_dX = path[path.size() - 1].m_dX - path[path.size() - 2].m_dX;
		t.m_dY = path[path.size() - 1].m_dY - path[path.size() - 2].m_dY;
		t.m_dZ = path[path.size() - 1].m_dZ - path[path.size() - 2].m_dZ;
		len += t.len();
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
		glVertex3dv(path[i]);
		//glVertex3fv(path[i]);
	}
	glEnd();
}
void CmyOpenGL::PostInit()
{
	glClearColor(0, 0, 0, 0);
	m_pCamere->SetCamera(CVector3(5, 10, 10,0), CVector3(0, 0, 0,0), true);
	m_pCamere->SaveCamera();
	m_pControl->SetSpeed(0.01, 0.5);
	
	Scat_Initial();
	m_pRecorder->BeginLoad("record.dat", false);
}

void CmyOpenGL::InDraw()
{
	if (m_bWired)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	m_pShape->DrawCoord(1000);
	DrawModel();
	float t = m_pTime->GetSimuTime() / 1000;

	glColor3f(1, 0, 0);

	Display();
}
void CmyOpenGL::Save_Pos()
{
	if (mode == 1)
	{
		s_pos1.Set(m_pCamere->m_viewMatrix);
	}
	else
	{
		s_pf = m_pCamere->m_pos;
		s_hpr1.h = m_pCamere->m_hpr.m_dX; s_hpr1.p = m_pCamere->m_hpr.m_dY; s_hpr1.b = m_pCamere->m_hpr.m_dZ;
	}
	
}
void CmyOpenGL::Reverse_Pos()
{
	if (mode == 1)
	{
		CMatrix r_posi;
		r_posi = s_pos1;
		//CM_print(r_posi);
		for (int i = 0; i < 16; i++)
		{
			m_pCamere->m_viewMatrix[i] = s_pos1[i];
		}
		CglMath::InverseMatrix(m_pCamere->m_viewMatrix, m_pCamere->m_viewMatrixInverse);
		CMatrix t1;
		t1.Set(m_pCamere->m_viewMatrixInverse);
		m_pCamere->m_pos = t1.vecMul(CVector3(0, 0, 0, 0));
		m_pCamere->m_eyedir.m_dX = t1.vecMul(CVector3(0, 0, -1, 0)).m_dX - m_pCamere->m_pos.m_dX;
		m_pCamere->m_eyedir.m_dY = t1.vecMul(CVector3(0, 0, -1, 0)).m_dY - m_pCamere->m_pos.m_dY;
		m_pCamere->m_eyedir.m_dZ = t1.vecMul(CVector3(0, 0, -1, 0)).m_dZ - m_pCamere->m_pos.m_dZ;

		m_pCamere->m_updir.m_dX = t1.vecMul(CVector3(0, 1, 0, 0)).m_dX - m_pCamere->m_pos.m_dX;
		m_pCamere->m_updir.m_dY = t1.vecMul(CVector3(0, 1, 0, 0)).m_dY - m_pCamere->m_pos.m_dY;
		m_pCamere->m_updir.m_dZ = t1.vecMul(CVector3(0, 1, 0, 0)).m_dZ - m_pCamere->m_pos.m_dZ;

		CglMath::Dir2HPR(m_pCamere->m_eyedir, m_pCamere->m_updir, m_pCamere->m_hpr);
	}
	else
	{
		m_pCamere->m_pos = s_pf;
		m_pCamere->m_hpr.m_dX = s_hpr1.b;
		m_pCamere->m_hpr.m_dY = s_hpr1.h;
		m_pCamere->m_hpr.m_dZ = s_hpr1.p;
	}

}
void CmyOpenGL::Scat_Jump()
{
	if (mode == 0){
		s_cq1 = s_hpr1.ToQuaternion();
		s_cq2 = s_hpr2.ToQuaternion();
		s_result = s_cq1.Slerp(s_cq2, (double)s_t / 100.0);
		CEuler tV = s_result.ToEuler();
		m_pCamere->m_hpr.Set(tV[0], tV[1], tV[2], 0);

		m_pCamere->m_pos = s_pf.lerp(s_pn, (double)s_t / 100.0);
	}
	else
	{
		CMatrix t1;
		CglMath::InverseMatrix(m_pCamere->m_viewMatrixSave, m_pCamere->m_viewMatrixSave);
		t1.Set(m_pCamere->m_viewMatrixSave);
		CMatrix t2;
		CglMath::InverseMatrix(m_pCamere->m_viewMatrix, m_pCamere->m_viewMatrixInverse);
		t2.Set(m_pCamere->m_viewMatrixInverse);

		s_cq2 = t2.ToQuaternion();
		s_cq1 = t1.ToQuaternion();

		s_result = s_cq1.Slerp(s_cq2, (double)s_t / 100.0);

		CMatrix t3;
		t3 = s_result.ToMatrix();
		for (int i = 0; i < 16; i++)
		{
			m_pCamere->m_viewMatrix[i] = t3[i];
		}
	}

}
bool CmyOpenGL::OnKey(unsigned int nChar, bool bDown)
{
	bool bChange = false;
	if (bDown)
	switch (nChar)
	{
		case VK_F1:
			m_bWired = !m_bWired;
			break;
		case ' ':
			Cmy_flag++;
			if (Cmy_flag == 3)
			{
				Cmy_flag = 0;
			}
		case VK_F2:
			std::cout << "change direction mode" << mode << std::endl;
			mode = mode == 0 ? 1 : 0;
			m_pCamere->SetType(mode);
			break;
		case VK_F3:
			m_pCamere->SaveCamera();

			s_hpr1.Set(m_pCamere->m_hpr[0], m_pCamere->m_hpr[1], m_pCamere->m_hpr[2]);
			s_pf = m_pCamere->m_pos;
			break;
		case VK_F4:
			m_pCamere->LoadCamera();

			break;
		case VK_F5:
			s_t = 1;
			s_hpr2.Set(m_pCamere->m_hpr[0], m_pCamere->m_hpr[1], m_pCamere->m_hpr[2]);
			s_pn = m_pCamere->m_pos;
			//Scat_Jump();
			break;
		default:
			break;
	}
	return false;
}
double seta = 0;

void CmyOpenGL::DrawModel()
{
	seta += m_pTime->GetTimeSpan()*0.01;
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1, 0, 0);
	glPushMatrix();
	m_pRecorder->MatrixRecord();
	glPopMatrix();
}

