#include "stdafx.h"
#include<stdio.h>
#include<iostream>
#include<string>
#include<gl/glut.h>
#include "CQuaternion.h"
using namespace std;
float Max(float a, float b)
{
	return a > b ? a : b;
}
CQuaternion ToQuaternion(CEuler e1)//欧拉角转换为四元数
{
	CQuaternion cq1;
	cq1.w = cosf((e1.h / 2.0) * 3.14 / 180) * cosf((e1.p / 2.0) * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180)
		+ sinf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180);
	cq1.x = cosf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180)
		+ sinf(e1.h / 2.0 * 3.14 / 180) * cosf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180);
	cq1.y = sinf(e1.h / 2.0 * 3.14 / 180) * cosf(e1.p / 2.0 * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180)
		- cosf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180);
	cq1.z = cosf(e1.h / 2.0 * 3.14 / 180) * cosf(e1.p / 2.0 * 3.14 / 180) * sinf(e1.b / 2.0 * 3.14 / 180)
		- sinf(e1.h / 2.0 * 3.14 / 180) * sinf(e1.p / 2.0 * 3.14 / 180) * cosf(e1.b / 2.0 * 3.14 / 180);
	return cq1;
}

CEuler ToEuler(CMatrix t1)
{
	CEuler e1;
	e1.p = (asinf(-t1.m12)) * 360 / (2*3.14);
	if (cosf(e1.p) == 0)
	{
		e1.h = (acosf(t1.m00)) * 180 / 3.14;
		e1.b = 0;
	}
	else
	{
		e1.h = (atan2f(t1.m02, t1.m22)) * 180 / 3.14;
		e1.b = (atan2f(t1.m10, t1.m11)) * 180 / 3.14;
	}
	return e1;
}
CQuaternion ToQuaternion(CMatrix t1)//矩阵转换为四元数
{
	CQuaternion cq1;
	cq1.w = sqrt(t1.m00 + t1.m11 + t1.m22 + 1) / 2.0f;
	cq1.x = sqrt(t1.m00 - t1.m11 - t1.m22 + 1) / 2.0f;
	cq1.y = sqrt(-t1.m00 + t1.m11 - t1.m22 + 1) / 2.0f;
	cq1.z = sqrt(-t1.m00 - t1.m11 + t1.m22 + 1) / 2.0f;
	if (cq1.w > Max(cq1.x, Max(cq1.y, cq1.z)))
	{
		float w = (4 * cq1.w);
		cq1.x = (t1.m21 - t1.m12) / w;
		cq1.y = (t1.m02 - t1.m20) / w;
		cq1.z = (t1.m10 - t1.m01) / w;//error1
	}
	else if (cq1.x > Max(cq1.w, Max(cq1.y, cq1.z)))
	{
		cq1.w = (t1.m21 - t1.m12) / (4 * cq1.x);
		cq1.y = (t1.m01 + t1.m10) / (4 * cq1.x);
		cq1.z = (t1.m20 + t1.m02) / (4 * cq1.x);
	}
	else if (cq1.y > Max(cq1.w, Max(cq1.x, cq1.z)))
	{
		cq1.w = (t1.m02 - t1.m20) / (4 * cq1.y);
		cq1.x = (t1.m01 + t1.m10) / (4 * cq1.y);
		cq1.z = (t1.m12 + t1.m21) / (4 * cq1.y);
	}
	else if (cq1.z > Max(cq1.w, Max(cq1.y, cq1.x)))
	{
		cq1.w = (t1.m10 - t1.m01) / (4 * cq1.z);
		cq1.x = (t1.m20 + t1.m02) / (4 * cq1.z);
		cq1.y = (t1.m12 + t1.m21) / (4 * cq1.z);
	}
	return cq1;
}

CEuler ToEuler(CVector3 ptest){
	float h, p, b;
	CVector3 p1, p2, p3;
	CEuler e1;
	p1.Set(ptest.m_dX, 0, ptest.m_dZ, 0);
	p2.Set(ptest.m_dX, ptest.m_dY, 0, 0);
	p3.Set(0, ptest.m_dY, ptest.m_dZ, 0);
	
	h = -acosf(p1.dotMul(CVector3(0, 0, -1, 0)) / (p1.len())) * 180 / 3.14;
	p = acosf(ptest.dotMul(p1) / (ptest.len() * p1.len())) * 180 / 3.14;
	b = 0;//acosf(p3.dotMul(CVector3(0, 0, -1, 0)) / (p3.len())) * 180 / 3.14;
	e1.Set(h, p, b);
	return e1;
}
void CM_print(CMatrix temp)
{
	for (int i = 0; i < 16; i++){
		if (!(i % 4) && i != 0)
			std::cout << endl;
		if (i == 15)
		{
			std::cout << temp[15];
			break;
		}
		else
		{
			std::cout << temp[i] << ",";
		}
	}
	printf("\n");
}
void int2(string type)
{
	cout << type << endl;
	if (type == "向量转换欧拉角")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CVector3 p(t1, t2, t3, 0);
		CEuler e1;
		e1 = ToEuler(p);
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "欧拉角转换向量")/*error1*/
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		e1.h = e1.h / 180 * 3.14;
		e1.p = e1.p / 180 * 3.14; 
		e1.b = e1.b / 180 * 3.14;
		CVector3 p;
		CVector3 updir;
		p = e1.ToVector3(&updir);
		printf("%.2f,%.2f,%.2f\n", p.m_dX, p.m_dY, p.m_dZ);
	}
	else if (type == "欧拉角转换四元数")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		CQuaternion cq1;
		cq1 = ToQuaternion(e1);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数转换欧拉角")
	{
		float t1, t2, t3 , t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3,&t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		CEuler e1;
		e1 = cq1.ToEuler();
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "欧拉角转换矩阵")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		CMatrix cm1;
		cm1 = e1.ToMatrix();
		CM_print(cm1);
	}
	else if (type == "矩阵转换欧拉角")
	{
		CMatrix cm1;
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f\n", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
			//temp[i] = 1;
		}
		cm1.Set(temp);
		CEuler e1;
		e1 = ToEuler(cm1);
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "矩阵转换四元数")
	{
		CMatrix cm1;
		float temp[16];
		for (int i = 0; i < 16; i++)
		{
			if (i == 15)
			{
				scanf("%f\n", &temp[i]);
				break;
			}
			scanf("%f,", &temp[i]);
		}
		cm1.Set(temp);
		CQuaternion cq1;
		cq1 = ToQuaternion(cm1);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数转换矩阵")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		CMatrix cm1;
		cm1 = cq1.ToMatrix();
		//cm1.Transpose();
		CM_print(cm1);
	}
	else if (type == "欧拉角标准化")
	{
		float t1, t2, t3;
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CEuler e1;
		e1.Set(t1, t2, t3);
		e1.Normal();
		printf("%.2f,%.2f,%.2f\n", e1.h, e1.p, e1.b);
	}
	else if (type == "四元数标准化")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		cq1.Normalize();
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数相乘")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		cq1 = cq1 * cq2;
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数求差")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		cq1.Div(cq2);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数点乘")
	{
		float t1, t2, t3, t4 , t5;
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f\n", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		t5 = cq1.dotMul(cq2);
		printf("%.2f\n", t5);
	}
	else if (type == "四元数求逆")
	{
		float t1, t2, t3, t4, t5;
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		cq1 = cq1.Inverse();
		printf("%.2f,%.2f,%.2f,%.2f\n", cq1.w, cq1.x, cq1.y, cq1.z);
	}
	else if (type == "四元数求角度和旋转轴")
	{
		float t1, t2, t3, t4,angle;
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f", &angle);
		scanf("%f,%f,%f\n", &t1, &t2, &t3);
		CVector3 p(t1, t2, t3,0);
		cq1.GetAngle(angle, p);
		printf("angle=%.2f,p:%.2f,%.2f,%.2f\n", angle, p.m_dX, p.m_dY, p.m_dZ);
	}
	else if (type == "四元数插值")
	{
		float t1, t2, t3, t4;
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq1;
		cq1.Set(t1, t2, t3, t4);
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq2;
		cq2.Set(t1, t2, t3, t4);
		float t;
		scanf("%f", &t);
		scanf("%f,%f,%f,%f", &t1, &t2, &t3, &t4);
		CQuaternion cq3;
		cq3.Set(t1, t2, t3, t4);
		cq3 = cq1.Slerp(cq2, t);
		printf("%.2f,%.2f,%.2f,%.2f\n", cq3.w, cq3.x, cq3.y, cq3.z);
	}
}
int main()
{
	string type;
	freopen("test.txt", "r", stdin);
	//freopen("out.txt", "w", stdout);
	while (cin >> type)
	{
		int2(type);
	}
	getchar();
	return 0;
}

