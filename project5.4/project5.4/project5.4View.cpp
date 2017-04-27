
// project5.4View.cpp : Cproject54View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// Cproject54View 构造/析构

Cproject54View::Cproject54View()
{
	// TODO:  在此处添加构造代码

}

Cproject54View::~Cproject54View()
{
}

BOOL Cproject54View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO:  在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Cproject54View 绘制

void Cproject54View::OnDraw(CDC* /*pDC*/)
{
	Cproject54Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	m_gl.OnPaint();

	// TODO:  在此处为本机数据添加绘制代码
}


// Cproject54View 打印

BOOL Cproject54View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Cproject54View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加额外的打印前进行的初始化过程
}

void Cproject54View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO:  添加打印后进行的清理过程
}


// Cproject54View 诊断

#ifdef _DEBUG
void Cproject54View::AssertValid() const
{
	CView::AssertValid();
}

void Cproject54View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cproject54Doc* Cproject54View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cproject54Doc)));
	return (Cproject54Doc*)m_pDocument;
}
#endif //_DEBUG


// Cproject54View 消息处理程序
BOOL Cproject54View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	//直接把所有鼠标键盘消息都发送给OpenGL处理。
	m_gl.PreTranslateMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
	return CView::PreTranslateMessage(pMsg);
}

void Cproject54View::OnInitialUpdate()
{
	
	CView::OnInitialUpdate();
	if (!m_gl.Init(this->GetSafeHwnd()))	//windows窗口生成完毕后，此时初始化OpenGL,把窗口绘制权利交给OpenGL
		AfxMessageBox(_T("error!"));
	SetTimer(1, 30, NULL);
	// TODO: Add your specialized code here and/or call the base class

}

BOOL Cproject54View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	return true;	//直接返回true，禁止windows绘制屏幕底色。绘制工作完全交给OpenGL
	return CView::OnEraseBkgnd(pDC);
}

void Cproject54View::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	m_gl.OnSize(cx, cy);	//当窗口大小调整时候调整OpenGL的视图
	// TODO: Add your message handler code here
}

void Cproject54View::OnTimer(UINT nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	m_gl.OnPaint();
	CView::OnTimer(nIDEvent);
}