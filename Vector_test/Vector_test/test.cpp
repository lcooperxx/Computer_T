#include"CVector3.h"
#include<gl/glut.h>
#include<stdio.h>
#include<math.h>

using namespace std;

void CVector3::Set(float x, float y, float z)//设置对应的值
{
	this->m_dX = x;
	this->m_dY = y;
	this->m_dZ = z;
}
CVector3 operator +( CVector3&c1,  CVector3&c2)//重载“+”操作
{
	CVector3 temp;
	temp.m_dX = c1.m_dX + c2.m_dX;
	temp.m_dY = c1.m_dY + c2.m_dY;
	temp.m_dZ = c1.m_dZ + c2.m_dZ;
	return temp;
}

CVector3 operator -(CVector3&c1, CVector3&c2)//重载“-”操作
{
	CVector3 temp;
	temp.m_dX = c1.m_dX - c2.m_dX;
	temp.m_dY = c1.m_dY - c2.m_dY;
	temp.m_dZ = c1.m_dZ - c2.m_dZ;
	return temp;
}
CVector3 CVector3::operator = (CVector3&c2)//重载赋值操作
{
	m_dX = c2.m_dX;
	m_dY = c2.m_dY;
	m_dZ = c2.m_dZ;
	return *this;
}
bool operator ==(CVector3&c1, CVector3&c2)//重载相等比较操作
{
	if (c1.m_dX == c2.m_dX&&c1.m_dY==c2.m_dY&&c1.m_dZ==c2.m_dZ)
		return true;
	else
		return false;

}
bool operator !=(CVector3&c1, CVector3&c2)//重载不等比较操作
{
	if (c1.m_dX != c2.m_dX||c1.m_dY != c2.m_dY||c1.m_dZ != c2.m_dZ)
		return true;
	else
		return false;
}

CVector3 operator *(CVector3&c1, float c2)//重载*操作，前向量
{
	CVector3 temp;
	temp.m_dX = c1.m_dX * c2;
	temp.m_dY = c1.m_dY * c2;
	temp.m_dZ = c1.m_dZ * c2;
	return temp;
}

CVector3 operator *(float c2,CVector3&c1)//重载*操作，前常数
{
	CVector3 temp;
	temp.m_dX = c1.m_dX * c2;
	temp.m_dY = c1.m_dY * c2;
	temp.m_dZ = c1.m_dZ * c2;
	return temp;
}

float CVector3::dotMul(const CVector3 &n)//重载点乘操作
{
	float temp;
	temp= m_dX * n.m_dX + m_dY * n.m_dY + m_dZ * n.m_dZ;
	return temp;
}

CVector3 CVector3::crossMul(const CVector3 &n)//叉乘操作
{
	CVector3 temp;
	temp.Set(m_dY * n.m_dZ - m_dZ * n.m_dY , 
		m_dZ * n.m_dX - m_dX * n.m_dZ , 
		m_dX*n.m_dY - m_dY * n.m_dX);
	return temp;
}

float CVector3 :: len(){//求模操作
	float temp = sqrt(m_dX * m_dX + m_dY * m_dY + m_dZ * m_dZ);
	return temp;
}
void CVector3 :: Normalize(){//标准化操作
	float temp = sqrt(m_dX * m_dX + m_dY * m_dY + m_dZ * m_dZ);
	this->Set(m_dX / temp, m_dY / temp, m_dZ / temp);
}
CVector3 CVector3::project(CVector3 &n)//投影操作
{
	CVector3 temp;
	float a1 = sqrt(m_dX * m_dX + m_dY * m_dY + m_dZ*m_dZ);
	n.Normalize();
	return a1*n;
}
