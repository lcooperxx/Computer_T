#include"stdafx.h"
#include<math.h>
#include<vector>
#include<iostream>
#include"total.h"
class CMatrix;

void CEuler::Set(double h, double p, double b)
{
	this->h = h;
	this->p = p;
	this->b = b;
}
CVector3 CEuler::ToVector3(CVector3 *updir)//�������updirָ�벻Ϊ0�����������ͬʱ����Ϸ�������updir�С�
{
	CVector3 p, q;
	CMatrix t1;
	//(*this).h *= 180 / 3.14; (*this).p *= 180 / 3.14;(*this).b *= 180 / 3.14;
	t1 = (*this).ToMatrix();
	//t1 = CMatrix::GetInverse(t1, 4);//error2

	p = t1.vecMul(CVector3(0, 0, -1, 0));
	p.m_dY /= p.m_dX;
	p.m_dZ /= p.m_dX;
	p.m_dX /= -p.m_dX;

	q.Set(0, 1, 0, 0);
	updir = &t1.vecMul(q);
	return p;
}
CMatrix CEuler::ToMatrix()//ŷ����ת��Ϊ����
{
	CMatrix t1;
	t1.m00 = cosf((*this).h * 3.14 / 180) * cosf((*this).b * 3.14 / 180) + sinf((*this).h * 3.14 / 180) * sinf((*this).p * 3.14 / 180) *sinf((*this).b * 3.14 / 180);
	t1.m10 = -cosf((*this).h * 3.14 / 180) * sinf((*this).b * 3.14 / 180) + sinf((*this).h * 3.14 / 180) * sinf((*this).p * 3.14 / 180) *cosf((*this).b * 3.14 / 180);//errora
	t1.m20 = sinf((*this).h * 3.14 / 180) * cosf((*this).p * 3.14 / 180);
	t1.m30 = 0.0f;

	t1.m01 = sinf((*this).b * 3.14 / 180) * cosf((*this).p * 3.14 / 180);
	t1.m11 = cosf((*this).b * 3.14 / 180) * cosf((*this).p * 3.14 / 180);
	t1.m21 = -sinf((*this).p * 3.14 / 180);
	t1.m31 = 0.0f;

	t1.m02 = -sinf((*this).h * 3.14 / 180) * cosf((*this).b * 3.14 / 180) + cosf((*this).h * 3.14 / 180) * sinf((*this).p * 3.14 / 180) *sinf((*this).b * 3.14 / 180);
	t1.m12 = sinf((*this).h * 3.14 / 180) * sinf((*this).b * 3.14 / 180) + cosf((*this).h * 3.14 / 180) * sinf((*this).p * 3.14 / 180) *cosf((*this).b * 3.14 / 180);
	t1.m22 = cosf((*this).h * 3.14 / 180) * cosf((*this).p * 3.14 / 180);
	t1.m32 = 0.0f;

	t1.m03 = 0.0f;
	t1.m13 = 0.0f;
	t1.m23 = 0.0f;
	t1.m33 = 1.0f;
	return t1;
}

void CEuler::Normal()//ŷ���ǹ淶��,h,r�Ƕ���-180~180��p�Ƕ���-90~90��������ʱ,h��������r�Ķ�����
{

	if ((*this).h > 180)
	{
		(*this).h -= 360;
		(*this).Normal();
	}
	else if ((*this).h < -180)
	{
		(*this).h += 360;
		(*this).Normal();
	}
	else if ((*this).p > 90)
	{
		(*this).p -= 180;
		(*this).Normal();
	}
	else if ((*this).p < -90)
	{
		(*this).p += 180;
		(*this).Normal();
	}
	else if ((*this).b > 180)
	{
		(*this).b -= 360;
		(*this).Normal();
	}
	else if ((*this).b < -180)
	{
		(*this).b += 360;
		(*this).Normal();
	}
	if ((*this).p == 90 || (*this).p == -90)
	{
		(*this).b = (*this).h;
	}
}
CQuaternion CEuler::ToQuaternion()//ŷ����ת��Ϊ��Ԫ��
{
	CQuaternion cq1;
	cq1.w = cosf(((*this).h / 2.0) * 3.14 / 180) * cosf(((*this).p / 2.0) * 3.14 / 180) * cosf((*this).b / 2.0 * 3.14 / 180)
		+ sinf((*this).h / 2.0 * 3.14 / 180) * sinf((*this).p / 2.0 * 3.14 / 180) * sinf((*this).b / 2.0 * 3.14 / 180);
	cq1.x = cosf((*this).h / 2.0 * 3.14 / 180) * sinf((*this).p / 2.0 * 3.14 / 180) * cosf((*this).b / 2.0 * 3.14 / 180)
		+ sinf((*this).h / 2.0 * 3.14 / 180) * cosf((*this).p / 2.0 * 3.14 / 180) * sinf((*this).b / 2.0 * 3.14 / 180);
	cq1.y = sinf((*this).h / 2.0 * 3.14 / 180) * cosf((*this).p / 2.0 * 3.14 / 180) * cosf((*this).b / 2.0 * 3.14 / 180)
		- cosf((*this).h / 2.0 * 3.14 / 180) * sinf((*this).p / 2.0 * 3.14 / 180) * sinf((*this).b / 2.0 * 3.14 / 180);
	cq1.z = cosf((*this).h / 2.0 * 3.14 / 180) * cosf((*this).p / 2.0 * 3.14 / 180) * sinf((*this).b / 2.0 * 3.14 / 180)
		- sinf((*this).h / 2.0 * 3.14 / 180) * sinf((*this).p / 2.0 * 3.14 / 180) * cosf((*this).b / 2.0 * 3.14 / 180);
	cq1.Normalize();
	return cq1;
}