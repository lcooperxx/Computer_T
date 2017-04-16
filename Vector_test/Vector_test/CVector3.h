#ifndef Cvector_h
#define Cvector_h
#include<math.h>
#include<vector>

class CVector3
{
public:
	CVector3(){};
	float m_dX, m_dY, m_dZ;
	CVector3(float m_dX, float m_dY, float m_dZ)//��ֵ����
	{
		this->m_dX = m_dX;
		this->m_dY = m_dY;
		this->m_dZ = m_dZ;
	}
	operator float*(){ return &m_dX; }
	friend CVector3 operator +(CVector3&c1, CVector3&c2);//���ء�+������
	friend CVector3 operator -(CVector3&c1,CVector3&c2);//���ء�-������
	friend bool operator ==(CVector3&c1, CVector3&c2);//���ء��Ƚ���ȡ�����
	friend bool operator !=(CVector3&c1, CVector3&c2);//���ء��Ƚϲ��ȡ�����

	friend CVector3 operator *(CVector3&c1, float c2);////���ء�*��������ǰ����
	friend CVector3 operator *(float c2, CVector3 &c1);//���ء�*��������ǰ����
	CVector3 operator=(CVector3 &c2);////���ظ�ֵ��=������
	float dotMul(const CVector3 &n);//���ء���ˡ�����
	CVector3 crossMul(const CVector3 &n);//���ء���ˡ�����
	float len();//���ء���ģ������
	void Normalize();//���ء���׼��������
	CVector3 project(CVector3 &n);//���ء�ͶӰ������
	void Set(float x, float y, float z);//������ֵ
};


#endif;