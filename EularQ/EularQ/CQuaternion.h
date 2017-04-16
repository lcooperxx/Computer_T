#include "stdafx.h"
#include "CMatrix.h"
#include <math.h>
#include <vector>
#include "CVector3.h"
#include "EularQ.h"
#include <iostream>
class CQuaternion
{
public:
	float x, y, z, w;
	void Set(float w, float x, float y, float z);
	operator float*(){ return &x; };
	CEuler ToEuler();//��Ԫ��ת��Ϊŷ����
	CMatrix ToMatrix();//��Ԫ��ת��Ϊ����
	void SetAngle(float angle, CVector3 axis); //��Ԫ������
	CQuaternion operator=(const CQuaternion& p); //���ظ�ֵ
	CQuaternion CQuaternion :: operator/(float p); //�������
	CQuaternion operator+(const CQuaternion& p); //���ء�+��
	CQuaternion operator*(float data); //��������
	CQuaternion operator*(const CQuaternion&p); //��Ԫ���˷�
	float dotMul(const CQuaternion&p); //���
	float len(); //��ģ
	bool Normalize();	//���׼��
	CQuaternion Inverse();//������Ԫ��,��ı�����
	CQuaternion GetInverse();//������Ԫ��,���ı����������µ���Ԫ��
	CQuaternion Div(const CQuaternion&b); //��� ��ǰΪa,��c=a-b
	void GetAngle(float& angle, CVector3& axis); //����ת��ͽǶ�
	CQuaternion Slerp(CQuaternion& Vend, float t); //��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
	void Slerp(CQuaternion& Vend, int n, float *t, CQuaternion *Result);//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result��
};
CEuler CQuaternion::ToEuler()//��Ԫ��ת��Ϊŷ����
{
	float h, p, b;
	p = (asinf(2 * ((*this).w * (*this).x - (*this).y * (*this).z))) * 180 / 3.14;
	if (cosf(p * 3.14 / 180) != 0)
	{
		h = (atan2(2 * ((*this).w * (*this).y + (*this).x * (*this).z), 1 - 2 * ((*this).x * (*this).x + (*this).y * (*this).y))) * 180 / 3.14;
		b = (atan2(2 * ((*this).w * (*this).z + (*this).x * (*this).y), 1 - 2 * ((*this).z * (*this).z + (*this).x * (*this).x))) * 180 / 3.14;
	}
	else
	{
		h = (atan2(2 * ((*this).w * (*this).y - (*this).x * (*this).z), 1 - 2 * ((*this).y * (*this).y + (*this).z * (*this).z))) * 360 / 2 / 3.14;
		b = 0;
	}
	CEuler e1;
	e1.Set(h, p, b);
	return e1;
}

CMatrix CQuaternion::ToMatrix()
{
	CMatrix t1;
	t1.m00 = 1.0f - 2.0f * (*this).y * (*this).y - 2.0f * (*this).z * (*this).z;
	t1.m10 = 2.0f * ((*this).x * (*this).y - (*this).z * (*this).w);//error2
	t1.m20 = 2.0f * (*this).x * (*this).z + 2.0f * (*this).y * (*this).w;
	t1.m30 = 0.0f;

	t1.m01 = 2.0f * ((*this).x * (*this).y + (*this).z * (*this).w);
	t1.m11 = 1.0f - 2.0f * (*this).x * (*this).x - 2.0f * (*this).z * (*this).z;
	t1.m21 = 2.0f * (*this).z * (*this).y - 2.0f * (*this).x * (*this).w;
	t1.m31 = 0.0f;

	t1.m02 = 2.0f * (*this).x * (*this).z - 2.0f * (*this).y * (*this).w;
	t1.m12 = 2.0f * (*this).z * (*this).y + 2.0f * (*this).x * (*this).w;
	t1.m22 = 1.0f - 2.0f * (*this).x * (*this).x - 2.0f * (*this).y * (*this).y;
	t1.m32 = 0.0f;

	t1.m03 = 0.0f;
	t1.m13 = 0.0f;
	t1.m23 = 0.0f;
	t1.m33 = 1.0f;
	//t1.Transpose();
	return t1;
}
void CQuaternion::Set(float x, float y, float z,float w)//���ö�Ӧ��ֵ
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}
void CQuaternion::SetAngle(float angle, CVector3 axis)//��Ԫ������
{
	float halfangle;
	halfangle = angle * 0.5f;
	(*this).Set(cosf(halfangle * 3.14 /180), axis.m_dX * sinf(halfangle *3.14/180), axis.m_dY * sinf(halfangle*3.14/180), axis.m_dZ * sinf(halfangle*3.14/180));
}
CQuaternion CQuaternion :: operator=(const CQuaternion& p) //���ظ�ֵ
{
	(*this).x = p.x;
	(*this).y = p.y;
	(*this).z = p.z;
	(*this).w = p.w;
	return (*this);
}
CQuaternion CQuaternion :: operator/(float p) //�������
{
	(*this).x /= p;
	(*this).y /= p;
	(*this).z /= p;
	(*this).w /= p;
	return (*this);
}
CQuaternion CQuaternion :: operator+(const CQuaternion& p) //���ء�+��
{
	(*this).w += p.w;
	(*this).x += p.x;
	(*this).y += p.y;
	(*this).z += p.z;
	return (*this);
}
CQuaternion CQuaternion :: operator*(float data) //��������
{
	(*this).w *= data;
	(*this).x *= data;
	(*this).y *= data;
	(*this).z *= data;
	return (*this);
}
CQuaternion CQuaternion :: operator*(const CQuaternion&p) //��Ԫ���˷�
{
	CQuaternion cq1;
	cq1.w = (*this).w * p.w - (*this).x * p.x - (*this).y * p.y - (*this).z * p.z;
	cq1.x = (*this).w * p.x + (*this).x * p.w + (*this).z * p.y - (*this).y * p.z;
	cq1.y = (*this).w * p.y + (*this).y * p.w + (*this).x * p.z - (*this).z * p.x;
	cq1.z = (*this).w * p.z + (*this).z * p.w + (*this).y * p.x - (*this).x * p.y;
	return cq1;
}

float CQuaternion::len() //��ģ
{
	float ans;
	ans = sqrt(((*this).w * (*this).w) + ((*this).x * (*this).x) + ((*this).y * (*this).y) + ((*this).z * (*this).z));
	return ans;
}
bool CQuaternion::Normalize()	//���׼��
{
	(*this) = (*this) / ((*this).len());
	return true;
}
CQuaternion CQuaternion::Inverse()//������Ԫ��,��ı�����
{
	(*this).w = (*this).w;
	(*this).x = -(*this).x;
	(*this).y = -(*this).y;
	(*this).z = -(*this).z;
	(*this) = (*this) / ((*this).len() * (*this).len());
	return (*this);
}
CQuaternion CQuaternion::GetInverse()//������Ԫ��,���ı����������µ���Ԫ��
{
	CQuaternion cq1;
	cq1.w = (*this).w;
	cq1.x = -(*this).x;
	cq1.y = -(*this).y;
	cq1.z = -(*this).z;
	cq1 = cq1 / (cq1.len() * cq1.len());
	return cq1;
}
CQuaternion CQuaternion::Div(const CQuaternion&b) //��� ��ǰΪa,��c=a-b
{
	(*this) = (*this).Inverse();
	(*this) = (*this) * b ;
	return (*this);
}
float CQuaternion::dotMul(const CQuaternion&p) //���
{
	return (*this).x * p.x + (*this).y * p.y + (*this).z * p.z + (*this).w * p.w;
}
void CQuaternion::GetAngle(float& angle, CVector3& axis) //����ת��ͽǶ�
{
	(*this).Normalize();
	angle = ((acosf((*this).w)) * 180 / 3.14) * 2.0f;
	axis.m_dX = (*this).x / sin(angle);
	axis.m_dY = (*this).y / sin(angle);
	axis.m_dZ = (*this).z / sin(angle);
	axis.m_dT = 0;
}

CQuaternion CQuaternion::Slerp(CQuaternion& Vend, float t) //��ֵ���ӵ�ǰ��Ԫ����ֵ��Vend��Ԫ��,t�ǲ���[0,1]
{
	(*this).Normalize();//����������ֱ���е�λ��
	Vend.Normalize();
	float angle;
	CVector3 axis;
	(Vend).GetAngle(angle, axis);//���Vend��Ԫ������ת�ǶȺ���ת��
	angle = angle * 3.14 / 180;
	float sinw = sin(angle);
	float k0, k1;
	k0 = sinf((1 - t)*angle) / sinw;
	k1 = sinf(t*angle) / sinw;
	(*this) = (*this) * k0 + Vend * k1;//���չ�ʽ����Ԫ�����в�ֵ����
	(*this).Normalize();//�����Ԫ����λ��
	return (*this);
}
void CQuaternion::Slerp(CQuaternion& Vend, int n, float *t, CQuaternion *Result)//��ֵ��һ�β�ֵ��n�����ݡ���ֵ��������������t�У�������ص�����Result��
{
	for (int i = 0; i < n; i++)
	{
		Result[i] = Slerp(Vend, t[i]);
	}
}
