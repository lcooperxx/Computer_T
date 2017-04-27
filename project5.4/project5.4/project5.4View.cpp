
// project5.4View.cpp : Cproject54View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "project5.4.h"
#endif

#include "project5.4Doc.h"
#include "project5.4View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cproject54View

IMPLEMENT_DYNCREATE(Cproject54View, CView)

BEGIN_MESSAGE_MAP(Cproject54View, CView)
	//{{AFX_MSG_MAP(CProject54View)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cproject54View ����/����

Cproject54View::Cproject54View()
{
	// TODO:  �ڴ˴���ӹ������

}

Cproject54View::~Cproject54View()
{
}

BOOL Cproject54View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// Cproject54View ����

void Cproject54View::OnDraw(CDC* /*pDC*/)
{
	Cproject54Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_gl.OnPaint();

	// TODO:  �ڴ˴�Ϊ����������ӻ��ƴ���
}


// Cproject54View ��ӡ

BOOL Cproject54View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void Cproject54View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void Cproject54View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  ��Ӵ�ӡ����е��������
}


// Cproject54View ���

#ifdef _DEBUG
void Cproject54View::AssertValid() const
{
	CView::AssertValid();
}

void Cproject54View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cproject54Doc* Cproject54View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cproject54Doc)));
	return (Cproject54Doc*)m_pDocument;
}
#endif //_DEBUG


// Cproject54View ��Ϣ�������
BOOL Cproject54View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//ֱ�Ӱ�������������Ϣ�����͸�OpenGL����
	m_gl.PreTranslateMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);
}

void Cproject54View::OnInitialUpdate()
{
	
	CView::OnInitialUpdate();
	if (!m_gl.Init(this->GetSafeHwnd()))	//windows����������Ϻ󣬴�ʱ��ʼ��OpenGL,�Ѵ��ڻ���Ȩ������OpenGL
		AfxMessageBox(_T("error!"));
	SetTimer(1, 30, NULL);
	// TODO: Add your specialized code here and/or call the base class

}

BOOL Cproject54View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;	//ֱ�ӷ���true����ֹwindows������Ļ��ɫ�����ƹ�����ȫ����OpenGL
	return CView::OnEraseBkgnd(pDC);
}

void Cproject54View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_gl.OnSize(cx, cy);	//�����ڴ�С����ʱ�����OpenGL����ͼ
	// TODO: Add your message handler code here
}

void Cproject54View::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnPaint();
	CView::OnTimer(nIDEvent);
}