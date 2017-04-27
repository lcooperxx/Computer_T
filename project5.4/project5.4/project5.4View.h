
// project5.4View.h : Cproject54View ��Ľӿ�
//

#pragma once

#include "CmyOpenGL.h"
class Cproject54View : public CView
{
	CmyOpenGL m_gl;
protected: // �������л�����
	Cproject54View();
	DECLARE_DYNCREATE(Cproject54View)

// ����
public:
	Cproject54Doc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnInitialUpdate();
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~Cproject54View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	//{{AFX_MSG(CProject54View)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // project5.4View.cpp �еĵ��԰汾
inline Cproject54Doc* Cproject54View::GetDocument() const
   { return reinterpret_cast<Cproject54Doc*>(m_pDocument); }
#endif

