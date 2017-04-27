#include"stdafx.h"
#include<math.h>
#pragma once
#include<vector>
#include<iostream>
#include <gl/glut.h>
#include"total.h"
#define N 4


CMatrix::CMatrix(double *p){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
		{
			(*this)[i + 4 * j] = p[i];
		}

	}
}
CMatrix::~CMatrix()
{

}
CMatrix::CMatrix(){
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++)
		{
			(*this)[i + 4 * j] = 0;
		}

	}
}
void CMatrix::Identity()//对角线是1，其余为0
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
void CMatrix::Set(double *p)
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
void CMatrix::SetRow(int i, CVector3 p)//设置第i行元素
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
void CMatrix::SetCol(int i, CVector3 p)//设置第i列元素
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

CMatrix& CMatrix::operator=(const CMatrix& p)//矩阵赋值
{
	CMatrix temp = p;
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = temp[i];
	}
	return (*this);
}
CMatrix& CMatrix::operator=(CMatrix& p)//矩阵赋值
{
	CMatrix temp = p;
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = temp[i];
	}
	return (*this);
}
CMatrix CMatrix :: operator*(double d)//矩阵数乘
{
	for (int i = 0; i < 16; i++)
	{
		(*this)[i] = (*this)[i] * d;
	}
	return *this;
}
CMatrix CMatrix :: operator*(CMatrix& p)//矩阵相乘
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
CVector3 CMatrix::vecMul(CVector3& p) //矩阵与向量相乘
{
	CVector3 t;
	t.Set(0, 0, 0, 0);
	for (int i = 0; i < 4; i++){
		for (int j = 0; j < 4; j++){
			t[i] += p[j] * (*this)[i * 4 + j];
		}
	}
	return t;
}
CVector3 CMatrix::posMul(CVector3& p)//矩阵与点相乘
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
void CMatrix::Transpose()//矩阵转置
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
//按第一行展开计算|A|
double Inverse(CMatrix arcs, int n)
{
	if (n == 1)
	{
		return arcs[0];
	}
	double ans = 0;
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
//得到给定矩阵src的逆矩阵保存到des中。
CMatrix CMatrix::GetInverseMatrix(CMatrix Matrix, CMatrix inverseMatrix)
{
	double imat[16];
	int i, j, k;
	for (i = 0; i<16; i++)imat[i] = Matrix[i];
	int is[4];
	int js[4];
	double fDet = 1.0f, temp;
	int f = 1;

	for (k = 0; k < 4; k++)
	{
		// 第一步，全选主元
		double fMax = 0.0f;
		for (i = k; i < 4; i++)
		{
			for (j = k; j < 4; j++)
			{
				const double f = fabs(imat[j * 4 + i]);
				if (f > fMax)
				{
					fMax = f;
					is[k] = i;
					js[k] = j;
				}
			}
		}
		if (fabs(fMax) < 1e-15)
			return 0;

		if (is[k] != k)
		{
			f = -f;
			temp = imat[k]; imat[k] = imat[is[k]]; imat[is[k]] = temp;
			temp = imat[4 + k]; imat[4 + k] = imat[4 + is[k]]; imat[4 + is[k]] = temp;
			temp = imat[8 + k]; imat[8 + k] = imat[8 + is[k]]; imat[8 + is[k]] = temp;
			temp = imat[12 + k]; imat[12 + k] = imat[12 + is[k]]; imat[12 + is[k]] = temp;
		}
		if (js[k] != k)
		{
			f = -f;
			temp = imat[k * 4]; imat[k * 4] = imat[js[k] * 4]; imat[js[k] * 4] = temp;
			temp = imat[k * 4 + 1]; imat[k * 4 + 1] = imat[js[k] * 4 + 1]; imat[js[k] * 4 + 1] = temp;
			temp = imat[k * 4 + 2]; imat[k * 4 + 2] = imat[js[k] * 4 + 2]; imat[js[k] * 4 + 2] = temp;
			temp = imat[k * 4 + 3]; imat[k * 4 + 3] = imat[js[k] * 4 + 3]; imat[js[k] * 4 + 3] = temp;
		}

		// 计算行列值
		fDet *= imat[k * 4 + k];

		// 计算逆矩阵

		// 第二步
		imat[k * 4 + k] = 1.0 / imat[k * 4 + k];
		// 第三步
		for (j = 0; j < 4; j++)
		{
			if (j != k)
				imat[j * 4 + k] *= imat[k * 4 + k];
			//m(k, j) *= m(k, k);
		}
		// 第四步
		for (i = 0; i < 4; i++)
		{
			if (i != k)
			{
				for (j = 0; j < 4; j++)
				{
					if (j != k)
						imat[j * 4 + i] -= imat[k * 4 + i] * imat[j * 4 + k];
					//m(i, j) = m(i, j) - m(i, k) * m(k, j);
				}
			}
		}
		// 第五步
		for (i = 0; i < 4; i++)
		{
			if (i != k)
				imat[k * 4 + i] *= -imat[k * 4 + k];
			//m(i, k) *= -m(k, k);
		}
	}

	for (k = 3; k >= 0; k--)
	{
		if (js[k] != k)
		{
			temp = imat[k]; imat[k] = imat[js[k]]; imat[js[k]] = temp;
			temp = imat[4 + k]; imat[4 + k] = imat[4 + js[k]]; imat[4 + js[k]] = temp;
			temp = imat[8 + k]; imat[8 + k] = imat[8 + js[k]]; imat[8 + js[k]] = temp;
			temp = imat[12 + k]; imat[12 + k] = imat[12 + js[k]]; imat[12 + js[k]] = temp;
		}
		if (is[k] != k)
		{
			temp = imat[k * 4]; imat[k * 4] = imat[is[k] * 4]; imat[is[k] * 4] = temp;
			temp = imat[1 + k * 4]; imat[1 + k * 4] = imat[1 + is[k] * 4]; imat[1 + is[k] * 4] = temp;
			temp = imat[2 + k * 4]; imat[2 + k * 4] = imat[2 + is[k] * 4]; imat[2 + is[k] * 4] = temp;
			temp = imat[3 + k * 4]; imat[3 + k * 4] = imat[3 + is[k] * 4]; imat[3 + is[k] * 4] = temp;
		}
	}
	for (i = 0; i<16; i++)
		inverseMatrix[i] = imat[i];
	return inverseMatrix;
}
CMatrix& CMatrix::SetRotate(double seta, CVector3 axis)//设置为旋转矩阵
{
	double u = axis.m_dX;
	double v = axis.m_dY;
	double w = axis.m_dZ;

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
CMatrix& CMatrix::SetScale1(double k, CVector3 axis)//设置为旋转矩阵
{
	double u = axis.m_dX;
	double v = axis.m_dY;
	double w = axis.m_dZ;

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
CMatrix& CMatrix::SetScale(CVector3 p)	//设置为缩放矩阵
{
	(*this)[0] = p.m_dX;
	(*this)[5] = p.m_dY;
	(*this)[10] = p.m_dZ;
	(*this)[15] = 1;
	return (*this);
}
CMatrix& CMatrix::SetTrans(CVector3 trans)	//设置为平移矩阵
{
	(*this)[0] = (*this)[5] = (*this)[10] = (*this)[15] = 1;
	(*this).m03 = trans.m_dX;
	(*this).m13 = trans.m_dY;
	(*this).m23 = trans.m_dZ;
	return (*this);
}
CMatrix CMatrix::run()
{
	glMultMatrixd(*this);
	return (*this);
}
double Max(double a, double b)
{
	return a > b ? a : b;
}
CQuaternion CMatrix::ToQuaternion()//矩阵转换为四元数
{
	CQuaternion cq1;
	cq1.w = sqrt((*this).m00 + (*this).m11 + (*this).m22 + 1) / 2.0f;
	cq1.x = sqrt((*this).m00 - (*this).m11 - (*this).m22 + 1) / 2.0f;
	cq1.y = sqrt(-(*this).m00 + (*this).m11 - (*this).m22 + 1) / 2.0f;
	cq1.z = sqrt(-(*this).m00 - (*this).m11 + (*this).m22 + 1) / 2.0f;
	if (cq1.w > Max(cq1.x, Max(cq1.y, cq1.z)))
	{
		double w = (4 * cq1.w);
		cq1.x = ((*this).m12 - (*this).m21) / w;
		cq1.y = ((*this).m20 - (*this).m02) / w;
		cq1.z = ((*this).m01 - (*this).m10) / w;//error1
	}
	else if (cq1.x > Max(cq1.w, Max(cq1.y, cq1.z)))
	{
		cq1.w = ((*this).m21 - (*this).m12) / (4 * cq1.x);
		cq1.y = ((*this).m01 + (*this).m10) / (4 * cq1.x);
		cq1.z = ((*this).m20 + (*this).m02) / (4 * cq1.x);
	}
	else if (cq1.y > Max(cq1.w, Max(cq1.x, cq1.z)))
	{
		cq1.w = ((*this).m02 - (*this).m20) / (4 * cq1.y);
		cq1.x = ((*this).m01 + (*this).m10) / (4 * cq1.y);
		cq1.z = ((*this).m12 + (*this).m21) / (4 * cq1.y);
	}
	else if (cq1.z > Max(cq1.w, Max(cq1.y, cq1.x)))
	{
		cq1.w = ((*this).m10 - (*this).m01) / (4 * cq1.z);
		cq1.x = ((*this).m20 + (*this).m02) / (4 * cq1.z);
		cq1.y = ((*this).m12 + (*this).m21) / (4 * cq1.z);
	}
	cq1.Normalize();
	return cq1;
}
CEuler CMatrix::ToEuler()
{
	CEuler e1;
	e1.p = (asinf(-(*this).m21)) * 360 / (2 * 3.14);
	if (cosf(e1.p) == 0)
	{
		e1.h = (acosf((*this).m00)) * 180 / 3.14;
		e1.b = 0;
	}
	else
	{
		e1.h = (atan2f((*this).m20, (*this).m22)) * 180 / 3.14;
		e1.b = (atan2f((*this).m01, (*this).m11)) * 180 / 3.14;
	}
	return e1;
}