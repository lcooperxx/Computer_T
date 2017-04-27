
// project5.4View.h : Cproject54View 类的接口
//

#pragma once

#include "CmyOpenGL.h"
class Cproject54View : public CView
{
	CmyOpenGL m_gl;
protected: // 仅从序列化创建
	Cproject54View();
	DECLARE_DYNCREATE(Cproject54View)

// 特性
public:
	Cproject54Doc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~Cproject54View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	//{{AFX_MSG(CProject54View)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // project5.4View.cpp 中的调试版本
inline Cproject54Doc* Cproject54View::GetDocument() const
   { return reinterpret_cast<Cproject54Doc*>(m_pDocument); }
#endif

