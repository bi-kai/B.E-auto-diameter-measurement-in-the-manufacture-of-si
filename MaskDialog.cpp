// MaskDialog.cpp : implementation file
//

#include "stdafx.h"
#include "VideoCaputure.h"
#include "MaskDialog.h"
#include <winuser.h>
#include <windows.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMaskDialog dialog


CMaskDialog::CMaskDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMaskDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMaskDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMaskDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMaskDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMaskDialog, CDialog)
	//{{AFX_MSG_MAP(CMaskDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMaskDialog message handlers


BOOL CMaskDialog::OnInitDialog(){

	CDialog::OnInitDialog();
	m_startPoint= CPoint(0,0);
	m_endPoint= CPoint(0,0);


	SetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE,   
		GetWindowLong(this->GetSafeHwnd(),GWL_EXSTYLE)^0x80000);   
	HINSTANCE   hInst   =   LoadLibrary("User32.DLL");     
	if(hInst)     
	{                           
		typedef   BOOL   (WINAPI   *MYFUNC)(HWND,COLORREF,BYTE,DWORD);                       
		MYFUNC   fun   =   NULL;               
		fun=(MYFUNC)GetProcAddress(hInst,"SetLayeredWindowAttributes");   
		if(fun)
			fun(this->GetSafeHwnd(),GetSysColor(COLOR_3DFACE),50,2); 
		FreeLibrary(hInst);     
	} 

	m_cap = FALSE;

	return FALSE;

}


void CMaskDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_cap = TRUE;
	m_startPoint.x = point.x;
	m_startPoint.y = point.y;
	HCURSOR hCurs1 = LoadCursor(NULL,IDC_CROSS);
	SetCursor(hCurs1);
	SetCapture();
	CDialog::OnLButtonDown(nFlags, point);
}

void CMaskDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_cap){
		m_cap = FALSE;
		m_endPoint.x = point.x;
		m_endPoint.y = point.y;
		m_lastRect = NULL;
		ReleaseCapture();
	}
	CDialog::OnLButtonUp(nFlags, point);
}

void CMaskDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_cap){
		CDC *pDC = GetDC();
		CRect *pRect;
		CBrush brush(RGB(255,255,255));
		if(point.x >= m_startPoint.x && point.y >= m_startPoint.y)
		{
			pRect = new CRect(m_startPoint.x,m_startPoint.y,point.x,point.y);
		}
		else if(point.x >= m_startPoint.x && point.y < m_startPoint.y)
		{
			pRect = new CRect(m_startPoint.x,point.y,point.x,m_startPoint.y);	
		}
		else if(point.x < m_startPoint.x && point.y >= m_startPoint.y)
		{
			pRect = new CRect(point.x,m_startPoint.y,m_startPoint.x,point.y);
		}
		else
		{
			pRect = new CRect(point.x,point.y,m_startPoint.x,m_startPoint.y);
		}
		pDC->DrawDragRect(pRect,CSize(2,2),&m_lastRect,CSize(2,2),&brush,&brush);
		
		m_lastRect = *pRect;
		
		ReleaseDC(pDC);
		
		free(pRect);
	}
	
	CDialog::OnMouseMove(nFlags, point);
}


void CMaskDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default

	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}
