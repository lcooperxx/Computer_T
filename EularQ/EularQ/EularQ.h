#include<math.h>
#include<vector>
#include<iostream>
#include"CMatrix.h"
#include"CVector3.h"
class CEuler
{
public:
	float h, p, b;
	void Set(float h, float p, float b);
	//operator float*(){ return &h;}
	//欧拉角转换为向量
	CVector3 CEuler::ToVector3(CVector3 *updir);//如果传入updir指针不为0，则输出向量同时输出上方向量到updir中。
	CMatrix CEuler::ToMatrix();//欧拉角转换为矩阵CMarix
	//CQuaternion CEuler::ToQuaternion();//欧拉角转换为四元数
	void CEuler::Normal();//欧拉角规范化,h,r角度在-180~180，p角度在-90~90。万向锁时,h代替所有r的度数。
};


void CEuler::Set(float h, float p, float b)
{
	this->h = h;
	this->p = p;
	this->b = b;
}
CVector3 CEuler::ToVector3(CVector3 *updir)//如果传入updir指针不为0，则输出向量同时输出上方向量到updir中。
{
	CVector3 p,q;
	CMatrix t1;
	//(*this).h *= 180 / 3.14; (*this).p *= 180 / 3.14;(*this).b *= 180 / 3.14;
	t1 = (*this).ToMatrix();
	t1 = GetInverse(t1,4);
	
	p = t1.vecMul(CVector3(0, 0, -1, 0));
	p.m_dY /= p.m_dX; 
	p.m_dZ /= p.m_dX;
	p.m_dX /= -p.m_dX;

	q.Set(0, 1, 0, 0);
	updir = &t1.vecMul(q);
	return p;
}
CMatrix CEuler::ToMatrix()//欧拉角转换为矩阵
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

void CEuler::Normal()//欧拉角规范化,h,r角度在-180~180，p角度在-90~90。万向锁时,h代替所有r的度数。
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