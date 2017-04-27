// myOpenGL.h: interface for the CmyOpenGL class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYOPENGL_H__9210654D_319B_4970_950D_A11A11AE5ECB__INCLUDED_)
#define AFX_MYOPENGL_H__9210654D_319B_4970_950D_A11A11AE5ECB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "opengl\total.h"
#define MAX_ARRAY_SIZE 10000
#pragma comment(lib,"opengl/3DEngine4.lib ")
class CmyOpenGL : public COpenGL
{
public:
	//计算顶点的法向量
	virtual void InDraw();
	virtual void PostInit();
	CmyOpenGL();
	virtual ~CmyOpenGL();
	bool m_bWired;
	vector<CVector3> pos;
	vector<int> point;
	vector<CVector3> path;

	int site = 0;
	int f2_type = 0;
	double z[3][10010], visit[10000];
	CMatrix s_pos1, s_pos2;
	CEuler s_hpr1, s_hpr2, s_hpr3;
	CQuaternion s_cq1, s_cq2, s_result;
	CVector3 s_pf, s_pn, s_presult;
	int s_t = 0;
	//double s_t = 2.0;
	double seta = 0;
	double axis = 0;
	//视点位置和方向
	double mx = 0, my = 0, mz = 0.5, rx = 0, ry = 0, rz = 0;//平移和旋转
	double sx = 1, sy = 1, sz = 1;//缩放
	double mspeed = 0.05, rspeed = 0.5;
	double g_IEyeMatt[16] , g_EyeMatt[16];
	CMatrix g_IEyeMat, g_EyeMat;
	int mode = 0;
	int Cmy_flag = 0;

	void Display();
	void Save_Pos();
	void Reverse_Pos();
	void Scat_Jump();

	void SetView();
	void scatter();
	void Scatter_Line();
	void Scatter_triangle();
	void Scat_Initial();


	void Scat_Path();

	void SetRC();
private:
	void DrawModel();
	virtual bool OnKey(unsigned int nChar, bool bDown);
};

#endif // !defined(AFX_MYOPENGL_H__9210654D_319B_4970_950D_A11A11AE5ECB__INCLUDED_)
