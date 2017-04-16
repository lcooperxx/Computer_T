#ifndef Cvector_h
#define Cvector_h
#include<math.h>
#include<vector>

class CVector3
{
public:
	CVector3(){};
	float m_dX, m_dY, m_dZ;
	CVector3(float m_dX, float m_dY, float m_dZ)//赋值操作
	{
		this->m_dX = m_dX;
		this->m_dY = m_dY;
		this->m_dZ = m_dZ;
	}
	operator float*(){ return &m_dX; }
	friend CVector3 operator +(CVector3&c1, CVector3&c2);//重载“+”操作
	friend CVector3 operator -(CVector3&c1,CVector3&c2);//重载“-”操作
	friend bool operator ==(CVector3&c1, CVector3&c2);//重载“比较相等”操作
	friend bool operator !=(CVector3&c1, CVector3&c2);//重载“比较不等”操作

	friend CVector3 operator *(CVector3&c1, float c2);////重载“*”操作，前向量
	friend CVector3 operator *(float c2, CVector3 &c1);//重载“*”操作，前常数
	CVector3 operator=(CVector3 &c2);////重载赋值“=”操作
	float dotMul(const CVector3 &n);//重载“点乘”操作
	CVector3 crossMul(const CVector3 &n);//重载“叉乘”操作
	float len();//重载“求模”操作
	void Normalize();//重载“标准化”操作
	CVector3 project(CVector3 &n);//重载“投影”操作
	void Set(float x, float y, float z);//设置数值
};


#endif;