// glCamera.cpp: implementation of the CglCamera class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "total.h"
#include "gl\glut.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CglCamera::CglCamera()
{
	Init();
}

void CglCamera::Init()
{
	m_type = 0;
	m_pos = CVector3(0, 0, 0,0);
	m_eyedir = CVector3(0, 0, -1,0);
	m_updir = CVector3(0, 1, 0,0);
	m_hpr = CVector3(0, 0, 0,0);
	int i;
	for (i = 0; i<16; i++)
	{
		if (i == 0 || i == 5 || i == 10 || i == 15)
		{
			m_viewMatrix[i] = 1;
			m_viewMatrixSave[i] = 1;
		}
		else
		{
			m_viewMatrixSave[i] = 0;
			m_viewMatrix[i] = 0;
		}
	}
}

CglCamera::~CglCamera()
{

}

void CglCamera::Move(int dir, double len)//dir=0��ʾ�����ƣ�dir=1��ʾ�����ƣ�dir=2��ʾǰ����
{
	if (m_type == 0)
	{
		CVector3 mov;
		if (dir == 0)
		{
			mov.Set(m_viewMatrixInverse[0], m_viewMatrixInverse[1], m_viewMatrixInverse[2],0);
		}
		else if (dir == 1)
		{
			mov.Set(m_viewMatrixInverse[4], m_viewMatrixInverse[5], m_viewMatrixInverse[6],0);
		}
		else if (dir == 2)
		{
			mov.Set(m_viewMatrixInverse[8], m_viewMatrixInverse[0], m_viewMatrixInverse[0],0);
		}
		m_pos = m_pos +  mov*len;
	}
	else if (m_type == 1)
	{
		glPushMatrix();
		glLoadIdentity();
		if (dir == 0)
			glTranslatef(-len, 0, 0);
		else if (dir == 1)
			glTranslatef(0, -len, 0);
		else
			glTranslatef(0, 0, -len);
		glMultMatrixd(m_viewMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_viewMatrix);
		CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
		glPopMatrix();
		m_pos.m_dX = m_viewMatrixInverse[12]; m_pos.m_dY = m_viewMatrixInverse[13]; m_pos.m_dZ = m_viewMatrixInverse[14];
	}
}

void CglCamera::Rotate(int dir, double len)//dir=0��ʾ����ת��dir=1��ʾ����ת��dir=2��ʾ����ת
{
	if (m_type == 0)
	{
		m_hpr[dir] += len;
	}
	else if (m_type == 1)
	{
		glPushMatrix();
		glLoadIdentity();
		if (dir == 0)
			glRotated(-len, 0, 1, 0);	//h
		else if (dir == 1)
			glRotated(-len, 1, 0, 0);	//p
		else
			glRotated(-len, 0, 0, 1);	//r
		glMultMatrixd(m_viewMatrix);
		glGetDoublev(GL_MODELVIEW_MATRIX, m_viewMatrix);
		CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
		glPopMatrix();
		m_eyedir = m_viewMatrixInverse*CVector3(0, 0, -1,0);
		m_eyedir = m_eyedir -  m_pos;

		m_updir = m_viewMatrixInverse*CVector3(0, 1, 0,0);
		m_updir =  m_updir - m_pos;
	}
}

void CglCamera::ShowView()
{
	if (m_type == 0)
	{
		glRotated(-m_hpr[2], 0, 0, 1);	//r
		glRotated(-m_hpr[1], 1, 0, 0);	//p
		glRotated(-m_hpr[0], 0, 1, 0);	//h
		glTranslatef(-m_pos[0], -m_pos[1], -m_pos[2]);
	}
	else if (m_type == 1)
	{
		gluLookAt(m_pos.m_dX, m_pos.m_dY, m_pos.m_dZ,
			m_pos.m_dX + m_eyedir.m_dX, m_pos.m_dY + m_eyedir.m_dY, m_pos.m_dZ + m_eyedir.m_dZ,
			m_updir.m_dX, m_updir.m_dY, m_updir.m_dZ
			);
	}
	glGetDoublev(GL_MODELVIEW_MATRIX, m_viewMatrix);
	CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
}

void CglCamera::SetCamera(const CVector3 &pos, const CVector3 &obj_dir, bool bObj, int type)
{
	if (type >= 0) m_type = type;
	m_pos = pos;
	if (bObj)	//����ڶ��������ǹ۲�Ķ���λ��
	{
		m_eyedir = obj_dir - pos;
	}
	else if (m_type == 0)	//�ڶ��������ǹ۲�ķ���,ˮƽ����hpr
	{
		m_hpr = obj_dir;
	}
	else if (m_type == 1)	//�ڶ��������ǹ۲�ķ���
	{
		m_eyedir = obj_dir;
	}
	glPushMatrix();
	glLoadIdentity();
	ShowView();
	glPopMatrix();
	CglMath::Dir2HPR(m_eyedir, m_updir, m_hpr);
}

void CglCamera::SaveCamera()
{
	int i;
	for (i = 0; i<16; i++)
	{
		m_viewMatrixSave[i] = m_viewMatrix[i];
	}
}

void CglCamera::LoadCamera()
{
	int i;
	for (i = 0; i<16; i++)
	{
		m_viewMatrix[i] = m_viewMatrixSave[i];
	}
	CglMath::InverseMatrix(m_viewMatrix, m_viewMatrixInverse);
	m_pos = m_viewMatrixInverse*CVector3(0, 0, 0,0);
	m_eyedir = m_viewMatrixInverse*CVector3(0, 0, -1,0) - m_pos;
	m_updir = m_viewMatrixInverse*CVector3(0, 1, 0,0) - m_pos;
	CglMath::Dir2HPR(m_eyedir, m_updir, m_hpr);
}
void CglCamera::SetType(int type)
{
	m_type = type;
	m_eyedir = m_viewMatrixInverse*CVector3(0, 0, -1,0) - m_pos;
	m_updir = m_viewMatrixInverse*CVector3(0, 1, 0,0) - m_pos;
	CglMath::Dir2HPR(m_eyedir, m_updir, m_hpr);
}
