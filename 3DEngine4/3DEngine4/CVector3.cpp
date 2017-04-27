#include"stdafx.h"
#include<math.h>
#include<vector>
#include"total.h"
class CEuler;
 CVector3 operator*(double *mat, CVector3& p)
{
	double v[4];
	for (int i = 0; i<4; i++)
	{
		v[i] = mat[i] * p.m_dX + mat[4 + i] * p.m_dY + mat[8 + i] * p.m_dZ + mat[12 + i];
	}
	CVector3 vec;
	vec.m_dX = v[0] / v[3];
	vec.m_dY = v[1] / v[3];
	vec.m_dZ = v[2] / v[3];
	return vec;
}
CVector3  CVector3::operator-()
 {
	 CVector3 vec;
	 vec.m_dX = - (*this).m_dX;
	 vec.m_dY = - (*this).m_dY;
	 vec.m_dZ = - (*this).m_dZ;
	 return vec;
 }
CVector3 operator -(const CVector3&c1, const CVector3&c2)//重载“-”操作
{
	CVector3 temp;
	temp.m_dX = c1.m_dX - c2.m_dX;
	temp.m_dY = c1.m_dY - c2.m_dY;
	temp.m_dZ = c1.m_dZ - c2.m_dZ;
	temp.m_dT = c1.m_dT - c2.m_dT;
	return temp;
}
void CVector3::Set(double x, double y, double z, double t)//设置对应的值
{
	this->m_dX = x;
	this->m_dY = y;
	this->m_dZ = z;
	this->m_dT = t;
}
CVector3 operator +(CVector3&c1, CVector3&c2)//重载“+”操作
{
	CVector3 temp;
	temp.m_dX = c1.m_dX + c2.m_dX;
	temp.m_dY = c1.m_dY + c2.m_dY;
	temp.m_dZ = c1.m_dZ + c2.m_dZ;
	temp.m_dT = c1.m_dT + c2.m_dT;
	return temp;
}


CVector3& CVector3::operator = (const CVector3 &c2)//重载赋值操作
{
	m_dX = c2.m_dX;
	m_dY = c2.m_dY;
	m_dZ = c2.m_dZ;
	m_dT = c2.m_dT;
	return *this;
}
bool operator ==(CVector3&c1, CVector3&c2)//重载相等比较操作
{
	if (c1.m_dX == c2.m_dX&&c1.m_dY == c2.m_dY&&c1.m_dZ == c2.m_dZ&&c1.m_dT == c2.m_dT)
		return true;
	else
		return false;

}
bool operator !=(CVector3&c1, CVector3&c2)//重载不等比较操作
{
	if (c1.m_dX != c2.m_dX || c1.m_dY != c2.m_dY || c1.m_dZ != c2.m_dZ || c1.m_dT == c2.m_dT)
		return true;
	else
		return false;
}

CVector3 operator *(CVector3&c1, double c2)//重载*操作，前向量
{
	CVector3 temp;
	temp.m_dX = c1.m_dX * c2;
	temp.m_dY = c1.m_dY * c2;
	temp.m_dZ = c1.m_dZ * c2;
	temp.m_dT = c1.m_dT * c2;
	return temp;
}

CVector3 operator *(double c2, CVector3&c1)//重载*操作，前常数
{
	CVector3 temp;
	temp.m_dX = c1.m_dX * c2;
	temp.m_dY = c1.m_dY * c2;
	temp.m_dZ = c1.m_dZ * c2;
	temp.m_dT = c1.m_dT * c2;
	return temp;
}

double CVector3::dotMul(CVector3 &n)//重载点乘操作
{
	double temp;
	temp = m_dX * n.m_dX + m_dY * n.m_dY + m_dZ * n.m_dZ + m_dT * n.m_dT;
	return temp;
}

CVector3 CVector3::crossMul(CVector3 &n)//叉乘操作
{
	CVector3 temp;
	temp.Set(m_dY * n.m_dZ - m_dZ * n.m_dY,
		m_dZ * n.m_dT - m_dT * n.m_dZ,
		m_dT * n.m_dX - m_dX * n.m_dT,
		m_dX*n.m_dY - m_dY * n.m_dX);
	return temp;
}

double CVector3::len(){//求模操作
	double temp = 0;
	temp = sqrt(m_dX * m_dX + m_dY * m_dY + m_dZ * m_dZ);
	return temp;
}
void CVector3::Normalize(){//标准化操作
	double temp = sqrt(m_dX * m_dX + m_dY * m_dY + m_dZ * m_dZ);
	this->Set(m_dX / temp, m_dY / temp, m_dZ / temp, m_dT / temp);
}
CVector3 CVector3::project(CVector3 &n)//投影操作
{
	CVector3 temp;
	double a1 = sqrt(m_dX * m_dX + m_dY * m_dY + m_dZ*m_dZ + m_dT * m_dT);
	n.Normalize();
	return a1*n;
}

CVector3 CVector3::lerp(CVector3 p, double t)
{
	(*this) = (1 - t) * (*this) + t * p;
	return (*this);
}

CEuler CVector3::ToEuler(){
	double h, p, b;
	CVector3 p1, p2, p3;
	CEuler e1;
	p1.Set((*this).m_dX, 0, (*this).m_dZ, 0);
	p2.Set((*this).m_dX, (*this).m_dY, 0, 0);
	p3.Set(0, (*this).m_dY, (*this).m_dZ, 0);

	h = acosf(p1.dotMul(CVector3(0, 0, -1, 0)) / (p1.len())) * 180 / 3.14;
	p = acosf((*this).dotMul(p1) / ((*this).len() * p1.len())) * 180 / 3.14;
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