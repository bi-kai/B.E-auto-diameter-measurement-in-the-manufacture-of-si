// ChangeRadius.cpp : implementation file
//

#include "stdafx.h"
#include "VideoCaputure.h"
#include "ChangeRadius.h"
#include "SoftKeyboard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChangeRadius dialog


CChangeRadius::CChangeRadius(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeRadius::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeRadius)
	m_radius = 0.0;
	//}}AFX_DATA_INIT
}


void CChangeRadius::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeRadius)
	DDX_Text(pDX, IDC_EDIT1, m_radius);
	//}}AFX_DATA_MAP
}

double CChangeRadius::GetRadius()
{
	return m_radius;
}

BEGIN_MESSAGE_MAP(CChangeRadius, CDialog)
	//{{AFX_MSG_MAP(CChangeRadius)
	ON_EN_SETFOCUS(IDC_EDIT1, OnSetfocusEdit1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeRadius message handlers

void CChangeRadius::OnSetfocusEdit1() 
{
	// TODO: Add your control notification handler code here
	CSoftKeyboard softkeyboard;
		UpdateData();
	CVideoCaputureApp *app = (CVideoCaputureApp *)AfxGetApp(); //生成指向应用程序类的指针
	if(softkeyboard.DoModal() == IDOK)
	{
		m_radius=app->softkeyboard_value;	
		UpdateData(0);
	}
}
