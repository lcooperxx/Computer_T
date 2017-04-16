#include "stdafx.h"
#include<math.h>
#pragma once
#include<vector>
#include<iostream>
#include <gl/glut.h>
#include"CVector3.h"
#define N 4
class CMatrix
{
public:
	float m00, m10, m20, m30;
	float m01, m11, m21, m31;
	float m02, m12, m22, m32;
	float m03, m13, m23, m33;
	CMatrix();
	//CMatrix(float m00, float m01, float m02, float m03,
	//	float m10, float m11, float m12, float m13,
	//	float m20, float m21, float m22, float m23,
	//	float m30, float m31, float m32, float m33
	//	)//��ֵ����
	//{
	//	this->m00 = m00; this->m01 = m01; this->m02 = m02; this->m03 = m03;
	//	this->m10 = m10; this->m11 = m11; this->m12 = m12; this->m13 = m13;
	//	this->m20 = m20; this->m21 = m21; this->m22 = m22; this->m23 = m23;
	//	this->m30 = m30; this->m31 = m31; this->m32 = m32; this->m33 = m33;
	//}

	CMatrix(float *p);
	void Identity();		//����Ϊ��λ����
	void Set(float *p);	//������16��Ԫ�ظ�ֵ
	void SetRow(int i, CVector3 p);//���õ�i��Ԫ��
	void SetCol(int j, CVector3 p);	//���õ�i��Ԫ������
	CMatrix operator=(const CMatrix& p);//����ֵ
	CMatrix operator*(float d);//��������
	CMatrix operator*(const CMatrix& p);//�������
	CVector3 vecMul(CVector3& p); //�������������
	CVector3 posMul(CVector3& p);//����������
	void Transpose();//����ת��
	CMatrix& SetRotate(float seta, CVector3 axis);//����Ϊ��ת����
	CMatrix& SetTrans(CVector3 trans);	//����Ϊƽ�ƾ���
	CMatrix& SetScale1(float k, CVector3 p);	//����Ϊ���ž���
	CMatrix& SetScale(CVector3 p);	//����Ϊ���ž���
	float Inverse(CMatrix arcs, int n);//��������,�ɹ���������ʽ��ֵ�����򷵻�0
	CMatrix GetInverse(CMatrix src, int n);//���������
	CMatrix  getAStart(CMatrix arcs, int n);//����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
	CMatrix run();
	operator float*(){ return &m00; }
	operator const float*()const { return &m00; }
};

CMatrix::CMatrix(float *p){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
		{
			(*this)[i + 4 * j] = p[i];
		}

	}
}
CMatrix::CMatrix(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
		{
			(*this)[i + 4 * j] = 0;
		}

	}
}
void CMatrix::Identity()//�Խ�����1������Ϊ0
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (i == j)
			{
				(*this)[i * 4 + j] = 1;
			}
			else
			{
				(*this)[i * 4 + j] = 0;
			}
		}
	}
}
void CMatrix::Set(float *p)
{
	int k = 0;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			(*this)[i + j * 4] = p[k++];
		}
	}
}
void CMatrix::SetRow(int i, CVector3 p)//���õ�i��Ԫ��
{
	/*int star = i * 4;
	for ( int j = 0; j < 3; j++){
	(*this)[star++] = p[j];
	}
	(*this)[star] = 0;*/
	(*this)[i] = p.m_dX;
	(*this)[i + 4] = p.m_dY;
	(*this)[i + 8] = p.m_dZ;
	if (i < 3)
	{
		(*this)[i + 12] = 0;
	}
	else
	{
		(*this)[i + 12] = 1;
	}
}
void CMatrix::SetCol(int i, CVector3 p)//���õ�i��Ԫ��
{
	(*this)[i] = p.m_dX;
	(*this)[i + 1] = p.m_dY;
	(*this)[i + 2] = p.m_dZ;
	if (i < 3)
	{
		(*this)[i * 4 + 3] = 0;
	}
	else
	{
		(*this)[i * 4 + 3] = 1;
	}
}
CMatrix CMatrix::operator=(const CMatrix& p)//����ֵ
{
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = p[i];
	}
	return (*this);
}
CMatrix CMatrix :: operator*(float d)//��������
{
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = (*this)[i] * d;
	}
	return *this;
}
CMatrix CMatrix :: operator*(const CMatrix& p)//�������
{
	CMatrix ans;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++){
				ans[i * 4 + j] += (*this)[i * 4 + k] * p[k * 4 + j];
			}
		}
	}
	return ans;
}
CVector3 CMatrix::vecMul(CVector3& p) //�������������
{
	CVector3 t;
	t.Set(0, 0, 0, 0);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			t[i] += p[j] * (*this)[i*4+j];
		}
	}
	return t;
}
CVector3 CMatrix::posMul(CVector3& p)//����������
{
	CVector3 t;
	t.Set(0, 0, 0, 1);
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			t[i] += p[j] * (*this)[i * 4 + j];
		}
	}
	return t;
}
void CMatrix::Transpose()//����ת��
{
	CMatrix p = *this;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{

			(*this)[i * 4 + j] = p[j * 4 + i];
		}
	}
}
//����һ��չ������|A|
float Inverse(CMatrix arcs, int n)
{
	if (n == 1)
	{
		return arcs[0];
	}
	float ans = 0;
	CMatrix temp;
	int i, j, k;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n - 1; j++)
		{
			for (k = 0; k<n - 1; k++)
			{
				temp[j * 4 + k] = arcs[(j + 1) * 4 + ((k >= i) ? k + 1 : k)];
			}
		}
		double t = Inverse(temp, n - 1);
		if (i % 2 == 0)
		{
			ans += arcs[i] * t;
		}
		else
		{
			ans -= arcs[i] * t;
		}
	}
	return ans;
}
//����ÿһ��ÿһ�е�ÿ��Ԫ������Ӧ������ʽ�����A*
CMatrix  getAStart(CMatrix arcs, int n)
{
	CMatrix ans;
	if (n == 1)
	{
		ans[0] = 1;
		return 0;
	}
	int i, j, k, t;
	CMatrix temp;
	for (i = 0; i<n; i++)
	{
		for (j = 0; j<n; j++)
		{
			for (k = 0; k<n - 1; k++)
			{
				for (t = 0; t<n - 1; t++)
				{
					temp[k * 4 + t] = arcs[(k >= i ? (k + 1) * 4 : k * 4) + (t >= j ? t + 1 : t)];
				}
			}
			ans[j * 4 + i] = Inverse(temp, n - 1);  //�˴�˳�������ת��
			if ((i + j) % 2 == 1)
			{
				ans[j * 4 + i] = -ans[j * 4 + i];
			}
		}
	}
	return ans;
}
//�õ���������src������󱣴浽des�С�
CMatrix GetInverse(CMatrix src, int n)
{
	CMatrix des;
	float flag = Inverse(src, n);
	if (0 == flag)
	{
		std::cout << "����ʽ=0" << std::endl;
		return des;//���������������ʽΪ0�������½���
	}
	else
	{
		//printf("������=%d\n", flag);
		CMatrix t = getAStart(src, n);
		for (int i = 0; i<n; i++)
		{
			for (int j = 0; j<n; j++)
			{
				des[i * 4 + j] = t[i * 4 + j] / flag;
			}
		}
	}
	return des;
}
CMatrix& CMatrix::SetRotate(float seta, CVector3 axis)//����Ϊ��ת����
{
	float u = axis.m_dX;
	float v = axis.m_dY;
	float w = axis.m_dZ;

	(*this)[0] = cosf(seta) + (u * u) * (1 - cosf(seta));
	(*this)[1] = u * v * (1 - cosf(seta)) + w * sinf(seta);
	(*this)[2] = u * w * (1 - cosf(seta)) - v * sinf(seta);
	(*this)[3] = 0;

	(*this)[4] = u * v * (1 - cosf(seta)) - w * sinf(seta);
	(*this)[5] = cosf(seta) + v * v * (1 - cosf(seta));
	(*this)[6] = w * v * (1 - cosf(seta)) + u * sinf(seta);
	(*this)[7] = 0;

	(*this)[8] = u * w * (1 - cosf(seta)) + v * sinf(seta);
	(*this)[9] = v * w * (1 - cosf(seta)) - u * sinf(seta);
	(*this)[10] = cosf(seta) + w * w * (1 - cosf(seta));
	(*this)[11] = 0;

	(*this)[12] = 0;
	(*this)[13] = 0;
	(*this)[14] = 0;
	(*this)[15] = 1;
	return (*this);
}
CMatrix& CMatrix::SetScale1(float k, CVector3 axis)//����Ϊ��ת����
{
	float u = axis.m_dX;
	float v = axis.m_dY;
	float w = axis.m_dZ;

	(*this)[0] = 1 + (u * u) * (k - 1);
	(*this)[1] = u * v * (k - 1);
	(*this)[2] = u * w * (k - 1);
	(*this)[3] = 0;

	(*this)[4] = u * v * (k - 1);
	(*this)[5] = 1 + v * v * (k - 1);
	(*this)[6] = w * v * (k - 1);
	(*this)[7] = 0;

	(*this)[8] = u * w * (k - 1);
	(*this)[9] = v * w * (k - 1);
	(*this)[10] = 1 + w * w * (k - 1);
	(*this)[11] = 0;

	(*this)[12] = 0;
	(*this)[13] = 0;
	(*this)[14] = 0;
	(*this)[15] = 1;
	return (*this);
}
CMatrix& CMatrix::SetScale(CVector3 p)	//����Ϊ���ž���
{
	(*this)[0] = p.m_dX;
	(*this)[5] = p.m_dY;
	(*this)[10] = p.m_dZ;
	(*this)[15] = 1;
	return (*this);
}
CMatrix& CMatrix::SetTrans(CVector3 trans)	//����Ϊƽ�ƾ���
{
	(*this)[0] = (*this)[5] = (*this)[10] = (*this)[15] = 1;
	(*this).m03 = trans.m_dX;
	(*this).m13 = trans.m_dY;
	(*this).m23 = trans.m_dZ;
	return (*this);
}
CMatrix CMatrix::run()
{
	glMultMatrixf(*this);
	return (*this);
}